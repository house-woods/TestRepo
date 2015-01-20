/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:44  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: Command Factory
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseCmdFactory.cpp  $
* Version: $Revision:   1.6  $
* Description:
* Command Factory.
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/src/BaseCmdFactory.cpv  $

    Rev 1.6   Oct 27 1999 14:35:44   dtchalak
 Query Interface updates.

    Rev 1.5.1.0   Sep 20 1999 16:18:00   dtchalak
 Just to create branch.

    Rev 1.5   02 Sep 1999 15:43:28   dtchalak
 New interface support added.

    Rev 1.4   21 Jul 1999 10:28:30   dtchalak
 Multiple Ne support included.

    Rev 1.3   23 Jun 1999 16:47:30   dtchalak


    Rev 1.2   14 Jun 1999 16:15:02   dtchalak
 Updates related to new CM.idl and other details.

    Rev 1.1   04 May 1999 19:02:52   dtchalak
 Comments added.

    Rev 1.0   05 Apr 1999 12:34:58   bacuff


    Rev 1.1   04 Feb 1999 14:41:30   dtchalak
 ready for IP-5

    Rev 1.0   22 Jan 1999 12:25:02   dtchalak

//
//    Rev 1.2   Nov 09 1998 16:58:34   dtchalak
//
//
//    Rev 1.1   Nov 05 1998 11:20:52   dtchalak
//
//
//    Rev 1.0   Oct 16 1998 17:30:32   dtchalak
//
**********************************************************************************************/




#include "BaseCmdFactory.h"
#include "AgentCmdInfo.h"
#include "CmAutodiscoveryCmd.h"
#include "BaseException.h"
#include "AgentCmd.h"
#include "tutil.h"

#include "Server_i.h"
#include "ConfigNames.h"

using namespace EMS_FW;
using namespace std;

BaseCmdFactory* BaseCmdFactory::_instance = 0;

const string BaseCmdFactory::GENERIC_PROTOCOL = "GENERIC_PROTOCOL";


BaseCmdFactory* BaseCmdFactory::instance()
{
    if (_instance == 0)
    {
        _instance = new BaseCmdFactory();
    }
    return _instance;
}

BaseCmdFactory::BaseCmdFactory()
{}

BaseCmdFactory::~BaseCmdFactory()
{}


bool BaseCmdFactory::destroy()
{
    if (_instance != NULL)
    {
        delete _instance;
        _instance = NULL;
    }
    return true;
}


const string& BaseCmdFactory::getProtocol()
{
    return BaseCmdFactory::GENERIC_PROTOCOL;
}

void BaseCmdFactory::registerFactory(BaseCmdFactory* factory)
{
    _factoryMap[factory->getProtocol()] = factory;
}


EMS_FW::BaseCmd* BaseCmdFactory::_createCommand(EMS_FW::BaseCmdInfo* info)
{
    if (info->getType() == BaseCmdInfo::CM_AUTODISCOVERY)
        return new CmAutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_FULL_AUTODISCOVERY)
        return new CmAutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_ONDEMAND_AUTODISCOVERY)
        return new CmAutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_REQUESTED_AUTODISCOVERY)
        return new CmAutodiscoveryCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_SET)
        return new GenericAgentCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_GET)
        return new GenericAgentCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_CREATE)
        return new GenericAgentCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_DELETE)
        return new GenericAgentCmd(info);
    else if (info->getType() == AgentCmdInfo::CM_AGENT_ACTION)
        return new GenericAgentCmd(info);

    return NULL;
}

EMS_FW::BaseCmd* BaseCmdFactory::createCommand(EMS_FW::BaseCmdInfo* info, const string& protocol)
{
    //TLIaa79146. This will allow ususage of application implemented BaseCmdFactory.
    //If Application has not implemented any, it will use default

    // if(protocol == BaseCmdFactory::GENERIC_PROTOCOL)
    // {
    //  return _createCommand(info);
    // }
    maptype::const_iterator p = _factoryMap.find(protocol);
    if (p == _factoryMap.end()) // name is not in a map
    {
        string desc = "Unable to find factory for protocol->";
        desc += protocol;
        if (protocol == BaseCmdFactory::GENERIC_PROTOCOL)
        {
            return _createCommand(info);
        }
        else

            throw BaseException(desc, "BaseCmdFactory", "createCommand");
    }
    BaseCmdFactory* factory = (*p).second;
    return factory->createCommand(info, protocol);
}


EMS_FW::BaseCmdInfo* BaseCmdFactory::createInfo(const string& neId,
        const string& moc,
        const string& moi,
        const string& type,
        const string& protocol)
{
    string rprotocol = protocol;
    if (protocol == BaseCmdFactory::GENERIC_PROTOCOL)
    {

        //CmdConnection* con = CmdConnection::getInstance(neId);

        // read protocol from config file.

        rprotocol =
            Server_i::instance()->getConfigValue(ConfigNames::ProtocolKey, neId);

    }
    maptype::const_iterator p = _factoryMap.find(rprotocol);
    if (p == _factoryMap.end()) // name is not in a map
    {
        string desc = "Unable to find factory for protocol->";
        desc += protocol;
        throw BaseException(desc, "BaseCmdFactory", "createCommand");
    }
    BaseCmdFactory* factory = (*p).second;
    return factory->createInfo(neId, moc, moi, type, rprotocol);
}
