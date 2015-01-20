

/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:10:22  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* TL1GenCmdFactory 
* Type: H 
* Originator: $Author:   lzou$
* File: $Workfile:   TL1GenCmdFactory.h  $
* Version: $Revision:   1.1  $
* Description: 
* Basic Cmip Command Factory
* 
* Revision History:
*	Rev 1.0		init version		taken from AmsGenCmdFactory
*
* $Log:   P:/nsg_dev/Protocols/cmip/common/Inc/TL1GenCmdFactory.h_v  $
 * 
**********************************************************************************************/

#ifndef _TL1GenCmdFactory_H_
#define _TL1GenCmdFactory_H_

#include "BaseCmdFactory.h"

namespace EMS_FW
{

class TL1GenCmdFactory : public BaseCmdFactory
{
public:
	virtual EMS_FW::BaseCmd* createCommand(EMS_FW::BaseCmdInfo* info,const string& protocol);

	virtual EMS_FW::BaseCmdInfo* createInfo(const string& neId,
											const string& moc,
											const string& moi,
											const string& type,
											const string& protocol);

};

};

#endif

