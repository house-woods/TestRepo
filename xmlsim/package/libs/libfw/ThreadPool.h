/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:48:20  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Thread Pool
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   ThreadPool.h  $
* Version: $Revision:   1.5  $
* Description: 
* ThreadPool utilizes a list of ReusableThread instances to assign 
* functions to threads without incurring the overhead of instantiating 
* new threads every time.  The number of threads in the pool is limited
* based on the construction parameter <size>.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/ThreadPool.h_v  $
 * 
 *    Rev 1.5   Oct 27 1999 14:48:20   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.4.1.0   Sep 20 1999 16:51:08   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.4   02 Sep 1999 15:52:24   dtchalak
 * New interface support added.
 * 
 *    Rev 1.3   21 Jul 1999 10:36:52   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.2   10 Jun 1999 07:02:34   aloo
 * Made ThreadPool class a singleton
 * 
 *    Rev 1.1   04 May 1999 19:14:44   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 13:51:28   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 15:17:36   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 13:49:10   dtchalak
 *  
 * 
 *    Rev 1.1   Nov 09 1998 17:02:30   dtchalak
 *  
 * 
 *    Rev 1.0   Nov 05 1998 11:52:36   dtchalak
 *  
**********************************************************************************************/



// ThreadPool.h: interface for the ThreadPool class.
//
//	Written By:	JC Nichols	ObjectSpace, Inc.
//
//	Description:
//	ThreadPool utilizes a list of ReusableThread instances to assign 
//	functions to threads without incurring the overhead of instantiating 
//	new threads every time.  The number of threads in the pool is limited
//	based on the construction parameter <size>.
//
//	As described by ReusableThread, the underlying thread is not created
//	until the first invocation of the function.
//
//	The constructor calls initialize() and the destructor call shutdown()
//	automatically.  Yet these methods have been provided for convenience.
//
//	Usage:
//		1.	Create an instance of ThreadPool; it initializes by default.
//		2.	Call execute with the function to be assigned to an available
//			thread.  Provide any optional (void*) argument.
//
//////////////////////////////////////////////////////////////////////

#if !defined THREAD_POOL_H
#define THREAD_POOL_H

#include <ospace/std/list>

#include "ReusableThread.h"

typedef void* (*ThreadPoolFunction)(void*);

class ThreadPool  
{
public:

	static ThreadPool* instance(int size = 1);

	// Construct--initialize.
	ThreadPool( int size);

	// Destruct--shutdown.
	// The destructor is not virtual since no other methods are virtual.
	// If this class is modified later to support extensions, the destructor.
	// must be made virtual.
	~ThreadPool();


	// Configure the pool with the specified number of instances for ReusableThread.
	void initialize();

	// Complete all active threads and destroy thread pool.
	void shutdown();

	// Determine if pool is currently initialized.
	bool isInitialized();

	// Assign the specified function to an available thread in the pool.
	// If one is not available, suspend until one is freed.
	// Throws ThreadPoolNotInitialized if the pool has not been properly
	// initialized.  This should only happen if the pool is shutdown and 
	// a subsequent request is submitted.
	void execute( ThreadPoolFunction function, void* arg);

	// Stream out a simple textual description of the thread pool.
	friend ostream& operator<<( ostream& stream, const ThreadPool& pool);

private:

	static ThreadPool* _instance;

	// Definition for the list type for maintaining threads.
	typedef list< ReusableThread* > ReusableThreadList;
	
	// Definition for helper class on instance mutex.
	typedef osi_sem_lock< osi_fifo_mutex > PoolLock;

	// Mutator function on initialized state.
	void setInitialized( bool init);

	// Call back method invoked by ReusableThread on completion of each function.
	static void threadComplete( void* threadPtr);

	// Move specified thread from active list to available list.
	void reschedule( ReusableThread* thread);

	osi_fifo_mutex	_poolMutex;						// instance mutex around ThreadPool
	osi_counting_semaphore _availableSemaphore;		// resource semaphore for available threads

	int _maxThreadCount;						// specified size of pool
	bool _initialized;							// initialized status flag
	ReusableThreadList	_availableThreads;		// list of threads available for assignment
	ReusableThreadList	_activeThreads;			// list of threads currently processing 
};

// Class defined for exceptions when ThreadPool has not been properly initialized.
class ThreadPoolNotInitialized {};

#endif	// THREAD_POOL_H
