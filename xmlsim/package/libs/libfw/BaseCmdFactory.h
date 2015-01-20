/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:48  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Command Factory
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseCmdFactory.h  $
* Version: $Revision:   1.6  $
* Description: 
* Command Factory.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/inc/BaseCmdFactory.h_v  $
 * 
 *    Rev 1.6   Oct 27 1999 14:35:48   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.5.1.0   Sep 20 1999 16:18:12   dtchalak
 * Just to create branch.
 * 
 *    Rev 1.5   02 Sep 1999 15:42:34   dtchalak
 * New interface support added.
 * 
 *    Rev 1.4   21 Jul 1999 10:27:26   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.3   23 Jun 1999 16:47:06   dtchalak
 *  
 * 
 *    Rev 1.2   14 Jun 1999 16:14:00   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.1   04 May 1999 19:01:54   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 12:35:06   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 14:41:42   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 12:25:04   dtchalak
 *  
 * 
 *    Rev 1.1   Nov 09 1998 16:58:34   dtchalak
 *  
 * 
 *    Rev 1.0   Oct 16 1998 17:30:32   dtchalak
 *  
**********************************************************************************************/



#ifndef  _BaseCmdFACTORY_H_
#define _BaseCmdFACTORY_H_


#include "BaseCmd.h"
#include <map>


namespace EMS_FW
{


class BaseCmdInfo;

// Base command factory class. It is used as a wrapper around specific
// factories, that need to be registered in a base factory. Specific factories 
// are implementing different protocols/CMIP,TL1 etc../ or different command
// and command behavior for different products.Class is implemented as a singelton.
// BaseCmdFactory support generic protocol.Generic protocol is not tight to any specific
// protocol it bind command execution to the specific protocol at run time.
class BaseCmdFactory
{
public:

	static const string GENERIC_PROTOCOL;

	static BaseCmdFactory* instance();
	// Return pointer to the base factory class instance;

	void registerFactory(BaseCmdFactory* factory);
	// Register factory implementing specific protocol in a base Factory.

	static bool destroy();
	// Erase factory map.

	virtual EMS_FW::BaseCmd* createCommand(EMS_FW::BaseCmdInfo* info,
						   const string& protocol = GENERIC_PROTOCOL);
	// Return specific implementation of required command. Command type is specified 
	// in BaseCmdInfo. If protocol is not specified GENERIC_PROTOCOL is used.

	virtual EMS_FW::BaseCmdInfo* createInfo(const string& neId,
											const string& moc,
											const string& moi,
											const string& type,
											const string& protocol = GENERIC_PROTOCOL);
	// Return specific for the operation implementation of the BaseCmdInfo.

	virtual const string& getProtocol();
	// Return protocol that factory is supported.


protected:

	typedef map<string, BaseCmdFactory*, less<string> > maptype;

	maptype _factoryMap;
	// Map that contain registered factories.

	BaseCmdFactory(); 
	virtual ~BaseCmdFactory(); 

private:

	static BaseCmdFactory* _instance;
	// Pointer to base factory instance.

	EMS_FW::BaseCmd* _createCommand(EMS_FW::BaseCmdInfo* info);
	// Return command supporting generic protocol.

};


};

#endif
