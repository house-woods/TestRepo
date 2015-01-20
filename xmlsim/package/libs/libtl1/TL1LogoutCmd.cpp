/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Login Command
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1LogoutCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Implements the logout to the TL1 device command.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#include "TL1LogoutCmd.h"
#include "sysErrLog.h"
#include "TL1Connection.h"
#include "CmNameResolver.h"
#include "ConfigNames.h"
#include "Server_i.h"
#include "TL1CommandConfigurator.h"


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


TL1LogoutCmd::TL1LogoutCmd(TL1Connection* con, const string& tid, const string& uid, const string& neId)
{
    _tid = tid;
    _uid = uid;
    _neId = neId;
    _discardFlag = false;
    _result = CmdResult(false, "This is the default result structure.");

    // initialize info
    string preCmd = "CANC-USER:" + tid + ":" + uid + ":";
    string postCmd = ";";
    BaseCmdInfo* cmdInfo = new BaseCmdInfo(postCmd, preCmd, "CANC-USER");
    cmdInfo->setNeId(neId);
    info_ = (auto_ptr<EMS_FW::BaseCmdInfo>&) cmdInfo;
}

TL1LogoutCmd::~TL1LogoutCmd()
{}

void TL1LogoutCmd::prepare (string &preCmd, string &postCmd, int &timeout)
{
    ErrorLogger::logError(sourceName, "TL1LogoutCmd", "execute", __LINE__, TRACE4, "Method start.");
    preCmd = "CANC-USER:";
    preCmd = preCmd + _tid;
    preCmd = preCmd + ":";
    preCmd = preCmd + _uid;
    preCmd = preCmd + ":";
    postCmd = ";";
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1LogoutCmd", "execute", __LINE__, TRACE10,
                                 "About to send the command pre->%s post->%s", preCmd.c_str(), postCmd.c_str());

    timeout = TL1CommandConfigurator::instance()->getMSECTimeout ("canc-user");


    ErrorLogger::logError(sourceName, "TL1LogoutCmd", "execute", __LINE__, TRACE4, "Method finished.");

}

