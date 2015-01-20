/***********************************************************************************************
*
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1EventTypeResolver
* Type: CPP
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description: 
* TL1EventResolver reads the config file (TL1Event
* Type.cfg) and puts the entries in the map. It is a
* singleton class.
* This file maintains map of the TL1 REPT to the
* equivalent CMIP Event Type.
* for example:
* ED-EQPT=CM_AVC_EVENT
* TL1EventReceiver will call this object to resolve
* the type of event.
* 
* Revision History:
*
**********************************************************************************************/

#include "TL1EventTypeResolver.h"
#include "sysErrLog.h"
#include "BaseException.h"

using namespace EMS_FW;


TL1EventTypeResolver* TL1EventTypeResolver::_instance = NULL;


TL1EventTypeResolver::TL1EventTypeResolver()

{}


TL1EventTypeResolver::~TL1EventTypeResolver()
{}




TL1EventTypeResolver* TL1EventTypeResolver::instance ()
{

    if (_instance)
        return _instance;
    else
        throw BaseException("instance", "TL1EventTypeResolver", "Null TL1EventTypeResolver instance");

}

void TL1EventTypeResolver::registerResolver (TL1EventTypeResolver* resolver)
{

    if (_instance)
        delete _instance;
    _instance = resolver;

}
