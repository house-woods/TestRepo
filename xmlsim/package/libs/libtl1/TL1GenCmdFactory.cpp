

/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:11:28  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* TL1GenCmdFactory
* Type: H
* Originator: $Author:   lzou$
* File: $Workfile:   TL1GenCmdFactory.cpp  $
* Version: $Revision:   1.0  $
* Description:
*
* Revision History:
* Rev 1.0  init version  taken from AmsGenCmdFactory
*
**********************************************************************************************/

#include "TL1GenCmdFactory.h"
#include "BaseCmdFactory.h"
#include "BaseException.h"
#include "AgentCmdInfo.h"
#include "TL1AutodiscoveryCmd.h"
#include "CmEventQueueControlCmd.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include "TL1GenericCmd.h"

using namespace EMS_FW;

EMS_FW::BaseCmd* TL1GenCmdFactory::createCommand(EMS_FW::BaseCmdInfo* info, const string& protocol)
{
    if (info->getType() == BaseCmdInfo::CM_AUTODISCOVERY)
        return new TL1AutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_FULL_AUTODISCOVERY)
        return new TL1AutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_REQUESTED_AUTODISCOVERY)
        return new TL1AutodiscoveryCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_SET)
        return new TL1GenericCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_GET)
        return new TL1GenericCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_CREATE)
        return new TL1GenericCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_DELETE)
        return new TL1GenericCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_ACTION)
        return new TL1GenericCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_SUSPEND_EVENTQUEUE_DELAY_DETECTION)
        return new CmEventQueueControlCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_RESUME_EVENTQUEUE_DELAY_DETECTION)
        return new CmEventQueueControlCmd(info);
    string desc = "Unable to find command for ->";
    desc += info->getType();
    // throw BaseException(desc,"BaseCmdFactory","_createCommand");
    return NULL;
}


EMS_FW::BaseCmdInfo* TL1GenCmdFactory::createInfo(const string& neId,
        const string& moc,
        const string& moi,
        const string& type,
        const string& protocol)
{
    string rprotocol = protocol;
    if (protocol == BaseCmdFactory::GENERIC_PROTOCOL)
    {
        rprotocol = Server_i::instance()->getConfigValue(ConfigNames::ProtocolKey);
    }
    maptype::iterator p = _factoryMap.find(rprotocol);
    if (p == _factoryMap.end()) // name is not in a map
    {
        string desc = "Unable to find factory for protocol->";
        desc += protocol;
        throw BaseException(desc, "BaseCmdFactory", "createCommand");
    }
    BaseCmdFactory* factory = (*p).second;
    return factory->createInfo(neId, moc, moi, type, rprotocol);
}




