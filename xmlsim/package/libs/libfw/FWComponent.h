

/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:47:18  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: FWComponent
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   FWComponent.h  $
* Version: $Revision:   1.3  $
* Description: 
* Base Framework Component.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/FWComponent.hv_  $
 * 
 *    Rev 1.3   Oct 27 1999 14:47:18   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.2.1.0   Sep 20 1999 16:48:38   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.2   08 Sep 1999 13:06:02   dtchalak
 * USE_ALTERNATE_STD defines added.
 * 
 *    Rev 1.1   02 Sep 1999 15:51:06   dtchalak
 * New interface support added.
 * 
 *    Rev 1.0   21 Jul 1999 11:47:38   bacuff
 *  
**********************************************************************************************/


#ifndef _FWCOMPONENT_H_
#define _FWCOMPONENT_H_

#include <string>

namespace EMS_FW
{

class FWComponent
{
public:

	FWComponent(const std::string& className = "NotDefined", const std::string& name = "");
	void setName(const std::string& name);
	void setClassName(const std::string& className);
	const std::string& getName() const;
	const std::string& getClassName() const;

private:

	std::string _name; // Name of the Component
	std::string _className; // Name of the Class representing this Component.
};

};

#endif
