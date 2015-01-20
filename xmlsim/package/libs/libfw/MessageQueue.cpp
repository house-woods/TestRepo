// MessageQueue source.

#include <iostream>

#ifdef MESSAGE_QUEUE_H
#include "MessageQueue.h"
#endif MESSAGE_QUEUE_H


// Lock my internal osi_queue (FIFO) and add `msg` to the end.
// Increment `messageNotice_` to inform other threads of the
// new message.
// throws: `MessageQueueOverflow`

template <class T>
bool
MessageQueue<T>::SendMessage( const T& msg, const bool& waitOnQueue)
{
	while (waitOnQueue && GetCurrentSize() >= GetMaxSize()) {
		osi_this_thread::sleep(1);
	}

	MessageLock lock( messageMutex_);

	try{
	   BumpCurrentSize();
	}catch (...){
	   return false;
	}
	messages_.push( msg);
	messageNotice_.post();
	return true;
}


// Wait for `messageNotice_` from other threads.
// Retrieve the next message from the front of the osi_queue.

template <class T>
T
MessageQueue<T>::ReceiveMessage( void) 
{
	messageNotice_.obtain();
	MessageLock lock( messageMutex_);
	T msg = messages_.front();
	messages_.pop();
	DropCurrentSize();
	return msg;
}


template <class T>
const unsigned&
MessageQueue<T>::GetMaxSize( void) const
{
	return maxSize_;
}


template <class T> 
const unsigned&
MessageQueue<T>::GetCurrentSize( void) const
{
	return currentSize_;
}


template <class T>
void
MessageQueue<T>::BumpCurrentSize( void) {
	if (currentSize_ >= GetMaxSize())
		throw MessageQueueOverflow();
	++currentSize_;
}


template <class T>
void
MessageQueue<T>::DropCurrentSize( void) {
  if (currentSize_ >0)
	--currentSize_;
}
