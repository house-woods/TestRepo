/***********************************************************************************************
* COPYRIGHT 2002 - 2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: TL1MsgConverter
* Type: C++
* Originator: rugale
* File:  TL1MsgConverter.cpp
* Version:
* Description:
* Convert the TL1 Message into name-value pairs.
*
* Revision History:
* Rev 1.8  09.27/04  aaali
* Convert TL1MsgConverter to a singleton and remove dependency on the NE ID
*
*   Rev 1.7 06/18/2003 lzou
* set TimeStamp for CmdResult
*   Rev 1.6 05/13/2003 lzou
* Add _processStateBlk to handle state block seperately
*   Rev 1.5 05/09/2003 lzou
* Modification to handle PRTL and empty token after ',' in position-defined block
*   Rev 1.4 04/28/3003 lzou
* change the BaseCmdInfo into TL1CmdInfo and fix the empty common block scenario
*   Rev 1.3     03/14/2003      lzou
*       add the _stripExtraCharsForStr function and modify the _processSpeBlk
*       to accomodate the difference for OTS
*   Rev 1.2     03/03/2003      lzou
*       add the _processSpecBlk and _processPDAttrs functions
* Rev 1.1  01/09/2003  lzou
*  Add the return CmdResult, construct Aid List and obtain MOI/MOC from DecisionMaker
*  based on aid list etc.
* Rev 1.0  init version  rugale
* $Log:  $
**********************************************************************************************/

#include <ospace/helper.h>
#include <ospace/time.h>
#include "SocketHandler.h"
#include "TL1CmdInfo.h"
#include "AgentCmdInfo.h"
#include "MoAttributePo.h"
#include "EmsTypes.h"
#include "BaseException.h"
#include "TL1MsgConverter.h"
#include "TL1Message.h"
#include "ComManager.h"
#include "sysErrLog.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include "tutil.h"
#include "TL1Defines.h"
#include "EmsUnixDefines.h"
#include "AbstractProductDecisionMaker.h"
#include "NetworkElement.h"

using namespace ELogger;
using namespace EMS_FW;

extern const char* sourceName;
static const char* objectName = "TL1MsgConverter";

TL1MsgConverter* TL1MsgConverter::_instance = NULL;

TL1MsgConverter::TL1MsgConverter()
{
    _initialized = false;
}

TL1MsgConverter* TL1MsgConverter::instance ()
{
    if (_instance == NULL)
    {
        _instance = new TL1MsgConverter();
    }
    return _instance;
}


void TL1MsgConverter::initialize(const string &resposeFilePath)
{
    AutoUnlock initLock (_initializationMutex);
    const char* methodName = "initialize";
    if (_initialized)
    {
        ErrorLogger::logError( sourceName, objectName, methodName, TRACE1, "Already initialized the syntax map" );
    }
    else
    {
        // read the grammar (from, usually, TL1Response.txt) and cache it
        ErrorLogger::logVarError( sourceName, objectName, methodName, TRACE1, "Initializing the TL1 syntax map from file [%s]", resposeFilePath.c_str());
        osi_tokenizer tokenizer( ":", true, "\\\n", "\0", "", false );
        osi_tokenizer cmdExtractor( " ", false, "", "\n", "", false );
        ifstream is;
        is.open(resposeFilePath.c_str(), ios::in | ios::nocreate);
        if (is.fail())
        {
            string message = string ("Could not initialize the TL1 syntax map as the TL1 response file ") + resposeFilePath + " was not found";
            ErrorLogger::logError( sourceName, objectName, methodName, MAJOR, message.c_str());
            throw BaseException(message, objectName, methodName);
        }
        vector<string>* ptokens;
        vector< string > cmdTokens;
        char buffer[256];
        while (!is.eof())
        {
            is.getline(buffer, sizeof(buffer));
            cmdTokens = cmdExtractor.tokenize(buffer);
            if (cmdTokens.size() < 2)
                continue; //we have hit the end of file.
            ptokens = new vector<string> (tokenizer.tokenize(cmdTokens[1]));
            _grammar[cmdTokens[0]] = ptokens;
        }
        _initialized = true;
        ErrorLogger::logVarError( sourceName, objectName, methodName, TRACE1, "Initialized the TL1 syntax map from file [%s]", resposeFilePath.c_str());
    }
}

void TL1MsgConverter::finalize ()
{
    delete _instance;
    _instance = NULL;
}

void TL1MsgConverter::convertMessage(list<EMS_FW::BaseCmdInfo*>& infoList, TL1Message* event, CmdResult& result, const string &cmdname, const string &neId)
{
    const char* methodName = "convertMessage";
    ErrorLogger::logError( sourceName, objectName, methodName, TRACE11, "Entering..." );
    AbstractProductDecisionMaker* pdMaker = AbstractProductDecisionMaker::getProductDecisionMaker();
    // set result information
    const string& comCode = event->m_completeCode;
    result.setResultInfo (comCode);
    if ((comCode == TL1DEF::RES_COMPLETECODE_COMP) || (comCode == TL1DEF::RES_COMPLETECODE_PRTL))
        result.setStatus(true);
    else
        result.setStatus(false);
    // if the command is in map get the vector of names
    vector<string>* names = 0;
    size_t pos = cmdname.find_first_of(':');
    string cmdHeader = cmdname.substr(0, pos);
    map <string, vector<string>*>::iterator iter = _grammar.find(cmdHeader);
    if (iter != _grammar.end())
    {
        ErrorLogger::logVarError( sourceName, objectName, methodName, TRACE11, "Command found in the grammar file.  cmd: %s", cmdname.c_str() );
        names = (vector<string>*)((*iter).second);
    }
    else
    {
        ErrorLogger::logVarError( sourceName, objectName, methodName, TRACE3, "No syntax specified for command %s, will try default syntax", cmdname.c_str());
    }

    struct tm t;
    t.tm_sec = event->m_dateTime.m_sec;
    t.tm_min = event->m_dateTime.m_min;
    t.tm_hour = event->m_dateTime.m_hour;
    t.tm_mday = event->m_dateTime.m_day;
    t.tm_mon = event->m_dateTime.m_mon - 1;
    t.tm_year = event->m_dateTime.m_year + 100;
    string time, date;
    try
    {
        osi_date evtDate(event->m_dateTime.m_mon, event->m_dateTime.m_day, event->m_dateTime.m_year + 2000);
        osi_time evtTime(event->m_dateTime.m_hour, event->m_dateTime.m_min, event->m_dateTime.m_sec);
        time = evtTime.to_string("%H-%M-%S");
        date = evtDate.to_string("%y-%m-%d");
    }
    catch (osi_time_toolkit_error& tterr)
    {
        ErrorLogger::logVarError( sourceName, objectName, methodName, MINOR, "Failed to convert the Date/Time.  %s", tterr.description(tterr.code()));
    }
    catch (...)
    {
        ErrorLogger::logError( sourceName, objectName, methodName, MINOR, "Failed to convert the Date/Time  Unknown Exception.");
    }

    ErrorLogger::logVarError( sourceName, objectName, methodName, TRACE11, "TL1 Msg Date and Time: Date=%s  Time=%s", date.c_str(), time.c_str());
    long ltime = mktime(&t);
    result.setTimeStamp(ltime);
    struct tm* rt;
    struct tm rst;
    const long rtime = result.getTimeStamp();
    rt = localtime_r(&rtime, &rst);
    TextBlocks::const_iterator itr;
    string strFailure;
    string strErrorCode;

    for (itr = event->m_textBlocks.begin(); itr != event->m_textBlocks.end(); itr++)
    {
        // we don't check !result.getStatus() now, since PRTL needs the error description
        if ((*itr).m_type == TextBlock::Comment)
        {
            if (strFailure.empty())
            {
                strFailure = (*itr).m_value;
            }
            else
                strFailure = strFailure + " " + (*itr).m_value;
            continue;
        }
        else if ((*itr).m_type == TextBlock::Unquoted)
        {
            if (strErrorCode.empty())
            {
                strErrorCode = (*itr).m_value;
            }
            else
                strErrorCode = strErrorCode + " " + (*itr).m_value;
            continue;
        }
        else if ((*itr).m_type == TextBlock::Quoted)
        {
            list<string> aidList;
            string sAid;

            vector<string> tokens;
            tokenizeQuotedTextBlock((*itr).m_value, tokens);

            if (names == 0 || names->empty())
            {
                ErrorLogger::logVarError(sourceName, objectName, methodName, MAJOR, "Unable to parse result block. Please add command %s to grammar file", cmdHeader.c_str());
                return ;
            }
            else if (names->size() != tokens.size())
            {
                //The list of names and values should match
                ErrorLogger::logVarError(sourceName, objectName, methodName, MINOR, "Response for command %s does not match expected syntax", cmdHeader.c_str());
            }

            vector<string>::iterator p = names->begin();

            EMS_FW::BaseCmdInfo* baseInfo = new EMS_FW::TL1CmdInfo();
            baseInfo->setType(AgentCmdInfo::CM_ACTION_RESPONSE);
            baseInfo->setTimeStamp(ltime);
            baseInfo->setNeId(neId);
            ((TL1CmdInfo* )baseInfo)->setTl1msgString(cmdHeader);

            bool discard = false;
            for (vector<string>::iterator i = tokens.begin(); !discard && i != tokens.end(); i++)
            {
                try
                {
                    if ((*p) == TL1DEF::AN_AID)
                    {
                        sAid = (*i);

                        MoAttributePo* attr = new MoAttributePo();
                        PrintableStringASN1* value = new PrintableStringASN1();
                        value->SetValue(sAid.c_str());
                        attr->setName(TL1DEF::AN_AID);
                        attr->setValue(value);
                        baseInfo->addAttribute(attr);
                    }
                    else if ((*p) == TL1DEF::AN_SPECIFICBLOCK)
                        _processSpecBlk((*i), baseInfo);
                    else if ((*p) == TL1DEF::AN_STATEBLOCK)
                        _processStateBlk((*i), baseInfo);
                    else     //Position Defined attribute
                        _processPDAttrs((*i), baseInfo, (*p), sAid);
                    p++;
                    if (p == names->end())
                        break;
                }
                catch (BaseException& ex)
                {
                    ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MINOR, "Caught base exception %s", ex.getDescription().c_str());
                    discard = true;
                }
                catch (...)
                {
                    ErrorLogger::logError(sourceName, objectName, methodName, __LINE__, MINOR, "Unknown Exception...");
                    discard = true;
                }
            } // End For
            if (pdMaker && !discard)
            {
                discard = !( pdMaker->isAidSupported(sAid) );
                if (discard)
                    ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE1, "Discard the responses for entity with aid %s", sAid.c_str());
            }
            if (discard)
            {
                delete baseInfo;
                continue;
            }
            ErrorLogger::logError(sourceName, objectName, methodName, TRACE11, "End of parsing the text blocks ");

            if (!sAid.empty())
            {
                // obtain AID
                string tempaid = sAid;
                if (tempaid[tempaid.length() - 1] == ',')
                {
                    tempaid += ','; // osi_tokenizer does not consider "X," as "X" and "", even if allowEmptyTokens is true
                }
                osi_tokenizer tokenizerForAidBlock( ",", true, "\\\n", "\0", "", false );
                vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensAids;
                tokensAids = tokenizerForAidBlock.tokenize(tempaid);
                vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterAids;
                if (tokensAids.size() > 1)
                {
                    for (iterAids = tokensAids.begin(); iterAids != tokensAids.end(); iterAids++)
                        aidList.push_back(*iterAids);
                }
                else
                    aidList.push_back(sAid);
            }
            else
            {
                if (pdMaker)
                {
                    string aid = pdMaker->determineAid (baseInfo, cmdname);
                    aidList.push_back (aid);
                }
            }
            string detMoi, detMoc;
            try
            {
                if (pdMaker)
                {
                    detMoi = pdMaker->determineMoi(neId, aidList, cmdname);
                    detMoc = pdMaker->determineMoc(neId, aidList, cmdname);
                }
            }
            catch (BaseException& ex)
            {
                string errorDes = "Moi " + cmdname + " not supported";
                ErrorLogger::logError(sourceName, objectName, methodName, __LINE__, MINOR, errorDes.c_str());
                result.setStatus(false);
                strFailure = errorDes;  // Default Value
            }
            ErrorLogger::logVarError( sourceName, objectName, methodName, TRACE11, "TL1 DecisionMaker resloved: moi=%s  moc=%s  cmd=%s", detMoi.c_str(), detMoc.c_str(), cmdname.c_str());
            baseInfo->setMoi(detMoi);
            baseInfo->setMoc(detMoc);
            ((EMS_FW::TL1CmdInfo*)baseInfo)->setTL1msgDate(date);
            ((EMS_FW::TL1CmdInfo*)baseInfo)->setTL1msgTime(time);
            ((EMS_FW::TL1CmdInfo*)baseInfo)->setTL1Aid(sAid);

            infoList.push_back(baseInfo);
        }
    } // end For
    if (!result.getStatus())    // If command failed,
    {
        if (!strFailure.empty())
        {
            _stripExtraCharsForStr(strFailure, "\r");
            result.setResultInfo(strFailure);
        }
        if (!strErrorCode.empty())
        {
            _stripExtraCharsForStr(strErrorCode, "\r");
            result.setStrErrCode(strErrorCode);
        }
    }
    else    // If command succeeded,
    {
        if (!strFailure.empty())
        {
            _stripExtraCharsForStr(strFailure, "\r");
            result.setStrErrCode(strFailure); // Pass the comment along
        }
    }

    ErrorLogger::logError(sourceName, objectName, methodName, TRACE11, "Returning...");
}


void TL1MsgConverter::_processPDAttrs(const string& block, EMS_FW::BaseCmdInfo*& info, const string& commonblock, string& aid)
{
    const char* methodName = "_processPDAttrs";
    ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, TRACE11, "Method Begins ......");

    osi_tokenizer tokenizerForCommonBlock( ",", true, "\\\n", "\0", "", false );
    vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensVars, tokensValues;
    tokensValues = tokenizerForCommonBlock.tokenize(block);
    tokensVars = tokenizerForCommonBlock.tokenize(commonblock);
    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterVars;
    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterValues;

    if (!block.empty())
    {
        char last = block[block.length() - 1];
        if (last == ',')
            tokensValues.push_back("");
    }
    if (tokensValues.size() != tokensVars.size())
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MAJOR,
                                 "Common Block Syntax Definition has different number of attributes than command response.\n    %s != %s for %s",
                                 commonblock.c_str(), block.c_str(), aid.c_str());
        return ;
    }
    iterValues = tokensValues.begin();
    for (iterVars = tokensVars.begin(); iterVars != tokensVars.end(); iterVars++)
    {
        string attName = (*iterVars);
        string attValue = (*iterValues);
        _stripExtraCharsForStr(attValue, "\"\r\n \\");
        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, TRACE11, "Variable Name %s And Value %s", attName.c_str(), attValue.c_str());
        if (attName == TL1DEF::AN_AID)
        {
            aid = attValue;
        }
        MoAttributePo* attr = new MoAttributePo();
        PrintableStringASN1* value = new PrintableStringASN1();
        value->SetValue(attValue.c_str());
        attr->setName(attName);
        attr->setValue(value);
        info->addAttribute(attr);
        iterValues++;
    }
    ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, TRACE11, "Method Ends ......");
}

//**************************************************************************
//  Changed to support commas in the quoted strings and performs better
//
void TL1MsgConverter::_processSpecBlk(const string& block, EMS_FW::BaseCmdInfo*& info)
{
    const char* methodName = "_processSpecBlk";
    ErrorLogger::logError(sourceName, objectName, methodName, TRACE11, "Method Begins ......");

    string attName, attValue;
    char currChar = '\0';
    bool collectingName = true, inQuotedString = false;
    string::const_iterator iterC = block.begin();
    while (iterC != block.end())
    {
        currChar = (*iterC++);
        if (currChar == '\n')
            break;
        if (currChar != '\\' && currChar != '\r')
        {
            if (currChar == '\"')
            {
                collectingName = false;
                if (inQuotedString)
                    inQuotedString = false;
                else
                    inQuotedString = true;
            }
            else if (currChar == '='  && !inQuotedString)
            {
                collectingName = false;
            }
            else if (currChar == ','  && !inQuotedString)
            {
                collectingName = true;
                if (!attName.empty() || !attValue.empty())
                {
                    if ((attName == "SOURCE") || (attName == "CTAG"))
                    {
                        ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE1,
                                                 "Encountered a special name-value pair [%s=%s]; storing it in the CTAG field of TL1CmdInfo structure", attName.c_str(), attValue.c_str());
                        ((TL1CmdInfo* )info)->setCTAG(attValue);
                    }
                    else
                    {
                        StringASN1* value = new StringASN1();
                        value->SetValue(attValue.c_str());
                        info->addAttribute(new MoAttributePo(attName, value));
                        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, TRACE7,
                                                 "New Variable: %s = %s", attName.c_str(), attValue.c_str());
                    }
                    if (attName.empty())
                        ErrorLogger::logError(sourceName, objectName, methodName, __LINE__, MAJOR, "Specific Block should be Name-Value pairs!");
                    attName.erase();
                    attValue.erase();
                }
            }
            else if (collectingName)
                attName.append(1, currChar);
            else
                attValue.append(1, currChar);
        }
    }
    if (!attName.empty() || !attValue.empty())
    {
        if ((attName == "SOURCE") || (attName == "CTAG"))
        {
            ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE1,
                                     "Encountered a special name-value pair [%s=%s]; storing it in the CTAG field of TL1CmdInfo structure", attName.c_str(), attValue.c_str());
            ((TL1CmdInfo* )info)->setCTAG(attValue);
        }
        else
        {
            StringASN1* value = new StringASN1();
            value->SetValue(attValue.c_str());
            info->addAttribute(new MoAttributePo(attName, value));
            ErrorLogger::logVarError(sourceName, objectName, methodName, TRACE7,
                                     "New Variable: %s = %s", attName.c_str(), attValue.c_str());
        }
    }

    ErrorLogger::logError(sourceName, objectName, methodName, TRACE11, "Method Ends ......");
}

void TL1MsgConverter::_processStateBlk(const string& block, EMS_FW::BaseCmdInfo*& info)
{
    const char* methodName = "_processStateBlk";
    ErrorLogger::logError(sourceName, objectName, methodName, TRACE11, "Method Begins ......");
    size_t position = block.find_first_of('=');
    if (position != osi_npos)
    {
        _processSpecBlk(block, info);
        return ;
    }
    osi_tokenizer tokenizerForStateBlock( ",", true, "\\\n", "\0", "", false );
    vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensVars;
    tokensVars = tokenizerForStateBlock.tokenize(block);
    if (tokensVars.size() > 2)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MAJOR, "State Block %s should not have more than 2 tokens", block.c_str());
    }
    else if (tokensVars.size() <= 0)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MAJOR, "State Block %s should not be empty", block.c_str());
    }
    else
    {
        string syntax;
        string aid;
        string strBlock = block;
        if (tokensVars.size() == 1)
        {
            syntax = TL1DEF::AN_PRISTATE;
            if (!block.empty())
            {
                char last = block[block.length() - 1];
                if (last == ',')
                {
                    strBlock = block.substr(0, block.length() - 1);
                }
            }
        }
        else
            syntax = TL1DEF::AN_PRISTATE + "," + TL1DEF::AN_SECSTATE;

        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, TRACE10, "Syntax for the block %s is %s", strBlock.c_str(), syntax.c_str());
        _processPDAttrs(strBlock, info, syntax, aid);
    }
    ErrorLogger::logError(sourceName, objectName, methodName, TRACE11, "Method Ends ......");
}

void TL1MsgConverter::_stripExtraCharsForStr (string& msg, const string& extraChars)
{
    // Remove the extra charaters such as ' ' around a string
    int posL = msg.find_first_not_of(extraChars);
    int posR = msg.find_last_not_of(extraChars);
    if ((posL == osi_npos) && (posR == osi_npos))
        msg.erase();
    if ((posL != osi_npos) && (posR != osi_npos))
        msg = msg.substr(posL, posR - posL + 1);
}

void TL1MsgConverter::tokenizeQuotedTextBlock(const string& text, vector<string>& tokens)
{
    string::size_type numColons = 0;
    count(text.begin(), text.end(), ':', numColons);
    tokens.reserve(numColons + 1);

    string value;
    value.reserve(256);
    bool inQuote = false;
    for (const char* p = text.c_str(); *p; p++)
    {
        switch(*p)
        {
            case '\n':
            case '\\':
                break;

            case '"': 
                inQuote = !inQuote;
                value.append(1, *p);
                break;

            case ':':
                if (!inQuote)
                {
                    tokens.push_back(value);
                    value.erase();
                    break;
                }

            default:
                value.append(1, *p);
        }
    }

    tokens.push_back(value);
}
