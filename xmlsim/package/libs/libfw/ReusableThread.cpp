/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:47:58  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Reusable Thread 
* Type: C++ 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   ReusableThread.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* Implementation of Reusable Thread.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/ReusableThread.cpv  $
 
    Rev 1.5   Oct 27 1999 14:47:58   dtchalak
 Query Interface updates.
 
    Rev 1.4.1.0   Sep 20 1999 16:50:10   dtchalak
 Just to create a branch.
 
    Rev 1.4   02 Sep 1999 15:54:42   dtchalak
 New interface support added.
 
    Rev 1.3   21 Jul 1999 10:38:58   dtchalak
 Multiple Ne support included.
 
    Rev 1.2   14 Jun 1999 16:29:10   dtchalak
 Updates related to new CM.idl and other details.
 
    Rev 1.1   04 May 1999 19:16:36   dtchalak
 Comments added.
 
    Rev 1.0   05 Apr 1999 13:51:00   bacuff
  
 
    Rev 1.1   04 Feb 1999 15:17:02   dtchalak
 ready for IP-5
 
    Rev 1.0   22 Jan 1999 13:48:58   dtchalak
  
// 
//    Rev 1.1   Nov 09 1998 17:02:20   dtchalak
//  
// 
//    Rev 1.0   Nov 05 1998 11:52:32   dtchalak
//  
**********************************************************************************************/



// ReusableThread.cpp: implementation of the ReusableThread class.
//
//////////////////////////////////////////////////////////////////////

#include "ReusableThread.h"
#include "sysErrLog.h"

extern const char* sourceName;

using namespace ELogger;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReusableThread::ReusableThread() :
        _myThread( NULL), _continue( true), _functionReady(1),  //indicates manual reset required
        _function( NULL), _functionArg( NULL), _callback( NULL), _callbackArg( NULL)
{}

ReusableThread::~ReusableThread()
{
    terminate();
    if (_myThread != NULL)
    {
#ifdef WIN32
        osi_this_thread::wait_for_thread( *_myThread);
#elif EMSUNIX
        //  Cannot call wait_for_thread() on a detached thread,
        //  so this is the alternative.
        while ( _myThread->exists() )
        {
            osi_this_thread::milli_sleep( 200 );
        }
#endif
        delete _myThread;
    }
}



void*
ReusableThread::mainThreadFunction( void* rt)
{
    ReusableThread& self = *((ReusableThread*) rt);

    while ( self._continue)
    {
        try
        {
            self._functionReady.wait();
            if (!self._continue)
                break;  // terminating thread

            ThreadLock lock ( self._threadMutex)
                ;

            // execute thread function
            (self.getFunction())( self.getFunctionArg());
            // invoke callback if set
            (self.getCallback())( self.getCallbackArg());
            self.clearFunction();
            //self._functionReady.reset();
        }
        catch (...)
        {
            cerr << "ReusableThread exception..." << endl;
            ErrorLogger::logError(sourceName, "ReusableThread",
                                  "mainThreadFunction", __LINE__,
                                  CRITICAL, "Unknown exception TraceList will be dumped.");
            ErrorLogger::dumpTraceList();
            (self.getCallback())( self.getCallbackArg());
            self.clearFunction();
        }
        osi_this_thread::yield();
    }

    return 0;
}

void
ReusableThread::terminate()
{
    ThreadLock lock ( _threadMutex)
        ;
    _continue = false;
    _functionReady.signal();  // release to abort any pending function
}


void
ReusableThread::executeFunction( ReusableThreadCallback callback, void* callbackArg)
{
    // using lazy construction of thread due
    // problems using this in constructor
    if (_myThread == NULL)
    {
        ThreadLock lock ( _threadMutex)
            ;
#ifdef WIN32

        _myThread = new osi_thread( mainThreadFunction, this);
#elif EMSUNIX
        //  Need to create the [posix] thread in a detached state
        //  so that thread resources can be reclaimed when the
        //  thread terminates.
        _myThread = new osi_thread( mainThreadFunction, this, 0, true);
#endif

    }

    setCallback( callback, callbackArg);
    if (getFunction() == NULL)
        throw InvalidFunction();

    _functionReady.signal(); // allow main thread function to process
}

void
ReusableThread::setFunction( ReusableThreadFunction function, void* arg)
{
    ThreadLock lock ( _threadMutex)
        ;
    _function = function;
    _functionArg = arg;
}

ReusableThreadFunction
ReusableThread::getFunction()
{
    ThreadLock lock ( _threadMutex)
        ;
    return _function;
}

void*
ReusableThread::getFunctionArg()
{
    ThreadLock lock ( _threadMutex)
        ;
    return _functionArg;
}

void
ReusableThread::setCallback( ReusableThreadCallback callback, void* arg)
{
    ThreadLock lock ( _threadMutex)
        ;
    _callback = callback;
    _callbackArg = arg;
}

ReusableThreadCallback
ReusableThread::getCallback()
{
    ThreadLock lock ( _threadMutex)
        ;
    return _callback;
}

void*
ReusableThread::getCallbackArg()
{
    ThreadLock lock ( _threadMutex)
        ;
    return _callbackArg;
}

void
ReusableThread::clearFunction()
{
    ThreadLock lock ( _threadMutex)
        ;
    _function = NULL;
    _functionArg = NULL;
    _callback = NULL;
    _callbackArg = NULL;
    _functionReady.reset();
}

ostream&
operator<<( ostream& stream, const ReusableThread& thread)
{
    stream << "ReusableThread with TID: ";
    if (thread._myThread == NULL)
        stream << "NULL";
    else
        stream << thread._myThread->tid();
    return stream;
}
