/***********************************************************************************************
* COPYRIGHT $Date:   June 25 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Generic Command
* Type: CPP
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1GenericCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Generic TL1 command used to send the commands through the TL1 protocol.
* 
* Revision History:
* Rev 1.2  Jan 10 2003  lzou
*  add a function to check the NE status (active or inactive)
*  If NE is inactive, block the command sending.
*     Rev 1.1    Jan 05 2003 17:32:32   lzou
*  Add Timeout value for command
*  Rev 1.0   Oct 16 2002 17:32:32   dtchalak
*
* $Log:  $
**********************************************************************************************/


#include "TL1GenericCmd.h"
#include "AgentCmdInfo.h"
#include "Server_i.h"
#include "CmNameResolver.h"
#include "ConfigNames.h"
#include "sysErrLog.h"
#include "TL1CommandConfigurator.h"

extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;



TL1GenericCmd::TL1GenericCmd()
{}



TL1GenericCmd::TL1GenericCmd(EMS_FW::BaseCmdInfo* info) : TL1BaseCmd(info)
{}





TL1GenericCmd::~TL1GenericCmd()
{}

CmdResult TL1GenericCmd::execute (TL1Connection *con)
{
    return (TL1BaseCmd::execute (con));
}


CmdResult TL1GenericCmd::execute()
{
    if ( ! isNeActive() )
    {
        ErrorLogger::logVarError(sourceName, "TL1GenericCmd", "execute",
                                 __LINE__, TRACE1,
                                 "Unable to execute TL1GenericCmd because the NE is not active ");
        CmdResult result;
        result.setResultInfo(" Execution Failed, The NE is not active");
        result.setStatus(false);
        return result;
    }
    string preCmd = info_->getMoi();
    string postCmd = info_->getMoc();

    const string cmdType = info_->getType();
    unsigned long timeOut = TL1CommandConfigurator::instance()->getMSECTimeout (cmdType);

    return sendCommand(preCmd, postCmd, timeOut);
}

bool TL1GenericCmd::isNeActive()
{
    AgentCmdInfo* agentCmdInfo = (AgentCmdInfo*)info_.get();
    if (agentCmdInfo)
    {
        return agentCmdInfo->isNeActive();
    }
    return false;
}
