/***********************************************************************************************
* COPYRIGHT 2002  BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1NameResolver
* Type: cpp
* Originator: $Author: $
* File: $Workfile:   TL1NameResolver.cpp  $
* Version: $Revision:  $
* Description: 
* Name resolver specific for TL1
* 
* Revision History:
*
* $Log:  $
*  
**********************************************************************************************/


#include "TL1NameResolver.h"
#include "EMSDefines.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include "TL1Defines.h"

using namespace EMS_FW;


const string TL1NameResolverName = "TL1NameResolver";

TL1NameResolver::TL1NameResolver()
{
    _initialized = _initialize();
}

const string& TL1NameResolver::getClassName() const
{
    return TL1NameResolverName;
}

bool TL1NameResolver::_initialize()
{
    bool res = CmNameResolver::_initialize();
    _nameMap[TL1DEF::MOC_TL1MANAGEDELEMENT] = TL1DEF::MOC_TL1MANAGEDELEMENT;
    _nameMap[TL1DEF::MOC_TL1SHELF] = TL1DEF::MOC_TL1SHELF;
    _nameMap[TL1DEF::MOC_TL1EQUIPMENT] = TL1DEF::MOC_TL1EQUIPMENT;
    _nameMap[TL1DEF::MOC_TL1USER] = TL1DEF::MOC_TL1USER;

    string fName = Server_i::instance()->getDefaultConfigValue(ConfigNames::MapFileNameKey, "");
    if (fName.empty())
        return res;
    return BaseNameResolver::initialize(fName);
}
