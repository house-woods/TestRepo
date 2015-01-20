/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:48:02  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: ReusableThread 
* Type: H  
* Originator: $Author:   dtchalak  $
* File: $Workfile:   ReusableThread.h  $
* Version: $Revision:   1.5  $
* Description: 
* ReusableThread is a stand-alone class which can execute many 
* functions on a separate thread.  
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/ReusableThread.h_v  $
 * 
 *    Rev 1.5   Oct 27 1999 14:48:02   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.4.1.0   Sep 20 1999 16:50:22   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.4   02 Sep 1999 15:52:12   dtchalak
 * New interface support added.
 * 
 *    Rev 1.3   21 Jul 1999 10:36:38   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.2   14 Jun 1999 16:26:36   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.1   04 May 1999 19:14:32   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 13:51:10   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 15:17:14   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 13:49:02   dtchalak
 *  
 * 
 *    Rev 1.1   Nov 09 1998 17:02:22   dtchalak
 *  
 * 
 *    Rev 1.0   Nov 05 1998 11:52:34   dtchalak
 *  
**********************************************************************************************/



// ReusableThread.h: interface for the ReusableThread class.
//
//	Written By:	JC Nichols	ObjectSpace, Inc.
//
//	Description:
//	ReusableThread is a stand-alone class which can execute many 
//	functions on a separate thread.  This class is used by ThreadPool;
//	however, it is not coupled to ThreadPool in any way.
//
//	The ObjectSpace Foundations<ToolKit> threading model is utilized.  The 
//	thread is an instance of osi_thread.  It requires a reference back to the 
//	ReusableThread for which it exists.  To avoid using the this pointer in 
//	the ReusableThread constructor, the thread is created lazily on the
//	first invocation of execute().
//
//	Usage:
//	1.  Ceate an instance of ReusableThread.
//	2.  Call setFunction() with a function to be run under the related thread.
//		Supply an optional (void*) argument to the function.
//	3.  Call executeFunction() to inform the thread to run the function above.
//		Supply an optional call back function and argument.  The call back function 
//		is called when the thread completes the specified function.
//
//////////////////////////////////////////////////////////////////////

#if !defined REUSABLE_THREAD_H
#define REUSABLE_THREAD_H

#include <ostream>
#include <ospace/thread.h>
#include "ace/Synch.h"

using namespace std;

typedef void* (*ReusableThreadFunction)( void *);
typedef void (*ReusableThreadCallback)(void *);

class ReusableThread  
{
public:
	// Construct an instance, but don't create the osi_thread.
	ReusableThread();	

	// Destruct--signal the thread to terminate and wait for it to die.
	virtual ~ReusableThread();	

	// Signal thread to terminate before next function.
	void terminate();	

	// Execute the previously specified function and call the optional call back when complete
	// Create the osi_thread on first invocation and maintain it there after.
	// throws InvalidFunction exception if function has not been set.
	void executeFunction( ReusableThreadCallback callback = NULL, void* callbackArg = NULL);

	// Specify which function to execute later along with related methods
	// for usability.
	void setFunction( ReusableThreadFunction function, void* arg = NULL);
	ReusableThreadFunction getFunction();
	void* getFunctionArg();
	void clearFunction();

	// Stream out a simple textual description of the ReusableThread and related osi_thread.
	friend ostream& operator<<( ostream& stream, const ReusableThread& thread);

private:
	// Definition for helper class on instance mutex.
	typedef osi_sem_lock< osi_fifo_mutex > ThreadLock;

	// Main function of control in related osi_thread.
	static void* mainThreadFunction( void*);

	// Internal methods used by executeFunction() to support call back mechanism.
	void setCallback( ReusableThreadCallback callback, void* arg);
	ReusableThreadCallback getCallback();
	void* getCallbackArg();

	osi_thread*	_myThread;				// associated thread
	osi_fifo_mutex	_threadMutex;		// instance mutex around ReusableThread

  ACE_Event _functionReady;
//	osi_event_semaphore _functionReady;	// event used to signal osi_thread to process function

	ReusableThreadFunction	_function;	// function to be processed by thread
	void* _functionArg;					// void* argument to function

	ReusableThreadCallback _callback;	// call back function to call when function completes
	void* _callbackArg;					// void* argument to call back

	bool _continue;						// used to terminate main thread loop
};


// The following class defines a possible exception which can be thrown by executeFunction.
class InvalidFunction {};

#endif // REUSABLE_THREAD_H
