#ifndef _NOTIFICATION_MANAGER_H_
#define _NOTIFICATION_MANAGER_H_

#include <string>
#include <list>
#include <map>

#include "ace/Singleton.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue_T.h"
#include "ace/System_Time.h"

#include "EventConsumer.hh"

#include "AbstractEventReceiver.h"

class NotificationManager_impl
{
public:

    // Supplier-Role methods
    bool addTarget(const std::string& sessionId, const std::string& consumerIOR);
    bool removeTarget(const std::string& sessionId);

    bool publishToTarget(CORBA::Any& eventBody, EMS_Event::Event_Type eventType, const std::string& sessionId);

    bool publishToTargetList(CORBA::Any& eventBody, EMS_Event::Event_Type eventType, const std::list<std::string>& sessionIdList);

    bool publishToAll(CORBA::Any& eventBody, EMS_Event::Event_Type eventType);

    // Consumer-Role methods
    void registerPushConsumer(AbstractEventReceiver* pReceiver);

    void push(const CORBA::Any& eventBody, EMS_Event::Event_Type eventType);

protected:

    struct NotificationDispatchInfo
    {
        NotificationDispatchInfo(const std::string& sessionId, const std::string& consumerIOR) :
                _thread_handle(0),
                _cleanupOnExit(false),
                _sessionId(sessionId),
                _consumerIOR(consumerIOR),
                _bThreadExited(false)
        {}

        ~NotificationDispatchInfo()
        {}

        ACE_thread_t _thread_handle;
        bool _cleanupOnExit;
        std::string _sessionId;
        std::string _consumerIOR;
        ACE_Message_Queue<ACE_MT_SYNCH> _msgQueue;
        bool _bThreadExited;
    };

    static void* publishEventTask(void *arg);

    typedef std::map<std::string, NotificationDispatchInfo*> SessionDetails;
    SessionDetails _sessionDetails;

    typedef std::list<AbstractEventReceiver*> ConsumerList;
    ConsumerList _consumerList;

private:

    NotificationManager_impl();
    ~NotificationManager_impl();

    NotificationManager_impl(const NotificationManager_impl& rhs)
    {}
    NotificationManager_impl& operator=(const NotificationManager_impl& rhs)
    {
        return * this;
    }

    // ToDo: Add ref counting to this class to avoid making a copy for each target
    //
    struct Event
    {
        Event(const CORBA::Any& eventBody, const EMS_Event::Event_Type& eventType);
        ~Event();

        void release()
        {
            delete this;
        }

        static unsigned _totalEvents;
        unsigned _myId;

        CORBA::Any _eventBody;
        EMS_Event::Event_Type _eventType;
    };

    void _addEventToQueue(Event* pEvent, ACE_Message_Queue<ACE_MT_SYNCH>& queue);

    ACE_RW_Thread_Mutex _targetLock;
    ACE_RW_Thread_Mutex _consumerLock;

    friend class ACE_Singleton<NotificationManager_impl, ACE_Thread_Mutex>;
};

typedef ACE_Singleton<NotificationManager_impl, ACE_Thread_Mutex> NotificationManager;

#endif //_NOTIFICATION_MANAGER_H_
