/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:48:16  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: ThreadPool
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   ThreadPool.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* Implementation of Thread Pool
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/ThreadPool.cpv  $
 
    Rev 1.5   Oct 27 1999 14:48:16   dtchalak
 Query Interface updates.
 
    Rev 1.4.1.0   Sep 20 1999 16:50:56   dtchalak
 Just to create a branch.
 
    Rev 1.4   02 Sep 1999 15:54:52   dtchalak
 New interface support added.
 
    Rev 1.3   21 Jul 1999 10:39:10   dtchalak
 Multiple Ne support included.
 
    Rev 1.2   10 Jun 1999 07:02:36   aloo
 Made ThreadPool class a singleton
 
    Rev 1.1   04 May 1999 19:16:46   dtchalak
 Comments added.
 
    Rev 1.0   05 Apr 1999 13:51:18   bacuff
  
 
    Rev 1.1   04 Feb 1999 15:17:24   dtchalak
 ready for IP-5
 
    Rev 1.0   22 Jan 1999 13:49:06   dtchalak
  
// 
//    Rev 1.1   Nov 09 1998 17:02:28   dtchalak
//  
// 
//    Rev 1.0   Nov 05 1998 11:52:34   dtchalak
//  
**********************************************************************************************/


// ThreadPool.cpp: implementation of the ThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "ThreadPool.h"

ThreadPool* ThreadPool::_instance = 0;

// The following class is used for the threadComplete callback mechanism.
// It is not used outside of the scope of this file.
class ThreadCompleteArgs
{
public:
    ThreadCompleteArgs( ThreadPool& thePool, ReusableThread* theThread);
    ThreadPool& pool;
    ReusableThread* thread;
};

ThreadCompleteArgs::ThreadCompleteArgs( ThreadPool& thePool, ReusableThread* theThread) :
        pool( thePool), thread( theThread)
{}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreadPool::ThreadPool( int size) :
        _maxThreadCount( size), _availableSemaphore( 0, size), _initialized( false)
{
    initialize();
}

ThreadPool::~ThreadPool()
{
    if (isInitialized())
        shutdown();
}

ThreadPool*
ThreadPool::instance(int size)
{
    if (!_instance)
    {
        _instance = new ThreadPool(size);
    }

    return _instance;
}

void
ThreadPool::initialize()
{
    PoolLock lock ( _poolMutex)
        ;
    for (int i = 0; i < _maxThreadCount; ++i)
    {
        _availableThreads.push_back( new ReusableThread());
        _availableSemaphore.post();
    }
    setInitialized( true);
}

void
ThreadPool::shutdown()
{
    // wait for available threads and delete them
    for (int i = 0; i < _maxThreadCount; ++i)
    {
        _availableSemaphore.obtain();
        {
            PoolLock lock (_poolMutex)
                ;
            ReusableThread* thread = _availableThreads.front();
            delete thread;
            _availableThreads.pop_front();
        }
    }
    setInitialized( false);
}

bool
ThreadPool::isInitialized()
{
    PoolLock lock ( _poolMutex)
        ;
    return _initialized;
}

void
ThreadPool::execute( ThreadPoolFunction function, void* arg)
{
    if (!isInitialized())
        throw ThreadPoolNotInitialized();

    _availableSemaphore.obtain(); // get a thread resource
    ReusableThread* nextThread = NULL;
    {  // set up synchronized block for updating lists

        PoolLock lock ( _poolMutex)
            ; // protect lists for access
        nextThread = _availableThreads.front();
        _availableThreads.pop_front();
        _activeThreads.push_back( nextThread);
    }

    nextThread->setFunction( function, arg);
    // setup callback arguments
    ThreadCompleteArgs *args = new ThreadCompleteArgs( *this, nextThread);
    nextThread->executeFunction( threadComplete, args);
    return ;
}


void
ThreadPool::setInitialized( bool init)
{
    PoolLock lock ( _poolMutex)
        ;
    _initialized = init;
}


void
ThreadPool::threadComplete( void* args)
{
    ThreadCompleteArgs* callbackArgs = (ThreadCompleteArgs*) args;

    callbackArgs->pool.reschedule( callbackArgs->thread);

    delete callbackArgs;
}

void
ThreadPool::reschedule( ReusableThread* completedThread)
{
    PoolLock lock ( _poolMutex)
        ;
    _activeThreads.remove( completedThread);
    _availableThreads.push_back( completedThread);
    _availableSemaphore.post();
}


ostream&
operator<<( ostream& stream, const ThreadPool& pool)
{
    return stream << "ThreadPool with "
           << pool._activeThreads.size() << " active threads and "
           << pool._availableThreads.size() << " available threads!";
}
