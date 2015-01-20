/*******************************************************************************
* COPYRIGHT May 21 2004 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
*
* Name: TL1 Edit Password Command
* Type: C++
* Originator: ssteele1
* File: TL1EdPidCmd.cpp
* Description:
* Implements the ED-PID to the TL1 device command.
*
*******************************************************************************/

#include "TL1EdPidCmd.h"
#include "sysErrLog.h"
#include "TL1Connection.h"
#include "CmNameResolver.h"
#include "ConfigNames.h"
#include "Server_i.h"
#include "TL1CommandConfigurator.h"


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


//******************************************************************************
TL1EdPidCmd::TL1EdPidCmd(TL1Connection* pConn, const string& tid, const string& uid,
                         const string& oldPasswd, const string& newPasswd,
                         const string& neId, short encryptType)
{
    _tid = tid;
    _uid = uid;
    _oldPasswd = oldPasswd;
    _newPasswd = newPasswd;
    _neId = neId;
    _encryptType = encryptType;
    _discardFlag = false;
    _result = CmdResult(false, "This is the default result structure.");

    // initialize info
    string preCmd = "ed-pid:" + tid + ":" + uid + ":";
    string postCmd = "::*********,*********;";
    BaseCmdInfo* cmdInfo = new BaseCmdInfo(postCmd, preCmd, "ed-pid");
    cmdInfo->setNeId(neId);
    info_ = (auto_ptr<EMS_FW::BaseCmdInfo>&) cmdInfo;
}


//******************************************************************************
TL1EdPidCmd::~TL1EdPidCmd()
{}

//******************************************************************************
CmdResult TL1EdPidCmd::execute()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "TL1EdPidCmd", "execute", TRACE11, "Method start.");
    string preCmd = "ed-pid:";
    preCmd += _tid;
    preCmd += ":";
    preCmd += _uid;
    preCmd += ":";
    string postCmd = "::";
    postCmd += _oldPasswd;
    postCmd += ",";
    postCmd += _newPasswd;
    if (_encryptType >0 || _oldPasswd.length() > 30)
    {
       if (_encryptType > 1)
          postCmd += ",A";
       else
          postCmd += ",Y";
    }

    postCmd += ";";
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1EdPidCmd", "execute", __LINE__, TRACE11,
                                 "About to send the command pre->%s post->%s", preCmd.c_str(), postCmd.c_str());

    unsigned long timeout = TL1CommandConfigurator::instance()->getMSECTimeout("ed-pid");

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "TL1EdPidCmd", "execute", TRACE11, "Method finished.");
    return sendCommand(preCmd, postCmd, timeout);
}

CmdResult TL1EdPidCmd::execute (TL1Connection *con)
{
    return (TL1BaseCmd::execute(con));
}
