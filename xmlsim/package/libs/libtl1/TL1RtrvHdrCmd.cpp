/***********************************************************************************************
* COPYRIGHT 2002 - 2004 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: TL1 Login Command
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1RtrvHdrCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Implements retrieve-header to the TL1 device command.
* 
* Revision History:
*  Origintator     09/27/2004  aaali
*
**********************************************************************************************/

#include "TL1RtrvHdrCmd.h"
#include "sysErrLog.h"
#include "TL1Connection.h"
#include "CmNameResolver.h"
#include "ConfigNames.h"
#include "Server_i.h"
#include "TL1CommandConfigurator.h"
#include "TL1Defines.h"
#include "TL1MsgConverter.h"


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


TL1RtrvHdrCmd::TL1RtrvHdrCmd(TL1Connection* con, const string& tid, const string& commandPrefix, const string &neId)
{
    _tid = tid;
    _neId = neId;
    _discardFlag = false;
    _commandPrefix = commandPrefix;
    _result = CmdResult(false, "This is the default result structure.");

    // initialize info
    _preCmd = commandPrefix + ":" + _tid + "::";
    _postCmd = ";";
    BaseCmdInfo* cmdInfo = new BaseCmdInfo(_postCmd, _preCmd, commandPrefix);
    cmdInfo->setNeId(_neId);
    info_ = (auto_ptr<EMS_FW::BaseCmdInfo>&) cmdInfo;
}


TL1RtrvHdrCmd::~TL1RtrvHdrCmd()
{}

bool TL1RtrvHdrCmd::isUniversalListener()
{
    return true;
}


CmdResult TL1RtrvHdrCmd::execute()
{
    ErrorLogger::logVarError(sourceName, "TL1RtrvHdrCmd", "execute", __LINE__, TRACE11, "About to send the command pre->%s post->%s", _preCmd.c_str(), _postCmd.c_str());
    unsigned long timeout = TL1CommandConfigurator::instance()->getMSECTimeout (_commandPrefix);
    ErrorLogger::logError(sourceName, "TL1RtrvHdrCmd", "execute", __LINE__, TRACE11, "Method finished.");
    return sendCommand(_preCmd, _postCmd, timeout, 1);
}

CmdResult TL1RtrvHdrCmd::execute (TL1Connection *con)
{
    return (TL1BaseCmd::execute (con));
}

void TL1RtrvHdrCmd::handleResponse (TL1Message *message, const string &rawMessage)
{
    if (!_discardFlag)
    {
        // response for another command or an autonomous message was received
        if (message->m_type == TL1Message::Response)
        {
            if (message->m_tag == _ctag) // response for myself
            {
                ErrorLogger::logVarError(sourceName, "TL1RtrvHdrCmd", "handleResponse", __LINE__, TRACE1, "Response received for command [%s]", _commandText.c_str());
                try
                {
                    TL1MsgConverter::instance()->convertMessage (_responseList, message, _result, info_->getType(), info_->getNeId());
                }
                catch (...)
                {
                    const string &comCode = message->m_completeCode;
                    _result.setResultInfo (comCode);
                    if ((comCode == TL1DEF::RES_COMPLETECODE_COMP) || (comCode == TL1DEF::RES_COMPLETECODE_PRTL))
                        _result.setStatus(true);
                    else
                        _result.setStatus(false);
                }
            }
            else
            {
                ErrorLogger::logVarError(sourceName, "TL1RtrvHdrCmd", "handleResponse", __LINE__, TRACE1, "Response received for some other command; [%s] assumed to have succeeded", _commandText.c_str());
                _result.setStrErrCode ("Data received; command assumed to have succeeded");
                _result.setResultInfo (TL1DEF::RES_COMPLETECODE_COMP);
                _result.setStatus (true);
            }
        }
        else
        {
            ErrorLogger::logVarError(sourceName, "TL1RtrvHdrCmd", "handleResponse", __LINE__, TRACE1, "ACKNOWLEDGEMENT or AUTONOMOUS MESSAGE received; [%s] assumed to have succeeded", _commandText.c_str());
            _result.setStrErrCode ("Data received; command assumed to have succeeded");
            _result.setResultInfo (TL1DEF::RES_COMPLETECODE_COMP);
            _result.setStatus (true);
        }
        _lock.post();
    }
}

