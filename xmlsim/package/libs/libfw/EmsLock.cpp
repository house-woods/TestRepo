

/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:54  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: EmsLock
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EmsLock.cpp  $
* Version: $Revision:   1.2  $
* Description: 
* Utility class that allow to wait for event specified time.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/EmsLock.cvp  $
   
      Rev 2.0   Mar 17 2005 12:46:54   hkhan1
   Reimplemented using ACE_Condition.
 
      Rev 1.2   Oct 27 1999 14:46:54   dtchalak
   Query Interface updates.
   
      Rev 1.1.1.0   Sep 20 1999 16:47:40   dtchalak
   Just to create a branch.
   
      Rev 1.1   02 Sep 1999 15:53:46   dtchalak
   New interface support added.
**********************************************************************************************/


#include "EmsLock.h"

#include <cstring>

#include "sysErrLog.h"

extern const char* sourceName;

using std::string;
using namespace EMS_FW;
using namespace ELogger;

static const char* className = (const char *)"EmsLock";

EmsLock::EmsLock() : pCond(new Condition_t(cond_mutex)), bIsPosted(false)
{}

EmsLock::~EmsLock()
{
    delete pCond;
}

string EmsLock::getPostText()
{
    return _postText;
}

void EmsLock::reset()
{
    _postText.erase();
    bIsPosted = false;
}

bool EmsLock::wait(long waitTime)
{
    return wait(waitTime, string());
}

bool EmsLock::wait(long waitTime, string& returnPostText)
{
    static const char* methodName = "wait";

    ErrorLogger::logError(sourceName, className, methodName, TRACE11, "Begin");

    ACE_Time_Value t(time(NULL) + int(waitTime / 1000L), (waitTime % 1000L)*1000L);

    Guard_t Guard(cond_mutex);

    bool bTimedOut = false;
    while (!bIsPosted && !bTimedOut)
    {
        if (pCond->wait(&t) == -1)  // if timed out, wait() returns -1 and sets errno == ETIME
        {
            if (errno == ETIME)
            {
                bTimedOut = true;
                ErrorLogger::logError(sourceName, className, methodName, TRACE11, "Timer expired");
            }
            else
            {
                ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Unexpected Error (%s), continuing to wait", strerror(errno));
            }
        }
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE11, "Posted with PostText = '%s'", _postText.c_str());
        }
    }

    returnPostText = _postText;
    bool retVal = !bIsPosted;  // Return true means timed out

    reset();

    ErrorLogger::logError(sourceName, className, methodName, TRACE11, "End");

    return retVal;
}

void EmsLock::post( const string &textIn )
{
    static const char* methodName = "post";

    ErrorLogger::logError(sourceName, className, methodName, TRACE11, "Begin");

    Guard_t Guard(cond_mutex);

    _postText = textIn;

    bIsPosted = true;

    ErrorLogger::logVarError(sourceName, className, methodName, TRACE11, "Posting. PostText = '%s'", textIn.c_str());

    pCond->signal(); // broadcast() ?

    ErrorLogger::logError(sourceName, className, methodName, TRACE11, "End");
}
