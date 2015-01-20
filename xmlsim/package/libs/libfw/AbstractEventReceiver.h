/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:00  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: AbstractEventReceiver.h 
* Type: Include File 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   AbstractEventReceiver.h  $
* Version: $Revision:   1.6  $
* Description: 
* An abstract class used by EventChannel to notify push consumers of events.
* A push consumer must implement a class inheriting from the AbstractEventReceiver
* and register it with the NsgEventChannel. A single process can implement
* use a signle receiver for multiple channels, or provide multiple, possibly different
* implementations of the receiver for use by different channels.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/AbstractEventReceiver.h_v  $
*
* Rev 1.7  Nov 29 2000 14:15:00 slisznia
* Added an event type exception.
* 
*    Rev 1.6   Oct 27 1999 14:46:00   dtchalak
* Query Interface updates.
* 
*    Rev 1.5.1.0   Sep 20 1999 16:45:08   dtchalak
* Just to create a branch.
* 
*    Rev 1.5   02 Sep 1999 15:49:50   dtchalak
* New interface support added.
* 
*    Rev 1.4   26 Jul 1999 11:14:30   aloo
* Updated to use OrbixNotification
* 
*    Rev 1.2   14 Jun 1999 16:24:24   dtchalak
* Updates related to new CM.idl and other details.
* 
*    Rev 1.1   04 May 1999 19:12:42   dtchalak
* Comments added.
* 
*    Rev 1.2   Dec 08 1998 10:10:26   dtchalak
* Resync with latest staff from CM.
* 
*    Rev 1.1   Nov 09 1998 17:03:06   dtchalak
*  
* 
*    Rev 1.0   Nov 02 1998 11:54:28   gfreiboth
*  
* 
*    Rev 1.1   Oct 09 1998 14:22:28   aloo
* Modified so it can be abstracted for all processes.
*
* File Dependencies: 
* 
************************************************************************************************/

#ifndef _AbstractEventReceiver_H__
#define _AbstractEventReceiver_H__

#include "EventConsumer.hh"

// Abstract base class for alarm receiver objects
class AbstractEventReceiver
{
public:
    AbstractEventReceiver()
    { }
    virtual ~AbstractEventReceiver()
    { }

    // inline definitions of operations.  Concrete receiver needs only to overwrite the necessary methods.
    virtual void processEvent (const CORBA::Any& event, EMS_Event::Event_Type type)
    {}
    ;
};

#endif
