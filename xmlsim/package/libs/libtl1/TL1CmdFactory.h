/***********************************************************************************************
* COPYRIGHT $Date:   June 27 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Command Factory
* Type: H
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


#ifndef _TL1CMDFACTORY_H_
#define _TL1CMDFACTORY_H_

#include "BaseCmdFactory.h"

namespace EMS_FW
{

// Command factory that support TL1 protocol.
class TL1CmdFactory : public BaseCmdFactory
{
public:

	static const string TL1;

	virtual EMS_FW::BaseCmd* createCommand(EMS_FW::BaseCmdInfo* info,const string& protocol);
	// Return CMIP implementation of the command specified in BaseCmdInfo.

	virtual EMS_FW::BaseCmdInfo* createInfo(const string& neId,
											const string& moc,
											const string& moi,
											const string& type,
											const string& protocol);
	// Return specific for the operation implementation of the BaseCmdInfo.

	virtual const string& getProtocol();
	// Return protocol that factory supported TL1.
};

};

#endif
