

/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:47:12  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: FWComponent
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   FWComponent.cpp  $
* Version: $Revision:   1.2  $
* Description: 
* Base Framework Component.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/FWComponent.cvp  $
   
      Rev 1.2   Oct 27 1999 14:47:12   dtchalak
   Query Interface updates.
   
      Rev 1.1.1.0   Sep 20 1999 16:48:26   dtchalak
   Just to create a branch.
   
      Rev 1.1   02 Sep 1999 15:53:58   dtchalak
   New interface support added.
   
      Rev 1.0   21 Jul 1999 11:48:28   bacuff
    
**********************************************************************************************/


#include "FWComponent.h"

using namespace EMS_FW;
using std::string;

FWComponent::FWComponent(const string& className, const string& name) :
        _name(name), _className(className)
{}


void FWComponent::setName(const string& name)
{
    _name = name;
}

const string& FWComponent::getName() const
{
    return _name;
}

void FWComponent::setClassName(const string& className)
{
    _className = className;
    if (_name.size() == 0)
        _name = className;  // using as a default setting
}

const string& FWComponent::getClassName() const
{
    return _className;
}

