/*******************************************************************************
* COPYRIGHT 2002 - 2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
*
* Name: TL1CmEventReceiver
* Type: C++
* Originator: lzou
* File: TL1CmEventReceiver.cpp
* Version:
* Description:
* Class to represent the event handling for the CM TL1 notifications.
*
* Revision History:
*   Rev 1.6 06/30/2003 adesai
*   modified _extractDateTime and handleevent to return NE local Date/Time in TL1CmdInfo
* Rev 1.5  06/12/2003  lzou
*  add _extractDateTime function to set Date/Time
*   Rev 1.4     06/08/2003      lzou
*       add _extractTL1ReptEvent etc to handle REPT EVT
* Rev 1.3  05/23/2003  lzou
*  Modification NotificationInfo type into TL1CmdInfo
* Rev 1.2      05/14/2003       lzou
*       add the _stripExtraCharsForStr function and modify the _processSpeBlk/_processCommonBlk
* Rev 1.1  01/16/2003  lzou
*  Handle LOC
* Rev 1.0  init version   lzou
*  Initialize the EventReceiver and Handle DbChg Event
*
*
********************************************************************************/
#include "TL1CmEventReceiver.h"
#include "sysErrLog.h"
#include "AgentCmdInfo.h"
#include "TL1CmdInfo.h"
#include "BaseCmdInfo.h"
#include "BaseException.h"
#include "TL1Utility.h"
#include "TL1Defines.h"
#include "TL1ProductDecisionMaker.h"
#include "MoAttributePo.h"
#include "NetworkElement.h"
#include "BaseNameResolver.h"
#include "TL1EventTypeResolver.h"
#include <ospace/helper.h>
#include <ospace/time.h>


using namespace ELogger;
using namespace EMS_FW;
using namespace std;

const string TL1CmEventReceiver::className = "TL1CmEventReceiver";
extern const char* sourceName;

TL1CmEventReceiver::TL1CmEventReceiver(const string& neId)
{
    setNeId(neId.c_str());
    setClassName(className);
    initialize();
}

TL1CmEventReceiver::TL1CmEventReceiver()
{
    setClassName(className);
}

TL1CmEventReceiver::~TL1CmEventReceiver()
{}

void TL1CmEventReceiver::initialize()
{
    ErrorLogger::logError(sourceName, className.c_str(), "initialize", TRACE4, "Method start .....");

    // since the syntax of DbChg event varies from one NE to another NE,
    // We should get this string from config file or ProductDecisionMaker
    // "time,date,source,userid,dbchgseq:cmdcode:aid:common_block:specific_block:pst,sst";
    string neId = getNeId();
    //    NetworkElement* ne = Server_i::instance()->locateNetworkElement(neId);
    string evtType = TL1DEF::EVT_DBCHG;
    TL1ProductDecisionMaker* maker = (TL1ProductDecisionMaker*)
                                     ProtocolProductDecisionMaker::getProductDecisionMaker();

    const string sDbChg = maker->getReptEventSyntax(neId, evtType);
    ErrorLogger::logVarError(sourceName, className.c_str(), "initialize", __LINE__, TRACE7,
                             "DB CHG Syntax: %s", sDbChg.c_str());
    evtType = TL1DEF::EVT_EVT;
    const string sEvt = maker->getReptEventSyntax (neId, evtType);
    ErrorLogger::logVarError(sourceName, className.c_str(), "initialize", __LINE__, TRACE7,
                             "Event Syntax: %s", sEvt.c_str());
    evtType = TL1DEF::EVT_SESSION;
    const string sEvtSess = maker->getReptEventSyntax (neId, evtType);
    ErrorLogger::logVarError(sourceName, className.c_str(), "initialize", __LINE__, TRACE7,
                             "Event Syntax: %s", sEvtSess.c_str());

    osi_tokenizer tokenizer( ":", true, "\\", "\n", "", false );

    _reptDbChg = tokenizer.tokenize( sDbChg );
    _reptEvt = tokenizer.tokenize( sEvt );
    _reptEvtSess = tokenizer.tokenize( sEvtSess );
    ErrorLogger::logError(sourceName, className.c_str(), "initialize", TRACE4, "Method Ends ......");

}

//******************************************************************************
bool TL1CmEventReceiver::handleEvent(TL1Message* event, const string& rawMessage)
{
    ErrorLogger::logError(sourceName, className.c_str(), "handleEvent", TRACE4, "Method start");

    bool bRetVal = true;
    try
    {
        const string& msgType1 = event->m_outputCode2;
        const string& msgType2 = event->m_outputCode3;

        TextBlocks::const_iterator itr;
        for (itr = event->m_textBlocks.begin();itr != event->m_textBlocks.end();itr++)
        {
            if ((*itr).m_type == TextBlock::Quoted)
            {

                BaseCmdInfo* info = new TL1CmdInfo();
                const string& aMsg = (*itr).m_value;

                if (ErrorLogger::isErrTypeEnabled(TRACE10))
                {
                    string duplicateMessage;
                    ErrorLogger::maskPassword(aMsg, duplicateMessage);
                    ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE10, "The first Text block for this event is: %s ", duplicateMessage.c_str());
                }

                if (isAppInterested(msgType1, msgType2))
                {
                    try
                    {
                        handleAppEvent(aMsg, info, msgType1, msgType2, rawMessage );
                    }
                    catch (...)
                    {
                        string duplicateMessage;
                        ErrorLogger::maskPassword(aMsg, duplicateMessage);
                        ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__,
                                                 CRITICAL, "handleAppEvent() failed to process the event msg %s", duplicateMessage.c_str());
                        delete info;
                        info = NULL;
                        // ignore the exception
                        continue;
                    }
                }
                else if (msgType1 == TL1DEF::EVT_DBCHG)
                {
                    ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__, TRACE7, "Autonomous Message is DBCHG");

                    try
                    {
                        _extractTL1DbChgEvent(aMsg, info, msgType1, msgType2, rawMessage);
                    }
                    catch (...)
                    {
                        string duplicateMessage;
                        ErrorLogger::maskPassword(aMsg, duplicateMessage);
                        ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__,
                                                 CRITICAL, "_extractTL1DbChgEvent() failed to process the event msg %s", duplicateMessage.c_str());
                        delete info;
                        info = NULL;
                        // ignore the exception
                        continue;
                    }
                }
                else if (msgType1 == TL1DEF::EVT_EVT)
                {
                    ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__, TRACE7, "Autonomous Message is EVT");

                    try
                    {
                        _extractTL1ReptEvent (aMsg, info, msgType1, msgType2, rawMessage);
                    }
                    catch (...)
                    {
                        string duplicateMessage;
                        ErrorLogger::maskPassword(aMsg, duplicateMessage);
                        ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__,
                                                 CRITICAL, "_extractTL1ReptEvent() failed to process the event msg %s", duplicateMessage.c_str());
                        delete info;
                        info = NULL;
                        // ignore the exception
                        continue;

                    }
                }
                else
                {
                    ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__, TRACE7, "Autonomous Message is ALM related. Ignore it.");

                    if (info)
                    {
                        delete info;
                        info = NULL;
                    }
                    //ignore it
                    continue;
                }
                if (info->getType() != BaseCmdInfo::CM_INVALID_EVENT)
                {
                    // send info object to event queue
                    info->setNeId(getNeId());
                    time_t eventTime;
                    time(&eventTime);
                    info->setTimeStamp(eventTime);

                    try
                    {
                        bRetVal = CmEventQueue::SendMessage(info, getNeId());
                    }
                    catch (MessageQueueOverflow& ovf)
                    {
                        ErrorLogger::logVarError(sourceName, className.c_str(), "handleEvent", __LINE__,
                                                 CRITICAL, "handleEvents() Unable to Event in a queue Overflow");
                        if (info)
                        {
                            delete info;
                            info = NULL;
                        }

                        throw;
                    }
                }    // end of if- invalid event
                else
                {
                    ErrorLogger::logError(sourceName, className.c_str(), "handleEvent", __LINE__,
                                          CRITICAL, "handleEvents() Unable to Extract Event");
                    if (info)
                    {
                        delete info;
                        info = NULL;
                    }
                }    // end of if - invalid type
            }    // end of if - Quato block
        } // end of for loop
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(sourceName, ex.getExClassName().c_str(),
                                 ex.getMethodName().c_str(), __LINE__,
                                 ex.getSeverity(), ex.getDescription().c_str());
        // ATTENTION need to notify Process Manager.
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, className.c_str(), "handleEvent", __LINE__,
                              CRITICAL, "handleEvents() Unable to Process Event");
        ErrorLogger::dumpTraceList();
    }

    return bRetVal;
}


//******************************************************************************
void TL1CmEventReceiver::_extractTL1DbChgEvent(const string& testBlock, EMS_FW::BaseCmdInfo*& info,
        const string& typeCode1,
        const string& typeCode2, const string& rawMessage)
{
    ErrorLogger::logError(sourceName, className.c_str(), "_extractTL1DbChgEvent", TRACE4, "Method start");

    vector<string> tokens;

    tokenizeQuotedTextBlock(testBlock, tokens);

    ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, TRACE7, "%s has %d tokens:", testBlock.c_str(), tokens.size());
    vector<string>::iterator i;

    // Follows are the variables we are interested in
    list<string> aidList;
    string cmd;
    string specific_block;
    string common_block;
    TL1ProductDecisionMaker* maker = (TL1ProductDecisionMaker*)
                                     ProtocolProductDecisionMaker::getProductDecisionMaker();

    vector<string>::iterator p;
    p = _reptDbChg.begin();
    for (i = tokens.begin(); i != tokens.end(); i++)
    {
        if (p == _reptDbChg.end())
        {
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, MAJOR,
                                     "DBCHG Syntax is too short");
            continue;
        }
        if ((*p) == TL1DEF::AN_DBCHGHEADER)
        {
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, TRACE3, "DBCHG HEADER %s", (*i).c_str());
            _processDbChgHeader((*i), info);
            p++;
            continue;
        }
        if ((*p) == TL1DEF::AN_AID)
        {
            const string& aid = (*i);
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, TRACE4, "AID %s", aid.c_str());
            string tempaid = aid;
            if (tempaid[tempaid.length() - 1] == ',')
            {
                tempaid += ',';
            }
            osi_tokenizer tokenizerForAidBlock( ",", true, "\\", "\n", "", false );
            vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensAids;
            tokensAids = tokenizerForAidBlock.tokenize(tempaid);
            vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterAids;
            if (tokensAids.size() > 1)
            {
                for (iterAids = tokensAids.begin(); iterAids != tokensAids.end(); iterAids++)
                {
                    aidList.push_back(*iterAids);
                }
            }
            else
            {
                aidList.push_back(aid);
            }
            MoAttributePo* attr = new MoAttributePo();
            StringASN1* value = new StringASN1();
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, TRACE10,
                                     "AID is %s before add AID attr", aid.c_str());

            // Currently, we only pass the original aid instead of aidList to application
            // If we want to pass a list of string, we need to define a new MoValue type
            // The SequenceANS1 type in BaseTypeASN1 is not implemented.
            value->SetValue(aid.c_str());
            attr->setName(TL1DEF::AN_AID);
            attr->setValue(value);
            info->addAttribute(attr);
            ((TL1CmdInfo*) info)->setTL1Aid(aid);  // For things which don't want to hunt it down.
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_CMDCODE)
        {
            cmd = (*i);
            ((TL1CmdInfo*) info)->setTl1msgString(cmd);
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, TRACE7, "CMD %s", cmd.c_str());
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_COMMONBLOCK)
        {
            common_block = (*i);
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_SPECIFICBLOCK)
        {
            specific_block = (*i);
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_STATEBLOCK)
        {
            osi_tokenizer tokenizerForAidBlock( ",", true, "\\", "\n", "", false );
            vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensStates;
            tokensStates = tokenizerForAidBlock.tokenize((*i));
            vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterStates;
            if (tokensStates.size() >= 1)
            {
                iterStates = tokensStates.begin();
                const string& attName = TL1DEF::AN_PRISTATE;
                string attValue = (*iterStates);

                _stripExtraCharsForStr(attValue, "\r\n \\");

                MoAttributePo* attr = new MoAttributePo();
                StringASN1* value = new StringASN1();
                value->SetValue(attValue.c_str());
                attr->setName(TL1DEF::AN_PRISTATE);
                attr->setValue(value);
                info->addAttribute(attr);
                iterStates++;
                if (iterStates != tokensStates.end())
                {
                    attValue = (*iterStates);

                    _stripExtraCharsForStr(attValue, "\r\n \\");
                }
                else
                    attValue = "";
                MoAttributePo* attr1 = new MoAttributePo();
                StringASN1* value1 = new StringASN1();
                value1->SetValue(attValue.c_str());
                attr1->setName(TL1DEF::AN_SECSTATE);
                attr1->setValue(value1);
                info->addAttribute(attr1);
            }   // end of If size()
            p++;
            continue;
        }
        //go to the next name

    }

    bool aidAvailable = true;
    if (aidList.empty())
    {
        aidAvailable = false;
    }
    else
    {
        const string& aid = aidList.front();
        if (aid.empty())
            aidAvailable = false;
    }

	if (cmd == "ED-ODU1TA" || cmd == "ED-ODU1TB")
	{
		cmd = "ED-ODU1T";
	}
	
	if (cmd == "ED-ODU2TA" || cmd == "ED-ODU2TB")
	{
		cmd = "ED-ODU2T";
	}
	
	if (cmd == "ED-ODU0TA" || cmd == "ED-ODU0TB")
	{
		cmd = "ED-ODU0T";
	}
	
	if (cmd == "ED-ODU3TA" || cmd == "ED-ODU3TB")
	{
		cmd = "ED-ODU3T";
	}
	
	
	if (cmd == "DLT-ODU1TA" || cmd == "DLT-ODU1TB")
	{
		cmd = "DLT-ODU1T";
	}
	
	if (cmd == "DLT-ODU2TA" || cmd == "DLT-ODU2TB")
	{
		cmd = "DLT-ODU2T";
	}
	
	if (cmd == "DLT-ODU0TA" || cmd == "DLT-ODU0TB")
	{
		cmd = "DLT-ODU0T";
	}
	
	if (cmd == "DLT-ODU3TA" || cmd == "DLT-ODU3TB")
	{
		cmd = "DLT-ODU3T";
	}
	
    string moi, moc;

    if (!aidAvailable)
    {
        if (!common_block.empty())
            _extractCommonBlKAttrs(common_block, info, cmd);
        if (!specific_block.empty())
            _extractSpeBlKAttrs(specific_block, info);
        if (maker)
        {
            const string& aid = maker->determineAid (info, rawMessage);
            list<string> aids;
            aids.push_back (aid);
            moi = maker->determineMoi(getNeId(), aids, cmd);
            moc = maker->determineMoc(getNeId(), aids, cmd);
        }
    }
    else
    {
        if (maker)
        {
            moi = maker->determineMoi(getNeId(), aidList, cmd);
            moc = maker->determineMoc(getNeId(), aidList, cmd);
        }
        if (!common_block.empty())
            _extractCommonBlKAttrs(common_block, info, cmd);
        if (!specific_block.empty())
            _extractSpeBlKAttrs(specific_block, info);
    }
    BaseNameResolver* evtTypeResolver = TL1EventTypeResolver::instance();
    const string& eventType = evtTypeResolver->resolveName(cmd);
    ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1DbChgEvent", __LINE__, TRACE10,
                             "Event Type is %s ", eventType.c_str());

    info->setType(eventType);
    info->setMoi(moi);
    info->setMoc(moc);
    ErrorLogger::logError(sourceName, className.c_str(), "_extractTL1DbChgEvent", TRACE4, "Method Ends ......");
}

//******************************************************************************
void TL1CmEventReceiver::_extractTL1ReptEvent(const string& testBlock, EMS_FW::BaseCmdInfo*& info, const string& typeCode1,
        const string& typeCode2, const string& rawMessage)
{
    ErrorLogger::logError(sourceName, className.c_str(), "_extractTL1ReptEvent", TRACE4, "Method start");
    TL1ProductDecisionMaker* maker = (TL1ProductDecisionMaker*)
                                     ProtocolProductDecisionMaker::getProductDecisionMaker();

    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator p;
    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator pEnd;
    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator i;
    osi_tokenizer tokenizerForBlock( ":", true, "\\", "\n", "", false );
    if (typeCode1 != TL1DEF::EVT_EVT)
    {
        ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, MAJOR,
                                 "Invalid Event type %s", typeCode1.c_str());
        info->setType(BaseCmdInfo::CM_INVALID_EVENT);
        return ;
    }
    if (typeCode2 == TL1DEF::EVT_SESSION)
    {
        ErrorLogger::logError(sourceName, className.c_str(), "_extractTL1ReptEvent", TRACE7, "Autonomous Message is EVT SESSION");
        p = _reptEvtSess.begin();
        pEnd = _reptEvtSess.end();
    }
    else
    {
        const string& sEvt = maker->getReptEventSyntax(getNeId(), TL1DEF::EVT_EVT, typeCode2);
        ErrorLogger::logVarError(sourceName, className.c_str(), "initialize", __LINE__, TRACE7,
                                 "Event Syntax: %s", sEvt.c_str());
        if (sEvt.empty())
        {
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, MAJOR,
                                     "Event Syntax is empty.");
            info->setType(BaseCmdInfo::CM_INVALID_EVENT);
            return ;
        }
        osi_tokenizer tokenizerForBlock( ":", true, "\\", "\n", "", false );
        _reptEvt.clear();
        _reptEvt = tokenizerForBlock.tokenize( sEvt );
        p = _reptEvt.begin();
        pEnd = _reptEvt.end();
    }

    // Follows are the variables we are interested in
    list<string> aidList;
    string cmd;
    string specific_block;
    string common_block;

    vector < string OS_DEFAULT_ALLOCATOR( string ) > tokens;

    tokens = tokenizerForBlock.tokenize(testBlock);
    ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE7, "%s has %d tokens:", testBlock.c_str(), tokens.size());

    for (i = tokens.begin(); i != tokens.end(); i++)
    {
        if (p == pEnd)
        {
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, MAJOR,
                                     "Evt Syntax is too short");
            continue;
        }
        if ((*p) == TL1DEF::AN_AID)
        {
            const string& aid = (*i);
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE7, "AID %s", aid.c_str());
            osi_tokenizer tokenizerForAidBlock( ",", true, "\\", "\n", "", false );
            vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensAids;
            tokensAids = tokenizerForAidBlock.tokenize(aid);
            vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterAids;
            if (tokensAids.size() > 1)
            {
                for (iterAids = tokensAids.begin(); iterAids != tokensAids.end(); iterAids++)
                {
                    aidList.push_back(*iterAids);
                }
            }
            else
            {
                aidList.push_back(aid);
            }
            MoAttributePo* attr = new MoAttributePo();
            StringASN1* value = new StringASN1();
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE10,
                                     "AID is %s before add AID attr", aid.c_str());

            // Currently, we only pass the original aid instead of aidList to application
            // If we want to pass a list of string, we need to define a new MoValue type
            // The SequenceANS1 type in BaseTypeASN1 is not implemented.
            value->SetValue(aid.c_str());
            attr->setName(TL1DEF::AN_AID);
            attr->setValue(value);
            info->addAttribute(attr);
            ((TL1CmdInfo*) info)->setTL1Aid(aid);  // For things which don't want to hunt it down.
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_CMDCODE)
        {
            cmd = (*i);
            ((TL1CmdInfo*) info)->setTl1msgString(cmd);
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE7, "CMD %s", cmd.c_str());
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_COMMONBLOCK)
        {
            common_block = (*i);
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_SPECIFICBLOCK)
        {
            specific_block = (*i);
            p++;
            continue;
        }
        else if ((*p) == TL1DEF::AN_CONDDESCBLOCK)
        {
            const string& conddescr = (*i);
            MoAttributePo* attr = new MoAttributePo();
            StringASN1* value = new StringASN1();
            if (ErrorLogger::isTraceListOn())
                ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE10,
                                         "conddescr is %s before add AID attr", conddescr.c_str());
            value->SetValue(conddescr.c_str());
            attr->setName(TL1DEF::AN_CONDDESCBLOCK);
            attr->setValue(value);
            info->addAttribute(attr);
            p++;
            continue;
        }
        //go to the next name
    }

    string moi;
    string moc;

    if (maker)
    {
        moi = maker->determineMoi(getNeId(), aidList, cmd);
        moc = maker->determineMoc(getNeId(), aidList, cmd);
    }
    if (!common_block.empty())
        _extractCommonBlKAttrs(common_block, info, typeCode2);
    if (!specific_block.empty())
        _extractSpeBlKAttrs(specific_block, info);
    BaseNameResolver* evtTypeResolver = TL1EventTypeResolver::instance();

    string evt = "REPT " + typeCode1 + " " + typeCode2;

    // for event, determine the event type by Output Code
    const string& eventType = evtTypeResolver->resolveName(evt);
    ErrorLogger::logVarError(sourceName, className.c_str(), "_extractTL1ReptEvent", __LINE__, TRACE10,
                             "Event Type is %s from evt %s", eventType.c_str(), evt.c_str());

    info->setType(eventType);
    info->setMoi(moi);
    info->setMoc(moc);
    ErrorLogger::logError(sourceName, className.c_str(), "_extractTL1ReptEvent", TRACE4, "Method Ends ......");
}

//******************************************************************************
// This method was rewritten to better deal with commas in quoted strings.
// The code may look more primative than what was here, but performs better.  Steve S
//
void TL1CmEventReceiver::_extractSpeBlKAttrs(const string& block, EMS_FW::BaseCmdInfo*& info)
{
    ErrorLogger::logError(sourceName, className.c_str(), "_extractSpeBlKAttrs", TRACE4, "Method Begins ......");

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
                        ErrorLogger::logVarError(sourceName, className.c_str(), "_extractSpeBlKAttrs", TRACE1,
                                                 "Encountered a special name-value pair [%s=%s]; storing it in the CTAG field of TL1CmdInfo structure", attName.c_str(), attValue.c_str());
                        ((TL1CmdInfo* )info)->setCTAG(attValue);
                    }
                    else
                    {
                        StringASN1* value = new StringASN1();
                        value->SetValue(attValue.c_str());
                        info->addAttribute(new MoAttributePo(attName, value));
                        ErrorLogger::logVarError(sourceName, className.c_str(), "_extractSpeBlKAttrs", __LINE__, TRACE7,
                                                 "New Variable: %s = %s", attName.c_str(), attValue.c_str());
                    }
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
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractSpeBlKAttrs", TRACE1,
                                     "Encountered a special name-value pair [%s=%s]; storing it in the CTAG field of TL1CmdInfo structure", attName.c_str(), attValue.c_str());
            ((TL1CmdInfo* )info)->setCTAG(attValue);
        }
        else
        {
            StringASN1* value = new StringASN1();
            value->SetValue(attValue.c_str());
            info->addAttribute(new MoAttributePo(attName, value));
            ErrorLogger::logVarError(sourceName, className.c_str(), "_extractSpeBlKAttrs", __LINE__, TRACE7,
                                     "New Variable: %s = %s", attName.c_str(), attValue.c_str());
        }
    }

    ErrorLogger::logError(sourceName, className.c_str(), "_extractSpeBlKAttrs", TRACE4, "Method Ends ......");

}

//******************************************************************************
// This method was rewritten to better deal with commas in quoted strings.
// The code may look more primative than what was here, but performs better.  Steve S
//
void TL1CmEventReceiver::_extractCommonBlKAttrs(const string& block, EMS_FW::BaseCmdInfo*& info,
        const string& cmd)
{
    ErrorLogger::logError(sourceName, className.c_str(), "_extractCommonBlKAttrs", TRACE4, "Method Begins ......");

    TL1ProductDecisionMaker* maker = (TL1ProductDecisionMaker*)
                                     ProtocolProductDecisionMaker::getProductDecisionMaker();
    if (!maker)
    {
        ErrorLogger::logError(sourceName, className.c_str(), "_extractCommonBlKAttrs", __LINE__, MAJOR,
                              "Did not get a ProductDecisionMaker pointer");
        return;
    }
    vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensVars, tokensValues;
    maker->getCommonBlockSyntax(cmd, block, tokensVars);

    string value;
    char currChar = '\0';
    bool inQuotedString = false;
    string::const_iterator iterC = block.begin();
    while (iterC != block.end())
    {
        currChar = (*iterC++);
        if (currChar == '\n')
            break;
        if (currChar != '\\' && currChar != '\r' && (currChar != ' ' || inQuotedString))
        {
            if (currChar == '\"')
            {
                if (inQuotedString)
                    inQuotedString = false;
                else
                    inQuotedString = true;
            }
            else if ((currChar == ',' || currChar == ':') && !inQuotedString)
            {
                tokensValues.push_back(value);
                value.erase();
            }
            else
                value.append(1, currChar);
        }
    }
    tokensValues.push_back(value);

    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterVars;
    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterValues;

    if (tokensValues.size() != tokensVars.size())
    {
        ErrorLogger::logVarError(sourceName, className.c_str(), "_extractCommonBlKAttrs", __LINE__, MAJOR,
                                 "Common Block Syntax Definition has different size of value obtained from DBCHG event. '%s' for %s cmd",
                                 block.c_str(), cmd.c_str());
        return ;
    }
    iterValues = tokensValues.begin();
    for (iterVars = tokensVars.begin(); iterVars != tokensVars.end(); iterVars++)
    {
        StringASN1* value = new StringASN1();
        value->SetValue((*iterValues).c_str());
        info->addAttribute(new MoAttributePo((*iterVars), value));
        iterValues++;
    }

    ErrorLogger::logError(sourceName, className.c_str(), "_extractCommonBlKAttrs", TRACE4, "Method Ends ......");
}

//******************************************************************************
void TL1CmEventReceiver::handleCommand(string& request)
{
    ErrorLogger::logError(sourceName, className.c_str(), "handleCommand", TRACE4, "Method begins ......");
    try
    {
        TL1Connection* pCon = TL1Connection::getInstance(getNeId());
        pCon->send_data(request.c_str());
    }
    catch (...)
    {
        ErrorLogger::logError( sourceName, "TL1CmEventReceiver", "handleCommand", TRACE1, "No Connector Available");
    }
    ErrorLogger::logError(sourceName, className.c_str(), "handleCommand", TRACE4, "Method Ends ......");

}

//******************************************************************************
// This method removes the extra charaters such as ' ' around a string
void TL1CmEventReceiver::_stripExtraCharsForStr (string& msg, const string& extraChars)
{
    int posL = msg.find_first_not_of (extraChars);
    int posR = msg.find_last_not_of (extraChars);
    if ((posL == string::npos) && (posR == string::npos))
    {
        msg = "";
    }
    if ((posL != string::npos) && (posR != string::npos))
    {
        msg = msg.substr(posL, posR - posL + 1);
    }
}

//******************************************************************************
// This method will extract the Date/Time from the header of event,
// then translate into long type and set into the timeStamp of TL1CmdInfo structure
//long eventTime is UTC time, where as the date/time in string format is  NE local time
void TL1CmEventReceiver::_extractDateTime (TL1Message* event, long& evtTime, string& date, string& time)
{
    //get the event time as a long
    struct tm t;
    t.tm_sec = event->m_dateTime.m_sec;
    t.tm_min = event->m_dateTime.m_min;
    t.tm_hour = event->m_dateTime.m_hour;
    t.tm_mday = event->m_dateTime.m_day;
    t.tm_mon = event->m_dateTime.m_mon - 1;
    t.tm_year = event->m_dateTime.m_year + 100;
    try
    {
        osi_date eventDate(event->m_dateTime.m_mon, event->m_dateTime.m_day, event->m_dateTime.m_year + 2000);
        osi_time eventTime(event->m_dateTime.m_hour, event->m_dateTime.m_min, event->m_dateTime.m_sec);

        time = eventTime.to_string("%H-%M-%S");
        date = eventDate.to_string("%y-%m-%d");
    }
    catch (osi_time_toolkit_error)
    {
        ErrorLogger::logVarError( sourceName, "TL1CmEventReceiver", "_extractDateTime", __LINE__, MINOR,
                                  "Fail to transfer the Date/Time");

    }
    catch (...)
    {
        ErrorLogger::logVarError( sourceName, "TL1CmEventReceiver", "_extractDateTime", __LINE__, MINOR,
                                  "Fail to transfer the Date/Time");

    }

    ErrorLogger::logVarError( sourceName, "TL1CmEventReceiver", "_extractDateTime", TRACE10,
                              "TL1 Msg Date and Time: Date=%s  Time=%s", date.c_str(), time.c_str());



    evtTime = mktime(&t);

}

//******************************************************************************
void TL1CmEventReceiver::_processDbChgHeader (const string& header, EMS_FW::BaseCmdInfo*& info)
{
    ErrorLogger::logError(sourceName, className.c_str(), "_processDbChgHeader", TRACE4, "Method start");

    // A DBChange header has tokens separated by commas
    // A comma preceded with no field will be ignored
    // so we can put as many or no commas after the DATE without breaking the parsing
    osi_tokenizer tokenizer( ",", true, "\\", "\n", "", false );
    vector < string OS_DEFAULT_ALLOCATOR( string ) > tokensVars;
    vector < string OS_DEFAULT_ALLOCATOR( string ) > ::iterator iterVars;
    tokensVars = tokenizer.tokenize(header);
    for (iterVars = tokensVars.begin(); iterVars != tokensVars.end(); iterVars++)
    {
        const string& str = (*iterVars);
        ErrorLogger::logVarError(sourceName, className.c_str(), "_processDbChgHeader", __LINE__, TRACE7, "Variable: %s", str.c_str());
        if (str.empty())
            continue;

        size_t position = str.find_first_of('=');
        if (position != string::npos)
        {
            // Name Value Pair
            const string& attName = str.substr(0, position);
            string attValue = str.substr(position + 1, str.size() - position);

            _stripExtraCharsForStr(attValue, "\"\r\n \\");
            if (attName == TL1DEF::AN_DBCHG_TIME)
            {
                ((TL1CmdInfo*)info)->setTL1msgTime(attValue);
            }
            else if (attName == TL1DEF::AN_DBCHG_DATE)
            {
                ((TL1CmdInfo*)info)->setTL1msgDate(attValue);
            }
            else if (attName == "DBCHGSEQ")
            {
                ((TL1CmdInfo*)info)->setDBCHGSEQ(attValue);
            }
        }
        else
        {
            // Position Defines
            ErrorLogger::logVarError(sourceName, className.c_str(), "_processDbChgHeader", __LINE__, MAJOR,
                                     "DBCHG Header should be Name-Value pairs!");
        }
    }
    ErrorLogger::logVarError(sourceName, className.c_str(), "_processDbChgHeader", TRACE4, "Method end ...");

}


//******************************************************************************
bool TL1CmEventReceiver::isAppInterested (const string& typeCode1, const string& typeCode2)
{
    return false;
}

//******************************************************************************
void TL1CmEventReceiver::handleAppEvent (const string& testBlock, EMS_FW::BaseCmdInfo*& info, const string& typeCode1,
        const string& typeCode2, const string& rawMessage)
{
    ErrorLogger::logError(sourceName, className.c_str(), "handleAppEvent", MINOR,
                          "Method not supported at this level.");
}


//******************************************************************************
void TL1CmEventReceiver::blockedAction ()
{
    const string& neId = getNeId();
    ErrorLogger::logVarError(sourceName, className.c_str(), "blockedAction", TRACE4,
                             "Events Blocked Method begins for NE=%s", neId.c_str());

    BaseCmdInfo* info = new TL1CmdInfo("NE", neId, BaseCmdInfo::CM_AVC_EVENT, 0);
    ((TL1CmdInfo*) info)->setTl1msgString("BLOCKED");
    info->setNeId(neId);
    try
    {
        const string &tid = Server_i::instance()->getDefaultConfigValue(ConfigNames::TID, neId, neId);
        ErrorLogger::logVarError(sourceName, className.c_str(), "blockedAction", TRACE1,
                                 "Blocked Event Receiving: notifying NE [%s]", tid.c_str());
        CmEventQueue::flush(neId);
        CmEventQueue::SendMessage(info, neId);
    }
    catch (MessageQueueOverflow& ovf)
    {
        ErrorLogger::logError(sourceName, className.c_str(), "blockedAction", __LINE__, ELogger::CRITICAL,
                              "Unable to send NE blocked info, Queue Overflow");
    }
}

//******************************************************************************
void TL1CmEventReceiver::unblockedAction ()
{
    const string& neId = getNeId();
    ErrorLogger::logVarError(sourceName, className.c_str(), "unblockedAction", TRACE4,
                             "Events Unblocked Method begins for NE=%s", neId.c_str());

    AgentCmdInfo* ainfo = new AgentCmdInfo(Server_i::instance()->getConfigValue(ConfigNames::RootMocKey, neId),
                                           Server_i::instance()->getConfigValue(ConfigNames::RootMoiKey, neId),
                                           BaseCmdInfo::CM_REQUESTED_AUTODISCOVERY);
    ainfo->setNeId(neId);
    try
    {
        const string &tid = Server_i::instance()->getDefaultConfigValue(ConfigNames::TID, neId, neId);
        ErrorLogger::logVarError(sourceName, className.c_str(), "unblockedAction", TRACE1,
                                 "Unblocked Event Receiving: Autodiscovery start request for [%s]", tid.c_str());
        CmEventQueue::flush(neId);
        CmEventQueue::SendMessage(ainfo, neId);
    }
    catch (MessageQueueOverflow& ovf)
    {
        ErrorLogger::logError(sourceName, className.c_str(), "unblockedAction", __LINE__, ELogger::CRITICAL,
                              "Unable to send autodiscover command Queue Overflow");
    }

}

void TL1CmEventReceiver::tokenizeQuotedTextBlock(const string& text, vector<string>& tokens)
{
    string::size_type numColons = 0;
    count(text.begin(), text.end(), ':', numColons);
    tokens.reserve(numColons + 1);

    string value;
    value.reserve(256);
    bool inQuote = false, done = false;
    for (const char* p = text.c_str(); !done && *p; p++)
    {
        switch(*p)
        {
            case '\n':
                done = true;

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
