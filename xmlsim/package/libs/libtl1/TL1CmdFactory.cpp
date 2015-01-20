/***********************************************************************************************
* COPYRIGHT $Date:   June 27 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Command Factory
* Type: CPP
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1CmdFactory.h  $
* Version: $Revision:   1.5  $
* Description: 
* TL1 command factory. 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#include "TL1CmdFactory.h"
#include "TL1GenericCmd.h"
#include "TL1AutodiscoveryCmd.h"
#include "BaseException.h"
#include "BaseCmdInfo.h"
#include "EMSDefines.h"

using namespace EMS_FW;
using namespace std;

extern const char* ServerIdName;

const string TL1CmdFactory::TL1 = "TL1";

EMS_FW::BaseCmd* TL1CmdFactory::createCommand(EMS_FW::BaseCmdInfo* info, const string& protocol)
{
    if (info->getType() == BaseCmdInfo::CM_AUTODISCOVERY)
        return new TL1AutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_FULL_AUTODISCOVERY)
        return new TL1AutodiscoveryCmd(info);
    else if (info->getType() == BaseCmdInfo::CM_REQUESTED_AUTODISCOVERY)
        return new TL1AutodiscoveryCmd(info);
    else
        return new TL1GenericCmd(info);
}


EMS_FW::BaseCmdInfo* TL1CmdFactory::createInfo(const string& neId,
        const string& moc,
        const string& moi,
        const string& type,
        const string& protocol)
{
    if (protocol == EMSDEF::TL1ProtocolKey)
    {
        BaseCmdInfo* info = new BaseCmdInfo(moc, moi, type);
        info->setNeId(neId);
        info->setAddlInfo (ServerIdName);
        return info;
    }

    return BaseCmdFactory::createInfo(neId, moc, moi, type, protocol);

}

const string& TL1CmdFactory::getProtocol()
{
    return TL1CmdFactory::TL1;
}



