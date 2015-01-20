/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: TL1 Socket Acceptor
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1SocketAcceptor.h  $
* Version: $Revision:   1.5  $
* Description:
*
*
* Revision History:

*
* $Log:  $
**********************************************************************************************/

#include "TL1ConnectionManager.h"
#include "TL1ConnectionResponse.h"
#include "TL1SocketAcceptor.h"
#include "SocketHandler.h"
#include "sysErrLog.h"
#include "ConfigNames.h"
#include "Server_i.h"
#include "TL1Lexer.h"
#include "TL1Parser.h"
#include "tutil.h"
#include "NodeDefines.h"
//#include "TL1Utility.h"
#include <ospace/helper.h>


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;
//using namespace TL1_MA;


const string TL1SocketAcceptor::className = "TL1SocketAcceptor";


TL1SocketAcceptor* TL1SocketAcceptor::getInstance(const string& acceptorId, const string& name)
{
    /* TL1SocketAcceptor* con;
     if(name.size() != 0)
      con = (TL1SocketAcceptor*) Server_i::instance()->getComponent(name,acceptorId);
     else
     {
      con = (TL1SocketAcceptor*) Server_i::instance()->getComponent(className,acceptorId);
     }
     if(con == NULL)
     {
      string desc = "Unable to get TL1SocketAcceptor for acceptorId->";
      desc += acceptorId;
      desc += " Name->";
      desc += name;
      throw BaseException(desc,"TL1","getInstance");
     }
     return con;
       */
    return NULL;
}


TL1SocketAcceptor::TL1SocketAcceptor(const string& acceptorId, const string& expectMsg,
                                     const string& ackMsg, int negWaitPeriod, unsigned portNumber, const string& hostName): SocketAcceptor(portNumber, hostName)
{
    setClassName(className);
    setName(acceptorId);
    _acceptorId = acceptorId;
    _expectedMsg = expectMsg + ";";
    _ackMsg = ackMsg + ";";
    _negWaitPeriod = negWaitPeriod;
    _neIpList.clear();
}



TL1SocketAcceptor::~TL1SocketAcceptor()
{
    _neIpList.clear();
    finalize();
}


bool TL1SocketAcceptor::initialize()
{
    // need to initialize the connection to the device

    ErrorLogger::logVarError(sourceName, "TL1SocketAcceptor", "initialize", __LINE__, TRACE4, "Host is ->%s", _hostname.c_str());
    if (!SocketAcceptor::initialize())
        return false; // fall to initialize the connection
    // ACE_OS::sleep(3); // wait the connection to be established before send login
    // need to send the command to the device
    return true;
}


void TL1SocketAcceptor::processMessage(const char* message)
{
    // here we need to process the message and notify
    string msg1 = message;

    string duplicateMessage ;
    ErrorLogger::maskPassword(msg1, duplicateMessage);
    ErrorLogger::logVarError(sourceName, "TL1SocketAcceptor", "processMessage", __LINE__, TRACE1, "Got the message length = %d ->%s", duplicateMessage.length(), duplicateMessage.c_str());

    // Ideally we don't want to parse the coming request

    const string tid = getTidFromTL1Cmd(msg1);
    if (!tid.empty())
    {
        const string ipKey = getIpbyTid(tid);
        // Currently, we don't use the eventReceiver to handle command
        // since forwarding message is simple.
        try
        {
            //TL1EventReceiver* eventReceiver=getListener(tid);
            TL1Session *session = TL1ConnectionManager::instance()->getSessionByTID (tid);
            if (session)
            {
                ErrorLogger::logVarError(sourceName, "TL1SocketAcceptor", "processMessage", __LINE__, TRACE1, "Message is intended for the NE with TID %s and is being forwarded to it", tid.c_str());
                //                session->send_data (message);
            }
            else
            {
                ErrorLogger::logVarError(sourceName, "TL1SocketAcceptor", "processMessage", __LINE__, TRACE1, "Unrecognized TID %s in message", tid.c_str());
            }

            /*   try
                  {
                   TL1Session* pCon = (TL1Session* )TL1Connection::getInstance("", ipKey);
                     if (pCon)
                      pCon->send_data(message);
                     else
                  ErrorLogger::logVarError( sourceName, "ProxyEventReceiver", "handleCommand", CRITICAL, "Couldn't get the connection instance by key %s.", ipKey.c_str());*/

        }
        catch (...)
        {
            ErrorLogger::logError( sourceName, "ProxyEventReceiver", "handleCommand", TRACE1, "No Connector Available");
        }

    }

}


TL1EventReceiver* TL1SocketAcceptor::getListener (const string &keyName)
{
    map <string, TL1EventReceiver*>::iterator i = _listeners.find (keyName);

    TL1EventReceiver *receiver = NULL;
    if (i != _listeners.end())
        receiver = (*i).second;
    return receiver;
}


void TL1SocketAcceptor::registerListener(const string &keyName, EMS_FW::TL1EventReceiver* receiver)
{
    _listeners.insert (pair<string, TL1EventReceiver*>(keyName, receiver));
}

bool TL1SocketAcceptor::unregisterListener(const string &keyName)
{
    map <string, TL1EventReceiver*>::iterator i = _listeners.find (keyName);
    if (i != _listeners.end())
    {
        _listeners.erase(i);
        return true;
    }
    return false;
}

void TL1SocketAcceptor::processThread()
{
    /* ErrorLogger::logError( sourceName, "TL1SocketAcceptor", "processThread", TRACE4, "Method started");
     
     ErrorLogger::logError( sourceName, "TL1SocketAcceptor", "processThread", TRACE4, "Method Ends"); */
}

void TL1SocketAcceptor::connectionLoss()
{

    ErrorLogger::logError( sourceName, "TL1SocketAcceptor", "connectionLoss", TRACE1, "Method started");
    SocketAcceptor::connectionLoss();
    ErrorLogger::logError( sourceName, "TL1SocketAcceptor", "connectionLoss", TRACE1, "Method finished");
}


bool TL1SocketAcceptor::incompleteMessage(string& msg, string& retTL1Msg)
{
    if (msg.length() == 0)
        return true;
    //char c = msg[msg.length() - 1];
    //if(c == ';') // this is the end of the message
    //{
    // iPos = msg.length()-1;
    // return false;
    //}
    // OTS response has a few other chars after ';' --- Added by lzou
    int iPos = msg.find_first_of(";");
    if (iPos != osi_npos)
    {
        int len = msg.length();
        retTL1Msg = msg.substr(0, iPos + 1);
        msg = msg.substr(iPos + 1, len - iPos);
        ErrorLogger::logError( sourceName, "TL1SocketAcceptor", "incompleteMessage", TRACE7, "Found Message Terminator");
        return false;
    }
    ErrorLogger::logError( sourceName, "TL1SocketAcceptor", "incompleteMessage", TRACE7, "Didn't find Message Terminator");
    return true;
}

void TL1SocketAcceptor::addNeIpList(const string& tid, const string& keyName)
{
    _neIpList.insert( pair<string, string>(tid, keyName));
}

bool TL1SocketAcceptor::eraseNeIpList(const string& tid)
{
    map <string, string>::iterator i = _neIpList.find (tid);
    if (i != _neIpList.end())
    {
        _neIpList.erase(i);
        return true;
    }
    return false;

}

string TL1SocketAcceptor::getIpbyTid(const string& tid)
{
    map <string, string>::iterator i = _neIpList.find (tid);
    string ip = "";
    if (i != _neIpList.end())
    {
        ip = (*i).second;
    }
    return ip;
}

string TL1SocketAcceptor::getTidFromTL1Cmd(const string& cmd)
{
    osi_tokenizer tokenizer( ":", true, "\\", ";", "", false );
    vector <string> tokens = tokenizer.tokenize( cmd );
    return tokens[1];
}

/*bool TL1SocketAcceptor::_isExpectedMsg (const char* message)
{
 string comMsg (message);
 if (comMsg == _expectedMsg)
  return true;
 return false;
}*/
