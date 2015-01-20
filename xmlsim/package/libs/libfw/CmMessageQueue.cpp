// MessageQueue source.

#include <iostream>
#include <list>

#include "CmMessageQueue.h"

#include "sysErrLog.h"

extern const char* sourceName;

using namespace ELogger;

CmMessageQueue::CmMessageQueue(int maxDelayThreshold, const unsigned& maxSize) :
        maxSize_(maxSize),
        currentSize_(0),
        maxReportSize_(maxSize),
        currentReportSize_(0),
        messageMutex_(false),
        messageNotice_( 0, maxSize ),
        _maxDelayThreshold(maxDelayThreshold),
        _ignoreQueueDelay(false)
{}

bool CmMessageQueue::SendMessage(BaseCmdInfo*msg, const bool& waitOnQueue)
{
    while (waitOnQueue && GetCurrentSize() >= GetMaxSize())
    {
        osi_this_thread::sleep(1);
    }

    MessageLock lock( messageMutex_);

    BumpCurrentSize();
    messages_.push( msg);
    messageNotice_.post();

    long oldestTimeStamp = messages_.front()->getTimeStamp();
    long currTimeStamp = msg->getTimeStamp();

    bool queueBackedUp = false;

    if (_ignoreQueueDelay == false &&
        0 != currTimeStamp &&
        0 != oldestTimeStamp &&
        (currTimeStamp - oldestTimeStamp) > _maxDelayThreshold)
    {
        queueBackedUp = true;

        ErrorLogger::logVarError(sourceName, "CmMessageQueue", "SendMessage", MINOR, "Detected a backup of %d seconds in the queue", (currTimeStamp - oldestTimeStamp));
    }

    return !queueBackedUp;
}

void CmMessageQueue::SendReportMessage(BaseCmdInfoList* msg, const bool& waitOnQueue)
{
    while (waitOnQueue && GetCurrentReportSize() >= GetMaxReportSize())
    {
        osi_this_thread::sleep(1);
    }

    MessageLock lock ( messageMutex_)
    ;

    BumpReportCurrentSize();
    messageReports_.push(msg);
    messageNotice_.post();
}

// Wait for `messageNotice_` from other threads.
// Retrieve the next message from the front of the osi_queue.

bool CmMessageQueue::ReceiveMessage(BaseCmdInfo*& msg, BaseCmdInfoList*& bciList)
{
    bool returnItem = false;
    messageNotice_.obtain();
    MessageLock lock(messageMutex_);

    if (QueueLength() > 0)
    {
        msg = messages_.front();
        messages_.pop();
        DropCurrentSize();
        returnItem = true;
    }
    else if (QueueReportLength() > 0)
    {
        bciList = messageReports_.front();
        messageReports_.pop();
        DropReportCurrentSize();
        returnItem = true;
    }

    return returnItem;
}


const unsigned&
CmMessageQueue::GetMaxSize( void) const
{
    return maxSize_;
}



const unsigned& CmMessageQueue::GetCurrentSize( void) const
{
    return currentSize_;
}


const unsigned&
CmMessageQueue::GetMaxReportSize( void) const
{
    return maxReportSize_;
}



const unsigned& CmMessageQueue::GetCurrentReportSize( void) const
{
    return currentReportSize_;
}



void
CmMessageQueue::BumpCurrentSize( void)
{
    if (currentSize_ >= GetMaxSize())
        throw CmMessageQueueOverflow();
    ++currentSize_;
}



void
CmMessageQueue::DropCurrentSize( void)
{
    if (currentSize_ > 0)
        --currentSize_;
}

void
CmMessageQueue::BumpReportCurrentSize( void)
{
    if (currentReportSize_ >= GetMaxSize())
        throw CmMessageQueueOverflow();
    ++currentReportSize_;
}



void
CmMessageQueue::DropReportCurrentSize( void)
{
    if (currentReportSize_ > 0)
        --currentReportSize_;
}


unsigned
CmMessageQueue::QueueLength( void)
{
    return messages_.size();
}

unsigned
CmMessageQueue::QueueReportLength( void)
{
    return messageReports_.size();
}

bool CmMessageQueue::flush (void)
{
    MessageLock lock ( messageMutex_)
    ;

    // try_lock returns false if no resources are available
    // so a deadlock cannot occur
    try
    {
        while (messageNotice_.try_lock())
        {
            if (QueueLength() > 0)
            {
                BaseCmdInfo* pMsg = messages_.front();
                delete pMsg;
                messages_.pop();
                DropCurrentSize();
            }
            else if (QueueReportLength() > 0)
            {
                BaseCmdInfoList* pList = messageReports_.front();
                const list<BaseCmdInfo*>& bciList = pList->getBaseCmdInfoList();
                list<BaseCmdInfo*>::const_iterator iter;

                for (iter = bciList.begin(); iter != bciList.end(); ++iter)
                {
                    delete (*iter);
                }
                delete pList;
                messageReports_.pop();
                DropReportCurrentSize();
            }
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

void CmMessageQueue::setMaxDelayThreshold(int seconds)
{
    _maxDelayThreshold = seconds;
}

bool CmMessageQueue::ignoreQueueDelay(bool ignore /* = true */)
{
    ErrorLogger::logError(sourceName, "CmMessageQueue", "ignoreQueueDelay", TRACE1, "Method Begin");

    bool oldVal = _ignoreQueueDelay;

    _ignoreQueueDelay = ignore;

    ErrorLogger::logError(sourceName, "CmMessageQueue", "ignoreQueueDelay", TRACE1, "Method End");

    return oldVal;
}
