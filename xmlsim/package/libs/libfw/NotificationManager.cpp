#include "NotificationManager.h"

#include "sysErrLog.h"
#include "BaseException.h"
#include "ComManager.h"

using namespace ELogger;

extern const char* sourceName;
static const char* className = "NotificationManager";

static size_t DISPATCH_QUEUE_HWM = 5000;
static size_t DISPATCH_QUEUE_LWM = 4999;

unsigned NotificationManager_impl::Event::_totalEvents = 0;

NotificationManager_impl::Event::Event(const CORBA::Any& eventBody, const EMS_Event::Event_Type& eventType) : _eventBody(eventBody), _eventType(eventType), _myId(_totalEvents++)
{
    if (_totalEvents%1000 == 0) // Log every 1000th event
    {
        ErrorLogger::logVarError(sourceName, className, "Event::Event", __LINE__, TRACE1, "Total events so far %u", _totalEvents);
    }
}

NotificationManager_impl::Event::~Event()
{
}

void* NotificationManager_impl::publishEventTask(void *arg)
{
    const char* methodName = "publishEventTask";

    NotificationDispatchInfo* pDispatchInfo = ((NotificationDispatchInfo*)arg);
    const char* c_strSessionId = pDispatchInfo->_sessionId.c_str();

    try
    {
        struct cancel_state cancelState;
        ACE_Thread::enablecancel(&cancelState, 1); //CANCEL_ON

        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Getting Consumer Object Reference for session ID %s", c_strSessionId);

        CORBA::Object_var objVar = ComManager::instance()->getOrb()->string_to_object(pDispatchInfo->_consumerIOR.c_str());
        CORBA::Object_var pTimeOutObjVar = ComManager::instance()->SetTimeOut(objVar, 10000);

        // Hammad: Need to investigate why _narrow() times out on client sessions sometimes
        EMS_Event::Consumer_var pConsumer = EMS_Event::Consumer::_nil();
        for (unsigned retriesLeft = 5; CORBA::is_nil(pConsumer) && --retriesLeft;)
        {
            try
            {
                pConsumer = EMS_Event::Consumer::_narrow(pTimeOutObjVar);
            }
            catch (CORBA::SystemException& se)
            {
                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                         "CORBA System Exception during narrow of Consumer Reference for session %s: %s", c_strSessionId, (se._info()).c_str());
                ACE_OS::sleep(5);
            }
        }

        if (CORBA::is_nil(pConsumer))
        {
            throw EMS_FW::BaseException ("Unable to narrow the Consumer Object Reference.");
        }

        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Notification Dispatch thread started for session ID %s", c_strSessionId);

        ACE_Message_Block* pMsgBlock = 0;
        ACE_Message_Queue<ACE_MT_SYNCH>& queue = pDispatchInfo->_msgQueue;
        while (queue.dequeue_head(pMsgBlock) != -1) // Remove one event from queue to publish to this consumer
        {
            if (pMsgBlock->size() == 0 && pMsgBlock->msg_type() == ACE_Message_Block::MB_STOP)
            {
                pMsgBlock->release();

                size_t remaining = queue.message_count();
                if (remaining)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, 
                                             "Cleaning up %d events in the Notification Dispatch Queue for session ID %s",
                                             remaining, c_strSessionId);

                    while (!queue.is_empty())
                    {
                        queue.dequeue_head(pMsgBlock);
                        auto_ptr<Event> pEvent((Event*)(pMsgBlock->base()));
                        pMsgBlock->release();
                    }
                }

                queue.deactivate();
            }
            else
            {
                try
                {
                    size_t remaining = queue.message_count();
                    if (remaining > 100)  // Perhaps there's a problem
                        ErrorLogger::logVarError(sourceName, className, methodName, TRACE2, "Removed an event from dispatch queue for session %s. %d events remaining in queue", c_strSessionId, remaining);
                    else  // Probably running normally
                        ErrorLogger::logVarError(sourceName, className, methodName, TRACE7, "Removed an event from dispatch queue for session %s. %d events remaining in queue", c_strSessionId, remaining);

                    auto_ptr<Event> pEvent((Event*)(pMsgBlock->base()));

                    CORBA::Any& eventBody = pEvent->_eventBody;
                    pConsumer->push(eventBody, pEvent->_eventType);
                }
                catch (CORBA::Exception& ex)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                             "CORBA Exception while publishing to session ID %s, Ignoring...: %s", c_strSessionId, (ex._info()).c_str());
                }
                catch (...)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                             "Unknown Exception while publishing to session ID %s, Ignoring...", c_strSessionId);
                }
                pMsgBlock->release();
                pMsgBlock = 0;
            }
        }
    }
    catch (CORBA::Exception& se)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "CORBA Exception: %s", (se._info()).c_str());
    }
    catch (EMS_FW::BaseException& ex )
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "FW Base Exception: %s", ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR,
                              "Unknown exception.");
    }

    if (pDispatchInfo->_cleanupOnExit)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR, "Notification Dispatch thread for session ID %s has been cancelled", c_strSessionId);
        delete pDispatchInfo;
    }
    else
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Notification Dispatch thread stopped for session ID %s", c_strSessionId);
        pDispatchInfo->_bThreadExited = true;
    }

    return 0;
}

NotificationManager_impl::NotificationManager_impl()
{}

NotificationManager_impl::~NotificationManager_impl()
{}

bool NotificationManager_impl::addTarget(const std::string& sessionId, const string& consumerIOR)
{
    static const char* methodName = "addTarget";

    bool bRetVal = false;

    const char* c_strSessionId = sessionId.c_str();

    ErrorLogger::logVarError( sourceName, className, methodName, TRACE3,
                              "Begin for session id [%s].", c_strSessionId);

    try
    {
        ACE_Write_Guard<ACE_RW_Thread_Mutex> guard(_targetLock);

        SessionDetails::iterator it = _sessionDetails.find(sessionId);
        if (it != _sessionDetails.end() && it->second->_consumerIOR == consumerIOR)
        {
            ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE1,
                                      "A thread already exists for this session id [%s]. Ignoring call", c_strSessionId);
        }
        else
        {
            if (it != _sessionDetails.end()) // If the session Id is registered, but with a different IOR
            {
                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                                         "A thread already exists for this session id [%s] but with a different IOR. Removing previous entry.", c_strSessionId);

                guard.release();
                removeTarget(sessionId); // remove it from the list
                guard.acquire_write();
            }

            // Spawn a new thread to handle publishing for this session
            NotificationDispatchInfo* pDispatchInfo = new NotificationDispatchInfo(sessionId, consumerIOR);

            pDispatchInfo->_msgQueue.high_water_mark(DISPATCH_QUEUE_HWM);
            pDispatchInfo->_msgQueue.low_water_mark(DISPATCH_QUEUE_LWM);

            ACE_thread_t threadHandle;

            ACE_Thread_Manager::instance()->spawn(publishEventTask, pDispatchInfo, THR_DETACHED | THR_SUSPENDED, &threadHandle);
            pDispatchInfo->_thread_handle = threadHandle;
            ACE_Thread_Manager::instance()->resume(threadHandle);

            _sessionDetails[sessionId] = pDispatchInfo;

            ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE1,
                                      "New session [%s] added. Publish thread started", c_strSessionId);
            bRetVal = true;
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, MINOR,
                                  "Caught unknown exception adding session id [%s]. Ignoring call", c_strSessionId);
    }

    ErrorLogger::logVarError( sourceName, className, methodName, TRACE3,
                              "End for session id [%s].", c_strSessionId);

    return bRetVal;
}

bool NotificationManager_impl::removeTarget(const std::string& sessionId)
{
    static const char* methodName = "removeTarget";

    bool bRetVal = false;

    const char* c_strSessionId = sessionId.c_str();

    ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE1,
                              "Begin for session id [%s].", c_strSessionId);

    try
    {
        NotificationDispatchInfo* pDispatchInfo = 0;

        {
            // Setup local scope so that we can release the mutex while we wait for the thread to terminate
            ACE_Write_Guard<ACE_RW_Thread_Mutex> guard(_targetLock);
            SessionDetails::iterator it = _sessionDetails.find(sessionId);
            if (it == _sessionDetails.end())
            {
                ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE1,
                                          "No thread found for session id [%s].", c_strSessionId);
            }
            else
            {
                pDispatchInfo = it->second;
                _sessionDetails.erase(it);
            }
        }

        if (pDispatchInfo)
        {
            ACE_Time_Value timeout = ACE_OS::gettimeofday();
            timeout += 60; // seconds
            pDispatchInfo->_msgQueue.enqueue_head(new ACE_Message_Block(0, ACE_Message_Block::MB_STOP), &timeout);

            int retriesLeft = 10;
            do
            {
                ACE_OS::sleep(1);
            }
            while (!pDispatchInfo->_bThreadExited && --retriesLeft);

            if (!pDispatchInfo->_bThreadExited)
            {
                // If the publish thread is not responsive, do not delete dispatchInfo here. The thread will detect the cancel request and clean it up
                pDispatchInfo->_cleanupOnExit = true;

                ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE1,
                                          "Notification Dispatch thread for session id [%s] was unresponsive, flagged for future shutdown.", c_strSessionId);
            }
            else
            {
                ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE1,
                                          "Notification Dispatch thread for session id [%s] closed successfully.", c_strSessionId);
                delete pDispatchInfo;
                bRetVal = true;
            }
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, MAJOR,
                                  "Unknown exception while terminating Notification Dispatch thread for session id [%s]", c_strSessionId);
    }

    ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, TRACE3,
                              "End for session id [%s].", c_strSessionId);

    return bRetVal;
}

bool NotificationManager_impl::publishToTarget(CORBA::Any& eventBody, EMS_Event::Event_Type eventType, const std::string& sessionId)
{
    const char* methodName = "publishToTarget";

    bool bRetVal = false;

    const char* c_strSessionId = sessionId.c_str();

    ErrorLogger::logVarError( sourceName, className, methodName, TRACE3,
                              "Begin for session id [%s].", c_strSessionId);

    try
    {
        ACE_Read_Guard<ACE_RW_Thread_Mutex> guard(_targetLock);

        SessionDetails::iterator it = _sessionDetails.find(sessionId);
        if (it == _sessionDetails.end())
        {
            ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, MINOR,
                                      "No thread found for session id [%s].", c_strSessionId);
        }
        else
        {
            Event* pEvent = new Event(eventBody, eventType);
            _addEventToQueue(pEvent, it->second->_msgQueue);
            bRetVal = true;
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown exception caught while publishing Event to session %s. Ignoring...", c_strSessionId);
    }

    ErrorLogger::logVarError( sourceName, className, methodName, TRACE3,
                              "End for session id [%s].", c_strSessionId);

    return bRetVal;
}

bool NotificationManager_impl::publishToTargetList(CORBA::Any& eventBody, EMS_Event::Event_Type eventType, const std::list<std::string>& sessionIdList)
{
    const char* methodName = "publishToTargetList";

    bool bRetVal = true;

    ErrorLogger::logError( sourceName, className, methodName, TRACE3, "Begin.");

    ACE_Read_Guard<ACE_RW_Thread_Mutex> guard(_targetLock);

    for (std::list<std::string>::const_iterator it = sessionIdList.begin(); it != sessionIdList.end(); it++)
    {
        const string& sessionId = *it;
        try
        {
            SessionDetails::iterator it2 = _sessionDetails.find(sessionId);
            if (it2 == _sessionDetails.end())
            {
                ErrorLogger::logVarError( sourceName, className, methodName, __LINE__, MINOR,
                                          "No thread found for session id [%s]. Skipping this session", sessionId.c_str());
                bRetVal = false; // false means not all publishes were successful.
            }
            else
            {
                Event* pEvent = new Event(eventBody, eventType);
                _addEventToQueue(pEvent, it2->second->_msgQueue);
            }
        }
        catch (...)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                     "Unknown exception caught while publishing Event to session [%s]. Ignoring...", sessionId.c_str());
            bRetVal = false; // false means not all publishes were successful.
        }
    }

    ErrorLogger::logVarError( sourceName, className, methodName, TRACE3, "End");

    return bRetVal;
}

bool NotificationManager_impl::publishToAll(CORBA::Any& eventBody, EMS_Event::Event_Type eventType)
{
    const char* methodName = "publishToAll";

    bool bRetVal = false;

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE3,
                          "Begin");

    try
    {
        ACE_Read_Guard<ACE_RW_Thread_Mutex> guard(_targetLock);

        for (SessionDetails::iterator it = _sessionDetails.begin(); it != _sessionDetails.end(); it++)
        {
            Event* pEvent = new Event(eventBody, eventType); // Move this outside of loop when ref counting is implemented for the Event class
            _addEventToQueue(pEvent, it->second->_msgQueue);
        }

        bRetVal = true;
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR,
                              "Unknown exception caught while publishing Events to All. Ignoring..." );
    }

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE3,
                          "End");

    return bRetVal;
}

void NotificationManager_impl::_addEventToQueue(Event* pEvent, ACE_Message_Queue<ACE_MT_SYNCH>& queue)
{
    const char* methodName = "_addEventToQueue";

    try
    {
        ACE_Message_Block* pMsgBlock = new ACE_Message_Block((char*)pEvent, 1);

        // Enqueue to the list with a timeout
        ACE_Time_Value now;
        ACE_System_Time::get_local_system_time(now);

        ACE_Time_Value timeout = now + ACE_Time_Value(0, 100000); // 100 milliseconds

        if ( -1 == queue.enqueue_tail(pMsgBlock, &timeout))
        {
            if (errno == EWOULDBLOCK)
            {
                ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR,
                                      "Timeout in dispatching to event queue. Event dropped");
            }
            else
            {
                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                         "Error (%d) while dispatching to event queue.", errno);
            }

            delete pEvent; // will delete the inner CORBA::Any object
            pMsgBlock->release();   // will decrement the reference count and delete the instance if it becomes zero
        }
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR,
                              "Unknown exception caught while adding event to MsgQueue. Ignoring..." );
    }
}

void NotificationManager_impl::registerPushConsumer(AbstractEventReceiver* pReceiver)
{
    ACE_Write_Guard<ACE_RW_Thread_Mutex> guard(_consumerLock);

    ConsumerList::const_iterator it = find(_consumerList.begin(), _consumerList.end(), pReceiver);
    if (it == _consumerList.end())
    {
        _consumerList.push_back(pReceiver);
    }
}

void NotificationManager_impl::push(const CORBA::Any& eventBody, EMS_Event::Event_Type eventType)
{
    ACE_Read_Guard<ACE_RW_Thread_Mutex> guard(_consumerLock);

    for (ConsumerList::const_iterator it = _consumerList.begin(); it != _consumerList.end(); it++)
    {
        (*it)->processEvent(eventBody, eventType);
    }
}
