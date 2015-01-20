

/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:58  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: EmsLock
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EmsLock.h  $
* Version: $Revision:   1.2  $
* Description: 
* Utility class that allow to wait for event specified time.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/EmsLock.hv_  $
 * 
 *    Rev 1.2   Oct 27 1999 14:46:58   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.1.1.0   Sep 20 1999 16:47:50   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.1   02 Sep 1999 15:50:56   dtchalak
 * New interface support added.
**********************************************************************************************/


#ifndef _EMSLOCK_H_
#define _EMSLOCK_H_

#include <string>

#include "ace/Synch.h"

namespace EMS_FW
{

class EmsLock
{
public:

    EmsLock();
    ~EmsLock();

    // Return true if exit because time expire
    bool wait(long waitTime);
    bool wait(long waitTime, std::string& returnPostText);

    void post(const std::string &textIn = "");

    void reset();

    std::string getPostText();

private:

    typedef ACE_Thread_Mutex Mutex_t;
    typedef ACE_Condition_Thread_Mutex Condition_t;
    typedef ACE_Guard<Mutex_t> Guard_t;

    // Disallow copying
    EmsLock(const EmsLock&)
    {}
    EmsLock& operator=(const EmsLock&)
    {
        return * this;
    }

    bool bIsPosted;
    std::string _postText;

    Mutex_t cond_mutex;
    Condition_t* pCond;
};
};

#endif
