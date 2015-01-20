/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:47:28  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Template Message Queue
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   MessageQueue.h  $
* Version: $Revision:   1.5  $
* Description: 
* Template Message Queue
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/MessageQueue.h_v  $
 * 
 *    Rev 1.5   Oct 27 1999 14:47:28   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.4.1.0   Sep 20 1999 16:49:00   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.4   02 Sep 1999 15:51:40   dtchalak
 * New interface support added.
 * 
 *    Rev 1.3   21 Jul 1999 10:35:58   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.2   14 Jun 1999 16:25:50   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.1   04 May 1999 19:14:00   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 13:50:50   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 15:16:50   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 13:48:54   dtchalak
 *  
 * 
 *    Rev 1.1   Nov 09 1998 17:00:10   dtchalak
 *  
 * 
 *    Rev 1.0   Oct 16 1998 17:47:24   dtchalak
 *  
**********************************************************************************************/


#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H


// SYNOPSIS:
// FIFO Queue with thread-safety for sending messages between threads.

// DESCRIPTION:
// This class contains a thread-safe FIFO queue to be utilized
// in sending requests from one thread to another.  It is implemented
// using a mutex to protect the queue and a counting semaphore to notify
// waiting threads.
//

#include <deque>
#include <queue>
#include <ospace/thread.h>
#include <ospace/thread/lock.h>

using namespace std;
template <class T>
class MessageQueue
{
public:
	MessageQueue( const unsigned& maxSize = 1);
	~MessageQueue() {}

	bool SendMessage( const T& message, const bool& waitOnQueue = false);
	T ReceiveMessage( void);

	unsigned QueueLength( void);

private:
	queue< T, deque< T > >	messages_;
	typedef osi_sem_lock<osi_mutex_semaphore> MessageLock;

	const unsigned			maxSize_;
	unsigned				currentSize_;
	osi_mutex				messageMutex_;
	osi_counting_semaphore	messageNotice_;

	const unsigned& GetMaxSize( void) const;
	const unsigned& GetCurrentSize( void) const;
	void BumpCurrentSize( void);
	void DropCurrentSize( void);
};


// MessageQueueOverflow is an exception thrown when `maxSize` is exceeded.
class MessageQueueOverflow {};





// Construct myself to be able to hold `maxSize` number of items
// (default is 1).

template <class T>
MessageQueue<T>::MessageQueue( const unsigned& maxSize /* = 1 */)
 :
		maxSize_(maxSize),
		currentSize_(0),
		messageMutex_(false), 
		messageNotice_( 0, maxSize )	// initial count set#endif
{}





// Return the number of items I contain.

template <class T>
inline unsigned 
MessageQueue<T>::QueueLength( void) 
{
	return messages_.size(); 
}



#ifdef _MSC_VER
#include "MessageQueue.C"
#endif

#endif

