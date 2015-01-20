/***********************************************************************************************
* COPYRIGHT 1999 - 2007 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: Attribute Factory managed object header file 
* Type: Include File
* Originator: dtchalak
* File: MoAttributefactory.h
* Version: 
* Description: 
* Attribute Factory managed object related functions
*
* Revision History:
*
*
************************************************************************************************/

#ifndef _MO_ATTRIBUTE_FACTORY_
#define _MO_ATTRIBUTE_FACTORY_


#include "BaseTypesASN1.h"

class MoAttributeFactory
{
public:

    // constructor
    MoAttributeFactory()
    {}

    // destructor
    ~MoAttributeFactory()
    {}

    static MoAttributeFactory* instance()
    {
        if (_instance == 0)
            _instance = new MoAttributeFactory();
        return _instance;
    }

    MoValue *getMoValue(const string& name)
    {
        return Asn1AnyTable::GetTrueType(name);
    }

private:

    static MoAttributeFactory *_instance;
};

// MoAttributeFactory* MoAttributeFactory::_instance = NULL;

#endif

