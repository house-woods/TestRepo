
/***********************************************************************************************
* COPYRIGHT 1999 - 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: AgentCmd
* Type: C++
* Originator: 
* File: AgentCmd.cpp
* Version: 
* Description: 
* Generic Agent Commands
* 
* Revision History:
*
**********************************************************************************************/


#include "AgentCmd.h"
#include "AgentCmdInfo.h"
#include "BaseCmdFactory.h"
#include "tutil.h"
#include "BaseException.h"
#include "Server_i.h"
#include "ConfigNames.h"


using namespace EMS_FW;

AgentCmd::AgentCmd(EMS_FW::BaseCmdInfo* info) : BaseCmd(info)
{
    // _initialize(); Could not be here.
}

AgentCmd::AgentCmd()
{}

AgentCmd::~AgentCmd()
{}

void AgentCmd::_initialize()
{
    AgentScope scope = ((AgentCmdInfo*)info_.get())->getScope();
    if ((scope.getType() == AgentScope::baseObject) ||
        (scope.getType() == AgentScope::undefinedScope))
        _scoped = false;
    else
        _scoped = true;
}

void AgentCmd::reinitializeWithSameConnection(EMS_FW::BaseCmdInfo* info)
{
    // info_ is auto_ptr and we need to assign other memory to use this autoPtr
    auto_ptr<BaseCmdInfo> ptr(info);
    info_ = ptr;
    _initialize();
}


GenericAgentCmd::~GenericAgentCmd()
{
    if (_cmd)
        delete _cmd;
}

GenericAgentCmd::GenericAgentCmd(EMS_FW::BaseCmdInfo* info)
{
    throw BaseException("Only Cmis Commands are supported", "GenericAgentCmd", "GenericAgentCmd");
}

void GenericAgentCmd::reinitializeWithSameConnection(EMS_FW::BaseCmdInfo* info)
{
    _cmd->reinitializeWithSameConnection(info);
}

bool GenericAgentCmd::hasMoreElements()
{

    return _cmd->hasMoreElements();
}

bool GenericAgentCmd::getNextElement(EMS_FW::BaseCmdInfo* info)
{
    return _cmd->getNextElement(info);
}



CmdResult GenericAgentCmd::execute()
{
    return _cmd->execute();
}

bool GenericAgentCmd::getNextResponse()
{
    return _cmd->getNextResponse();
}
