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
* Type: H 
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description: 
*	TL1EventResolver reads the config file (TL1Event
*	Type.cfg) and puts the entries in the map. It is a
*	singleton class.
*	This file maintains map of the TL1 REPT to the
*	equivalent CMIP Event Type.
*	for example:
*	ED-EQPT=CM_AVC_EVENT
*	TL1EventReceiver will call this object to resolve
*	the type of event.
* 
* Revision History:
*
**********************************************************************************************/
#ifndef TL1EventTypeResolver_h
#define TL1EventTypeResolver_h 1

#include "BaseNameResolver.h"

class TL1EventTypeResolver : public EMS_FW::BaseNameResolver  
{
 

  public:

      TL1EventTypeResolver();


      ~TL1EventTypeResolver();



      static TL1EventTypeResolver* instance ();

      
      static void registerResolver (TL1EventTypeResolver* resolver);

    

  protected:
    

  private: 
      static TL1EventTypeResolver* _instance;
      

};

#endif


