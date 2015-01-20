/***********************************************************************************************
* COPYRIGHT 2002 - 2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: TL1 Connection
* Type: C++
* Originator: dtchalak
* File: TL1Connection.h
* Version:
* Description:
* Class responsible to maintain the TL1 protocol connection to the socket.
*
* Revision History:
* Rev 2.5   6/2006    lzou
*  Enhance the first layer of TL1 parser to support large commented response from RTRV-AO etc.
* Rev 2.0   4/2005    aaali
*                     BellSouth FP3.0.1 patch
*                     added functionality to throw exceptions for
*                     malformed messages
*
* Rev 1.7  08/05/2003  lzou
*  add the HandShaking machanism between FM/CM to ProxyServer
* Rev 1.6  07/17/2003  smuthusa
*  pre-process will simply send back the input message.
* Rev 1.5  07/10/2003  lzou
*  clean the _cmdMap in descrutor to fix the crash issue
* Rev 1.4  05/15/2003  aaali
*  Implement the DUAL GNE Feature
* Rev 1.4  03/28/2002  lzou
*  Add the implementation to handle "already login scenario"
* Rev 1.3  02/14/2002  lzou
*     Add the implemenation to retrieve/update the varibles in NeInfo
* Rev 1.3  02/14/2002  lzou
*  Add the implementation for logout and break TL1 message block
* Rev 1.2  01/10/2002  lzou
*  Add the implementation for thread to maintain the session
* Rev 1.1  12/2002   lzou
*  Fixing the problem in incompletemessage check
*  Change the config value used for Tid, Uid, Password, Port so that server use the value
*   from the client instead of config file.
* Rev 1.0  init version  dtchalak
*
*
**********************************************************************************************/


// aaali added for missing event detection
#define MIN_ATAG 1
#define MAX_ATAG 999999999
// chenge this to 1 in the production code. it was set to 2 for testing
#define MAX_GAP  1

#include <vector>
#include <algorithm>

#include "EmsUnixDefines.h"
#include "TL1Connection.h"
#include "TL1ConnectionManager.h"
#include "SocketHandler.h"
#include "sysErrLog.h"
#include "ConfigNames.h"
#include "Server_i.h"
#include "NetworkElement.h"
#include "TL1LoginCmd.h"
#include "TL1LogoutCmd.h"
#include "TL1Lexer.h"
#include "TL1Parser.h"
#include "tutil.h"
#include "TL1MsgConverter.h"
#include "EmsTypes.h"
#include "NodeDefines.h"
#include "TL1SocketAcceptor.h"
#include "TL1BaseCmd.h"
#include "CmNameResolver.h"
#include <ospace/helper.h>
#include "TL1Defines.h"
#include "EMSDefines.h"

extern const char* sourceName;
extern const char* ServerIdName;
extern const char* CtagServerPrefix;
const char* FmIdName = "F";
const char* CmIdName = "C";


using namespace EMS_FW;
using namespace ELogger;
using namespace TL1_MA;
using namespace LexerTL1;


const string TL1Connection::className = "TL1Connection";
const int Eos = 0x10;

//------------------------------------------------------------------------------------------
// Constructor/Destructor and initialization methods
//------------------------------------------------------------------------------------------
TL1Connection* TL1Connection::getInstance(const string& neId, const string& name)
{
    TL1Connection* con = NULL;
    if (name.size() != 0)
        con = (TL1Connection*) Server_i::instance()->getComponent(name, neId);
    else
    {
        con = (TL1Connection*) Server_i::instance()->getComponent(className, neId);
    }
    return con;
}


TL1Connection::TL1Connection(const string& neId, unsigned portNumber, const string& hostName, bool
                             AMSUHCSimulatorSupport, const string& conTimeout, const string& conRetry)
        : SocketConnection(portNumber, hostName, conTimeout, conRetry)
{
    setClassName(className);
    char buffer[10];
    string name = hostName + "." + itoa(portNumber, buffer, 10);
    setName(name);
    _neId = neId;
    _locHandler = NULL;
    _AMSUHCSimulatorSupport = AMSUHCSimulatorSupport;
    _expectedMsg = "";
    _acceptNewCmd = true;
}

TL1Connection::~TL1Connection()
{
    ErrorLogger::logVarError(sourceName, "TL1Connection", "~TL1Connection", __LINE__,
                             TRACE4, "Descructor begin ...");
    _acceptNewCmd = false;
    _incompleteResMap.clear();
    clearCmdMap();
    finalize();
    ErrorLogger::logVarError(sourceName, "TL1Connection", "~TL1Connection", __LINE__,
                             TRACE4, "Descructor ends ...");

}

bool TL1Connection::initialize()
{
    ErrorLogger::logVarError(sourceName, "TL1Connection", "initialize", __LINE__, TRACE4, "Host is ->%s", _hostname.c_str());
    _waitOnIP = Server_i::instance()->getDefaultConfigValue("TL1WaitOnIP", "false");
    ErrorLogger::logVarError(sourceName, "TL1Connection", "initialize", __LINE__, TRACE4, "Wait on IP is set to %s", _waitOnIP.c_str());


    if (!SocketConnection::initialize())
        _isActive = false;
    else
        _isActive = true;
    //_responseBuffers.clear();
    return _isActive;
}

//------------------------------------------------------------------------------------------
// Local command receivers registration and commad implementation
//------------------------------------------------------------------------------------------
TL1EventReceiver* TL1Connection::_getReceiver(TL1Message *msg, const char *rawMessage)
{
    string ctag = msg->m_tag;
    ErrorLogger::logVarError(sourceName, "TL1Connection", "_getReceiver", __LINE__, TRACE10, "_getReceiver begin with ctag %s ...", ctag.c_str());
    TL1EventReceiver* pCmd = NULL;
    try
    {
        // This code works for the case that there is one response for one command (including IP response).
        // if we need to handle duplicate response case (NE has problems), we need to change this
        // read lock into write lock and uncomment out the code for erasing the
        // cmd from command map in else block --- lzou

        // this fix has been added to support a case where the NE sends
        // duplicate response for the same ctag - aaali

        osi_sem_write_lock lock (cmdMapLock)
        ;
        cmdMapType::iterator p = _cmdMap.find(ctag);
        if (p == _cmdMap.end())
            pCmd = NULL;
        else
        {
            pCmd = (*p).second;
            // if the message has been completely processed
            // release it from the command map so that the a response with
            // the same CTAG does not cause handlEvnt to be callaed again
            if (msg->m_finished)
            {
                ErrorLogger::logVarError(sourceName, "TL1Connection", "_getReceiver", __LINE__, TRACE10, "Message has been completely processed; releasing command/receiver with ctag [%s]", ctag.c_str());
                _cmdMap.erase(p);
            }
            pCmd->handleResponse (msg);
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "_getReceiver", __LINE__, MINOR, "Exception happen on Read Sem Lock ->%s", ex.description(ex.code()));
    }

    ErrorLogger::logVarError(sourceName, "TL1Connection", "_getReceiver", __LINE__, TRACE10, "_getReceiver end with pointer of cmd %d...", pCmd);
    return pCmd;
}

void TL1Connection::releaseReceiver(const string& ctag)
{
    ErrorLogger::logVarError(sourceName, "TL1Connection", "releaseReceiver", __LINE__,
                             TRACE10, "releaseReceiver begin with ctag %s ...", ctag.c_str());
    try
    {
        osi_sem_write_lock lock (cmdMapLock)
        ;
        cmdMapType::iterator p = _cmdMap.find(ctag);
        if (p != _cmdMap.end())
            _cmdMap.erase(p);
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "releaseReceiver", __LINE__, MINOR, "Exception happen on releaseReceiver->%s", ex.description(ex.code()));
    }
    ErrorLogger::logVarError(sourceName, "TL1Connection", "releaseReceiver", __LINE__,
                             TRACE10, "releaseReceiver  end ");
}

void TL1Connection::clearCmdMap ()
{
    //list<TL1EventReceiver*> tmpList;
    //list<TL1EventReceiver*>::iterator pTemList;
    ErrorLogger::logError( sourceName, "TL1Connection", "clearCmdMap", TRACE1, "Method Started");
    bool tosleep = false;
    try
    {
        osi_sem_write_lock lock (cmdMapLock)
        ;
        int n = _cmdMap.size();
        ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMap", __LINE__, TRACE10, "clearCmdMap begin with size of cmdMap %d", n);
        cmdMapType::iterator pCmd;
        for (pCmd = _cmdMap.begin(); pCmd != _cmdMap.end(); pCmd++)
        {
            ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMap", __LINE__,
                                     TRACE10, "clearCmdMap has size of cmdMap %d", _cmdMap.size());
            TL1BaseCmd* p = (TL1BaseCmd*) ((*pCmd).second);
            p->postCmdLock();
            tosleep = true;
        }
        ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMap", __LINE__, TRACE10, "About to clear the map");
        //_cmdMap.clear();
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMap", __LINE__, MINOR, "Exception happen on releaseReceiver->%s", ex.description(ex.code()));
    }
    ErrorLogger::logError( sourceName, "TL1Connection", "clearCmdMap", TRACE1, "Method Finished");

    /*
      // let releaseReceiver in TL1BaseCmd called
         if (tosleep)
         {
          for (int i=0;i<20;i++) // wait 2 second maximum
          {
           try{
           osi_this_thread::milli_sleep(100);
         }catch(...)
         {
          ACE_OS::sleep(1);
         }
      try {
           osi_sem_read_lock lock(cmdMapLock);
           int n = _cmdMap.size();
           ErrorLogger::logVarError(sourceName,"TL1Connection","clearCmdMap",__LINE__,
       TRACE10,"Size of cmdMap %d", n);
           if (n<=0)
            break;
      }catch(osi_thread_toolkit_error& ex)
           {
               string desc = "Exception happened on sem read lock. ";
               ErrorLogger::logVarError(sourceName,"TL1Connection","clearCmdMap",__LINE__,MINOR,"Exception happen on releaseReceiver->%s",ex.description(ex.code()));
           }
          }
         }
    */

}

void TL1Connection::clearCmdMapForNe (const string& neId)
{
    try
    {
        osi_sem_write_lock lock (cmdMapLock)
        ;
        int n = _cmdMap.size();
        ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMapForNe", __LINE__,
                                 TRACE10, "clearCmdMap begin with size of cmdMap %d for NE %s", n,
                                 neId.c_str());
        cmdMapType::iterator pCmd;
        for (pCmd = _cmdMap.begin(); pCmd != _cmdMap.end(); pCmd++)
        {
            ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMapForNe", __LINE__,
                                     TRACE10, "clearCmdMap has size of cmdMap %d", _cmdMap.size());
            TL1BaseCmd* p = (TL1BaseCmd*) ((*pCmd).second);
            BaseCmdInfo* cmdInfo = p->getInfo();
            if (cmdInfo)
            {
                string id = cmdInfo->getNeId();
                if (neId == id)
                    p->postCmdLock();
            }
        }
        ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMapForNe", __LINE__,
                                 TRACE10, "About to clear the map");
        //_cmdMap.clear();
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "clearCmdMapForNe", __LINE__, MINOR, "Exception happen on releaseReceiver->%s", ex.description(ex.code()));
    }
    /* try{
          osi_this_thread::milli_sleep(500);
        }catch(...)
        {
         ACE_OS::sleep(1);
        }     */
}

void TL1Connection::insertReceiver (const string& ctag, TL1EventReceiver* receiver)
{
    ErrorLogger::logVarError(sourceName, "TL1Connection", "insertReceiver", __LINE__,
                             TRACE10, "insertReceiver begin with ctag %s ...", ctag.c_str());
    if (!_acceptNewCmd)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "insertReceiver", __LINE__,
                                 MINOR, "TL1Connection is destructing, don't accept new command. ");
        throw BaseException("TL1Connection is destructing, don't accept new command. ",
                            "TL1Connection", "getLoginFlagFromInfo");
    }
    try
    {
        osi_sem_write_lock lock (cmdMapLock)
        ;
        _cmdMap[ctag] = receiver;
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "insertReceiver", __LINE__, MINOR,
                                 "Exception happen on insertReceiver->%s", ex.description(ex.code()));
    }
    ErrorLogger::logVarError(sourceName, "TL1Connection", "insertReceiver", __LINE__,
                             TRACE10, "insertReceiver  end ");
}

string TL1Connection::sendCommand(const string& preCmd, const string& postCmd,
                                  TL1EventReceiver* receiver, int timeout, const string &ctag)
{
    insertReceiver(ctag, receiver);
    string cmd = preCmd + ctag + postCmd;
    string connName = getName();
    string duplicateMessage ;
    ErrorLogger::maskPassword(cmd, duplicateMessage);
    ErrorLogger::logVarError(sourceName, "TL1Connection", "sendCommand", TRACE1,
                             "Sending the following command with timeout [%d] seconds via connection [%s]:\n\n%s\n", timeout / 1000, connName.c_str(), duplicateMessage.c_str());

    if ( (send_data(cmd.c_str())) == false)
    {
        string message = "Failed to send command [" + cmd + "] on the socket connection to [" + connName + "]; socket connection possibly closed";
        throw BaseException (message);
    }
    return ctag;
}

//------------------------------------------------------------------------------------------
// Pre/Post processing routines should be overridden in FM/CM
//------------------------------------------------------------------------------------------
string TL1Connection::preProcess (const char* rawMessage)
{
    ErrorLogger::logError(sourceName, "TL1Connection", "preProcess", TRACE7, "Method Begins.");
    string outputText = rawMessage;
    // The following is a temporary fix for how a 7100 FP2.0 NE responds to RTRV-SW
    // The LOCATION field looks like \"\development\loads\\" instead of \"\\development\\loads\\\"
    size_t position = outputText.find("SWVER=");
    if (position != string::npos)
    {
        for (size_t pos = position; pos < outputText.length() - 1; pos++)
        {
            if (outputText[pos] == '\\')
                pos++;  // skip next char
            else if (outputText[pos] == '\"')
            {
                if (outputText[pos + 1] == ',' || outputText[pos + 1] == ':')
                {
                    outputText.insert(pos, "\\");
                    pos++;
                    ErrorLogger::logVarError(sourceName, "TL1Connection", "preProcess", TRACE3,
                                             "Inserting \\ in position %d of %s. ", pos - 1, outputText.c_str() );
                }
            }
        }
    }

    ErrorLogger::logError(sourceName, "TL1Connection", "preProcess", TRACE7, "Method Ends.");
    return outputText;
}

void TL1Connection::postProcess (TL1Message *tl1structure)
{
    ErrorLogger::logError(sourceName, "TL1Connection", "postProcess", TRACE7, "No post-processing steps have been defined");
}

//------------------------------------------------------------------------------------------
// Message processing and distribution to sessions
//------------------------------------------------------------------------------------------
bool TL1Connection::incompleteMessage(string& msg, string& retTL1Msg)
{
    if (msg.length() == 0)
        return true;

    retTL1Msg.erase();
    retTL1Msg.reserve(msg.length());

    bool retState = true;
    string serverName = Server_i::instance()->getServerName();

    enum posState {InitPosStat, InComm, OutComm};
    enum ctagState {InitCtagStat, ExepectCtag, FoundStartCtag};
    enum symbolType {NOSYMBOL, SEMCOLON, GREATER, LESSER, BEGINCOMMENTS, ENDCOMMENTS, RESP};
    enum symbolState {NOSYMBOLSTAT, RETCHAR, RETCHAR_LINEFEED, RETCHAR_LINEFEED_M, RETCHAR_LINEFEED_M_SPACE, SLASHSYMBOL, STARTSYMBOL};

    int commentCount = 0;

    StringLexInput sSource(msg);

    char nextChar;
    string ctag;
    posState posStat = InitPosStat;
    ctagState ctagStat = InitCtagStat;
    symbolType symbol = NOSYMBOL;
    symbolState symbolStat = NOSYMBOLSTAT;

    while (1)
    {
        //reset symbol, it should be consumed by posStat switch already
        symbol = NOSYMBOL;
        if (!sSource.getCharacter(nextChar))
        {
            ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE3, "Found no Char, Return ...");
            return retState;
        }

        switch (nextChar)
        {

        case Eos:
            ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE3, "End of String");
            return retState;
        case '\r':
            symbolStat = RETCHAR;
            break;
        case '\n':
            if (symbolStat == RETCHAR)
                symbolStat = RETCHAR_LINEFEED;
            else    // if \r\n\n
                symbolStat = NOSYMBOLSTAT;
            break;
        case ';':
            if (symbolStat == RETCHAR_LINEFEED)
            {
                symbol = SEMCOLON;
                symbolStat = NOSYMBOLSTAT;
            }
            break;
        case '>':
            if (symbolStat == RETCHAR_LINEFEED)
            {
                symbol = GREATER;
                symbolStat = NOSYMBOLSTAT;
            }
            break;
        case '<':
            if (symbolStat == RETCHAR_LINEFEED)
            {
                symbol = LESSER;
                symbolStat = NOSYMBOLSTAT;
            }
            break;
        case '/':
            if (symbolStat == STARTSYMBOL)
                symbol = ENDCOMMENTS;
            else
                symbolStat = SLASHSYMBOL;
            break;
        case '*':
            if (symbolStat == SLASHSYMBOL)
                symbol = BEGINCOMMENTS;
            else
                symbolStat = STARTSYMBOL;
            break;

        case 'M':
            if (symbolStat == RETCHAR_LINEFEED)
                symbolStat = RETCHAR_LINEFEED_M;
            else
                symbolStat = NOSYMBOLSTAT;
            break;
        case ' ':
            if (symbolStat == RETCHAR_LINEFEED_M_SPACE)
            {
                symbol = RESP;
                symbolStat = NOSYMBOLSTAT;
            }
            else if (symbolStat == RETCHAR_LINEFEED_M)
                symbolStat = RETCHAR_LINEFEED_M_SPACE;
            else
                symbolStat = NOSYMBOLSTAT;
            break;

        } // switch
        if ((symbolStat != NOSYMBOLSTAT) &&
            (((symbolStat == RETCHAR) && (nextChar != '\r')) ||
             ((symbolStat == RETCHAR_LINEFEED) && (nextChar != '\n')) ||
             ((symbolStat == RETCHAR_LINEFEED_M) && (nextChar != 'M')) ||
             ((symbolStat == STARTSYMBOL) && (nextChar != '*')) ||
             ((symbolStat == SLASHSYMBOL) && (nextChar != '/')) ||
             ((symbolStat == RETCHAR_LINEFEED_M_SPACE) && (nextChar != ' '))))
            symbolStat = NOSYMBOLSTAT;
        retTL1Msg += nextChar;

        // if no symbol found, or it is not ctag state, not necessary go down
        if ((symbol == NOSYMBOL) && (ctagStat != ExepectCtag) && (ctagStat != FoundStartCtag))
        {
            continue;
        }

        switch (posStat)
        {
        case InitPosStat:
            if ((symbol == SEMCOLON) || (symbol == GREATER) || (symbol == LESSER))
            {
                ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE7, "Found a complete TL1 Msg block from a chunk buffer %s", msg.c_str());
                sSource.getCurrentStr(msg);
                return false;
            }
            else if (symbol == RESP)
            {
                if (_incompleteResMap.empty())
                    posStat = OutComm;
                else
                {
                    ctagStat = ExepectCtag;
                    ctag.erase();
                }
            }
            else if (ctagStat == ExepectCtag)
            {
                // ctag should be just alpha numeric char
                // it is cleaner to use c function isalnum
                if ((nextChar <= 'z') && (nextChar >= '0'))
                {
                    ctagStat = FoundStartCtag;
                    ctag += nextChar;
                }
            }
            else if (ctagStat == FoundStartCtag)
            {
                if (nextChar != ' ')
                {
                    ctag += nextChar;
                }
                else  // found CTAG
                {

                    ctagStat = InitCtagStat;

                    // cannot remove the entry yet, because may not find the end of message now.
                    commentCount = _getCountFromMap(ctag);
                    if (commentCount <= 0)
                    {
                        posStat = OutComm;
                        commentCount = 0;
                    }
                    else  // CTAG in map
                    {
                        string complCode;
                        while (1)
                        {

                            if (!sSource.getCharacter(nextChar))
                            {
                                ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE1, "Found no Char, Return ...");
                                return retState;
                            }
                            retTL1Msg += nextChar;
                            if (nextChar == ' ')
                            {
                                if ((complCode != "COMPLD") && (complCode != "DENY"))
                                {
                                    ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__,
                                                             MAJOR, "Completion Code %s after CTAG %s is invalid ", complCode.c_str(), ctag.c_str());
                                }
                                break;
                            }
                            // ctag should be just alpha char
                            // it is cleaner to use c function isalpha
                            if (nextChar >= '0' && nextChar <= 'z')  // in case \r\n
                                complCode += nextChar;
                        }

                        // insert artificially /* after COMPLD Code
                        _appendStrings(retTL1Msg, "\r\n/*   ", commentCount); // add \r\n ^^^ (/* ...
                        posStat = InComm;
                    }
                }
            }  // End of Found the Ctag
            else
                continue;
            break;
        case OutComm:
            if ((symbol == SEMCOLON) || (symbol == GREATER) || (symbol == LESSER))
            {
                ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE7, "Found a complete TL1 Msg block from a chunk buffer %s", msg.c_str());
                sSource.getCurrentStr(msg);
                if (!ctag.empty())
                    _popCountFromMap(ctag);
                return false;
            }
            else if (symbol == BEGINCOMMENTS)
            {
                commentCount = 1;
                posStat = InComm;
            }
            break;
        case InComm:
            if (symbol == BEGINCOMMENTS)
                commentCount++;
            else if (symbol == ENDCOMMENTS)
                commentCount--;
            else if (symbol == GREATER)
            {
                // Insert */ based on comment count before > and insert ctag/count into map
                ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE7, "Found a complete TL1 Msg block from a chunk buffer %s", msg.c_str());
                retTL1Msg = retTL1Msg.substr(0, retTL1Msg.length() - 3);
                _appendStrings (retTL1Msg, "*/ ", commentCount);
                retTL1Msg.append("\r\n>");
                sSource.getCurrentStr(msg);
                int nposCtag;
                if (ctag.empty())
                {
                    ctag = _findCtag(retTL1Msg, 0, nposCtag);
                }
                ErrorLogger::logVarError(sourceName, "TL1Connection", "incompleteMessage", __LINE__, TRACE7, "Found ctag %s", ctag.c_str());
                if (!ctag.empty())
                    _popCountFromMap(ctag);
                _incompleteResMap.insert(pair<string, int>(ctag, commentCount));
                return false;
            }
            if (commentCount == 0)
                posStat = OutComm;
            break;
        } // switch of posStat
    }    // while loop

    ErrorLogger::logError( sourceName, "TL1Connection", "incompleteMessage", TRACE7, "Didn't find Message Terminator");
    return true;
}

void TL1Connection::processMessage(const char* message)
{
    //string messageString (message);
    // _maxMessageSizeAllowed will only be defined in the Proxy and will default to 0 for FM and CM
    const string &maxTL1MessageSizeString = Server_i::instance()->getDefaultConfigValue ("MaxTL1MessageSize", "1048576");
    unsigned long maxTL1MessageSize = atol (maxTL1MessageSizeString.c_str());

    string validMessageCheck = ConfigNames::False;
    if (strlen(message) > maxTL1MessageSize)
    {
        string error("THE FOLLOWING RAW MESSAGE HAS EXCEEDED THE MAXIMUM LIMIT [" + maxTL1MessageSizeString + "] ON THE SIZE OF A SINGLE TL1 MESSAGE \n\n");
        error.append(message);
        error.append("\n\n");
        throw BaseException(error, "TL1Connection", "processMessage");
    }
    validMessageCheck = Server_i::instance()->getDefaultConfigValue ("ValidMessageCheck", ConfigNames::True);


    string connName = getName();

    string duplicateMessage;
    ErrorLogger::maskPassword(message, duplicateMessage);
    ErrorLogger::logVarError(sourceName, "TL1Connection", "processMessage", TRACE1, "Received the following message via [%s]:\n%s\n ", connName.c_str(), duplicateMessage.c_str());

    const string &protocol = Server_i::instance()->getDefaultConfigValue(ConfigNames::ProtocolKey, EMSDEF::TL1ProtocolKey);

    if (!_expectedMsg.empty())
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "processMessage", __LINE__, TRACE1,
                                 "Connection Negotiation is processing. Expects the Msg %s", _expectedMsg.c_str());
        string msg(message);
        size_t iTer = msg.find_first_of(';');
        msg = msg.substr(0, iTer);
        if (msg == _expectedMsg)
        {
            _negotiateLock.post();
        }
        return ;

    }

    const string& preProcessedMessage = preProcess(message);


    TL1ConnectionManager *manager = TL1ConnectionManager::instance();

    // note that all of the following code will be executed in the FM/CM
    // Set up the input source
    StringLexInput sSource(preProcessedMessage);

    // pass the source to the lexical analyzer
    LexerTL1::Lexer* lexer = LexerTL1::createLexer(&sSource);

    // Set up the Parser
    ParseTL1::Parser* parser = ParseTL1::createParser(lexer);
    TL1Message* msg = NULL;

    bool foundAtLeastOneValid = false;

    while (1)
    {
        msg = parser->getTL1Message();

        if (msg->m_type == TL1Message::End)
        {
            ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE11, "Found End of message....");
            delete msg;
            break;
        }

        if (msg->m_type == TL1Message::Ignore)
        {
            ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE11, "Found Ignore message");
            delete msg;
            msg = NULL;
            continue;
        }

        if ((msg->m_type == TL1Message::None) || (msg->m_type == TL1Message::ParseError)
            || (msg->m_type == TL1Message::Err) || (msg->m_type == TL1Message::Rejected)
            || (msg->m_type == TL1Message::Timeout))
        {
            ErrorLogger::logVarError( sourceName, "TL1Connection", "processMessage", MAJOR, "Invalid message type [%d] detected in \n\n%s\n", msg->m_type, message);
            continue;
        }


        // we will only get here if message is not ended or ignored
        foundAtLeastOneValid = true;

        // this will be executed in Proxy
        if (manager->getNumberOfAssociatedSessions(this))
        {
            manager->sendDataToSession (this, preProcessedMessage.c_str(), msg);
        }


        else  // FM/CM/Default proxy will fall through to this
        {
            if (msg->m_type == TL1Message::Autonomous)
            {
                ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE4, "Found Autonomous Message...  Post-processing the message.");

                // FM/CM can provide custom postprocess methods to work around product-specific problems/deviations in the TL1 response/event
                postProcess (msg);

                int type = (int) msg->m_type;
                TL1EventReceiver *receiver = getListener (msg->m_sid);
                if (!receiver)
                {
                    // get it by IP+PORT
                    ErrorLogger::logVarError( sourceName, "TL1Connection", "processMessage", TRACE11, "Found the Event Receiver by Key %s", getName().c_str());
                    receiver = getListener (getName());
                }
                if (receiver)
                    receiver->handleEvent (msg, preProcessedMessage);
                else
                {
                    // the default Proxy does not handle autonomous events so we don't care to log this
                    if (protocol != "Software")
                        ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", MAJOR, "Could not find a receiver!");
                }
            }
            else if (msg->m_type == TL1Message::Response )
            {
                ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE1, "Found Response Message...  Post-processing the message.");

                // FM/CM can provide custom postprocess methods to work around product-specific problems/deviations in the TL1 response/event
                postProcess (msg);

                int type = (int) msg->m_type;
                const string ctag = msg->m_tag;
                // getrceiver will do the handleevent
                TL1EventReceiver* receiver = _getReceiver(msg, preProcessedMessage.c_str());
                if (receiver)
                    ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE11, "Found Event Receiver in this Server");
                // we don't reach the above code for the proxy as it the message was passed to the session
                else
                    ErrorLogger::logVarError( sourceName, "TL1Connection", "processMessage", MAJOR, "Ignoring response message as no command registered with CTAG [%s] was found; we possibly got a duplicate response for a command", ctag.c_str());
            }
            else if (msg->m_type == TL1Message::Acknowledgement)
            {
                // If ACK is "RL", we need to throw exception to let user retry --- Li zou
                ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE1, "Found Acknowledgement Message ...");
                int type = (int) msg->m_type;
                if (msg->m_code == "IP")
                {
                    if (_waitOnIP == "true")
                    {
                        TL1EventReceiver* receiver = _getReceiver(msg, preProcessedMessage.c_str());
                        if (receiver)
                            ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE11, "Found Event Receiver for IP message in Server");
                    }
                    else
                        ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE1, "Ignoring IP message as WaitOnIP is false");
                }
            }
            else
                ErrorLogger::logError( sourceName, "TL1Connection", "processMessage", TRACE1, "Found blank lines or unknown type of message. Ignoring it.");
        }
        if (msg)
            delete msg;
    }     // end of while loop
    if (lexer)
    {
        delete lexer;
        lexer = NULL;
    }
    if (parser)
    {
        delete parser;
        parser = NULL;
    }
    if (validMessageCheck == ConfigNames::True)
    {
        if (foundAtLeastOneValid == false)
        {
            string error("NO VALID TL1 MESSAGE BLOCKS IN THE FOLLOWING RAW MESSAGE \n\n");
            error.append(message);
            error.append("\n\n");
            throw BaseException(error, "TL1Connection", "processMessage");
        }
    }
}

//------------------------------------------------------------------------------------------
// Event receiver registration routines
//------------------------------------------------------------------------------------------
void TL1Connection::registerListener(const string &keyName, EMS_FW::TL1EventReceiver* receiver)
{
    _listeners.insert (pair<string, TL1EventReceiver*>(keyName, receiver));
}

bool TL1Connection::unregisterListener(const string &keyName)
{
    map <string, TL1EventReceiver*>::iterator i = _listeners.find (keyName);
    if (i != _listeners.end())
    {
        _listeners.erase(i);
        return true;
    }
    return false;
}

TL1EventReceiver* TL1Connection::getListener (const string &keyName)
{
    map <string, TL1EventReceiver*>::iterator i = _listeners.find (keyName);

    TL1EventReceiver *receiver = NULL;
    if (i != _listeners.end())
        receiver = (*i).second;
    return receiver;
}

//------------------------------------------------------------------------------------------
// Recovery handler registration routines
//------------------------------------------------------------------------------------------
void TL1Connection::registerHandler(EMS_FW::TL1LocRecoverHandler* handler)
{
    _locHandler = handler;
}

void TL1Connection::unregisterHandler()
{
    _locHandler = NULL;
}

void TL1Connection::processThread()
{}

void TL1Connection::connectionLoss()
{
    string connName = getName();
    ErrorLogger::logVarError( sourceName, "TL1Connection", "connectionLoss", TRACE1, "Connection to [%s] lost; aborting all waiting commands sent via this connection", connName.c_str());
    _isActive = false;

    const string &protocol = Server_i::instance()->getDefaultConfigValue(ConfigNames::ProtocolKey, EMSDEF::TL1ProtocolKey);
    if (protocol == "Software")
    {
        clearCmdMap();
        return ;
    }

    TL1ConnectionManager *manager = TL1ConnectionManager::instance();
    SocketConnection::connectionLoss();
    clearCmdMap();
    //_responseBuffers.clear();
    manager->handleConnectionLoss (this);
}

void TL1Connection::addNeInfo (const string &tid, const string &neId, const string &username, const string &password)
{

    neInfo *info = new neInfo ();
    info->neId = neId;
    info->username = username;
    info->password = password;
    // we set this to false so that connection response does not try to login for an NE which failed during addnetworkelement
    info->tryLoggingIn = false;
    ;
    info->retrieveHeaderRetry = 0;
    info->lastATAG = "-1";

    try
    {
        osi_sem_write_lock lock (rwSemaphore)
        ;
        _neList.insert (pair<string, neInfo*>(tid, info));
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "addNeInfo", __LINE__, MINOR, "Exception happen on Write Sem Lock ->%s", ex.description(ex.code()));
    }

}


void TL1Connection::removeNeInfo (const string &tid)
{

    try
    {
        osi_sem_write_lock lock (rwSemaphore)
        ;
        map <string, neInfo*>::iterator i = _neList.find (tid);

        if (i != _neList.end())
        {
            neInfo *info = (*i).second;
            _neList.erase(i);
            if (info != NULL)
            {
                delete info;
                info = NULL;
            }
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "addNeInfo", __LINE__, MINOR, "Exception happen on Write Sem Lock ->%s", ex.description(ex.code()));
    }
}


neInfo *TL1Connection::getNeInfo (const string &tid)
{
    neInfo* info = NULL;
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        info = _getNeInfo(tid);
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getNeInfo", __LINE__, MINOR, "Exception happen on Read Sem Lock ->%s", ex.description(ex.code()));
    }
    return info;
}


string TL1Connection::getUidFromInfo(const string& tid)
{
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            return info->username;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getUidFromInfo", MINOR, "Exception caught on getUidFromInfo->%s", ex.description(ex.code()));
    }
    return "";
}

string TL1Connection::getNeIdFromInfo(const string& tid)
{
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            return info->neId;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getNeIdFromInfo", __LINE__, MINOR, "Exception caught on getNeIdFromInfo->%s", ex.description(ex.code()));
    }
    return "";
}

string TL1Connection::getPasswdFromInfo(const string& tid)
{
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            return info->password;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getPasswdFromInfo", __LINE__, MINOR, "Exception caught on getPasswdFromInfo->%s", ex.description(ex.code()));
    }
    return "";
}

string TL1Connection::getATAGFromInfo(const string& tid)
{
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            return info->lastATAG;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getATAGFromInfo", __LINE__, MINOR, "Exception caught on getATAGFromInfo->%s", ex.description(ex.code()));
    }
    return "";
}


bool TL1Connection::getLoginFlagFromInfo(const string& tid)
{
    bool flag;
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            flag = info->tryLoggingIn;
        }
        else
        {
            string desc = "The NE " + tid + " was removed from NeInfo.";
            ErrorLogger::logVarError(sourceName, "TL1Connection", "getLoginFlagFromInfo", __LINE__, MINOR, desc.c_str());
            throw BaseException(desc, "TL1Connection", "getLoginFlagFromInfo");
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getLoginFlagFromInfo", __LINE__, MINOR, "Exception happen on logout->%s", ex.description(ex.code()));
        throw BaseException(desc, "TL1Connection", "getLoginFlagFromInfo");
    }
    return flag;
}

int TL1Connection::getRtrvHdrRetryFromInfo(const string& tid)
{

    int nRetry = 0;
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            nRetry = info->retrieveHeaderRetry;
        }
        else
        {
            string desc = "The NE " + tid + " was removed from NeInfo.";
            ErrorLogger::logVarError(sourceName, "TL1Connection", "getRtrvHdrRetryFromInfo", __LINE__, MINOR, desc.c_str());
            throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getRtrvHdrRetryFromInfo", __LINE__, MINOR, "Exception happen on getRtrvHdrRetryFromInfo->%s", ex.description(ex.code()));
        throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
    }
    return nRetry;
}


bool TL1Connection::updatePasswdFromInfo(const string& tid, const string& newPasswd)
{

    try
    {
        osi_sem_write_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            info->password = newPasswd;
        }
        else
        {
            string desc = "The NE " + tid + " was removed from NeInfo.";
            ErrorLogger::logVarError(sourceName, "TL1Connection", "updatePasswdFromInfo", __LINE__, MINOR, desc.c_str());
            //throw BaseException(desc, "TL1Connection", "updatePasswdFromInfo");
            return false;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "updatePasswdFromInfo", __LINE__, MINOR, "Exception happen on getRtrvHdrRetryFromInfo->%s", ex.description(ex.code()));
        //throw BaseException(desc, "TL1Connection", "updatePasswdFromInfo");
        return false;
    }
    return true;
}

bool TL1Connection::updateATAGFromInfo(const string& tid, const string& newATAG)
{

    try
    {
        osi_sem_write_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            info->lastATAG = newATAG;
        }
        else
        {
            string desc = "The NE " + tid + " was removed from NeInfo.";
            ErrorLogger::logVarError(sourceName, "TL1Connection", "updateATAGFromInfo", __LINE__, MINOR, desc.c_str());
            //throw BaseException(desc, "TL1Connection", "updateATAGFromInfo");
            return false;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "updateATAGFromInfo", __LINE__, MINOR, "Exception happen on getRtrvHdrRetryFromInfo->%s", ex.description(ex.code()));
        //throw BaseException(desc, "TL1Connection", "updateATAGFromInfo");
        return false;
    }
    return true;
}

bool TL1Connection::updateLoginFlagFromInfo(const string& tid, bool newFlag)
{

    try
    {
        osi_sem_write_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            info->tryLoggingIn = newFlag;
        }
        else
        {
            string desc = "The NE " + tid + " was removed from NeInfo.";
            ErrorLogger::logVarError(sourceName, "TL1Connection", "updateLoginFlagFromInfo", __LINE__, MINOR, desc.c_str());
            //throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
            return false;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "updateLoginFlagFromInfo", __LINE__, MINOR, "Exception happen on getRtrvHdrRetryFromInfo->%s", ex.description(ex.code()));
        //throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
        return false;
    }
    return true;
}

bool TL1Connection::updateRtrvHdrRetryFromInfo(const string& tid, int newRetry)
{

    try
    {
        osi_sem_write_lock lock (rwSemaphore)
        ;
        neInfo* info = _getNeInfo(tid);
        if (info)
        {
            info->retrieveHeaderRetry = newRetry;
        }
        else
        {
            string desc = "The NE " + tid + " was removed from NeInfo.";
            ErrorLogger::logVarError(sourceName, "TL1Connection", "updateRtrvHdrRetryFromInfo", __LINE__, MINOR, desc.c_str());
            //throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
            return false;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "updateRtrvHdrRetryFromInfo", __LINE__, MINOR, "Exception happen on getRtrvHdrRetryFromInfo->%s", ex.description(ex.code()));
        //throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
        return false;
    }
    return true;
}

neInfo* TL1Connection::_getNeInfo (const string &tid)
{
    map <string, neInfo*>::iterator i = _neList.find (tid);
    neInfo *info = NULL;
    if (i != _neList.end())
        info = (*i).second;
    return info;
}


// we don't need this constructor
TL1Connection::TL1Connection (const string &hostName, unsigned long portNumber)
{
    setClassName(className);
    char buffer[10];
    string name = hostName + "." + itoa(portNumber, buffer, 10);
    setName(name);
    //_neId = neId;
    _locHandler = NULL;
}

bool TL1Connection::connectionNegotiate (const string& negotiateMsg, const string& expectMsg, int timeout)
{
    ErrorLogger::logError( sourceName, "TL1Connection", "connectionNegotiate",
                           TRACE4, "Method Begins ... ");
    _expectedMsg = expectMsg;
    //_negotiateLock.reset();
    if (timeout <= 0)
        timeout = 5;
    // add the terminator so that know it's a complete message
    string sendMsg = negotiateMsg + ";";
    send_data(sendMsg.c_str());
    ErrorLogger::logVarError( sourceName, "TL1Connection", "connectionNegotiate",
                              TRACE1, "Sent a message [%s] and waiting for message [%s] with a timeout of [%d] seconds", sendMsg.c_str(), expectMsg.c_str(), timeout);
    if (_negotiateLock.wait(timeout*1000))
    {
        ErrorLogger::logVarError( sourceName, "TL1Connection", "connectionNegotiate",
                                  MAJOR, "Negoatiatiion Time Out. Failed with sending msg %s and expecting msg %s",
                                  sendMsg.c_str(), expectMsg.c_str());
        _expectedMsg = "";
        return false;
    }

    ErrorLogger::logError( sourceName, "TL1Connection", "connectionNegotiate",
                           TRACE4, "Method Ends without timeout ");
    _expectedMsg = "";
    return true;
}

bool TL1Connection::_receiverExists (const string &ctag)
{
    try
    {
        osi_sem_read_lock lock (cmdMapLock)
        ;
        cmdMapType::iterator p = _cmdMap.find(ctag);
        // did not find a command associated with the CTAG
        if (p == _cmdMap.end())
            return false;
        else
            return true;
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Connection", "_getReceiver", __LINE__, MINOR, "Exception happen on Read Sem Lock ->%s", ex.description(ex.code()));
    }
    // return true to be on the safe side so that a new CTAG is geneated
    return true;
}


string TL1Connection::_findCtag (const string& msg, int startingPos, int& nposCtag)
{
    int npos = msg.find("\r\nM  ", startingPos);
    if (npos != string::npos)
    {
        nposCtag = msg.find_first_not_of(" ", npos + 4);
        int endPos = msg.find_first_of(" ", nposCtag + 1);
        return msg.substr(nposCtag, endPos - nposCtag);
    }
    else
        return "";

}

int TL1Connection::_popCountFromMap (const string& key)
{
    map<string, int>::iterator iter;
    iter = _incompleteResMap.find(key);

    if (iter != _incompleteResMap.end())
    {
        int ret = iter->second;
        _incompleteResMap.erase(iter);
        return ret;
    }
    return 0;

}

int TL1Connection::_getCountFromMap (const string& key)
{
    map<string, int>::iterator iter;
    iter = _incompleteResMap.find(key);

    if (iter != _incompleteResMap.end())
    {
        int ret = iter->second;
        return ret;
    }
    return 0;

}

void TL1Connection::_appendStrings (string& source, const string& appendStr, int& count)
{
    if (count > 0)
    {
        for (int k = 0; k < count;k++)
        {
            source.append(appendStr);
        }
    }
    return ;
}
