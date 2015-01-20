/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 EventReceiver
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1EventReceiver.h  $
* Version: $Revision:   1.5  $
* Description: 
* Base abstract class to represent the event handling for the notifications and responses.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#ifndef _TL1EventReceiver_H_
#define _TL1EventReceiver_H_

#include "TL1Message.h"
#include "FWComponent.h"

namespace EMS_FW
{


class TL1EventReceiver : public FWComponent
{

public:

    TL1EventReceiver()
    { }

    virtual string getNeId()
    {
        return _neId;
    }

    void setNeId(const char* neId)
    {
        _neId = neId;
    }

    virtual bool handleEvent(TL1Message* event, const string& rawMessage = "")
    {
        return true;
    }

    virtual void handleResponse (TL1Message* event, const string& rawMessage = "")
    {}

    virtual void blockedAction ()
    {}

    virtual void unblockedAction ()
    {}

protected:

    string _neId;


};

};

#endif



