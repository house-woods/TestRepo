#ifndef _EMS_EVENT_SCHEDULER_H_
#define _EMS_EVENT_SCHEDULER_H_

#include <map>

#include "ace/Singleton.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_List.h"
#include "ace/RW_Thread_Mutex.h"

#include "FWComponent.h"

class EmsTaskScheduler_impl : protected ACE_Thread_Timer_Queue_Adapter<ACE_Timer_List>
{
public:

    typedef int (*TaskHandler)(long taskId, const void *taskData);

    // Start a new task with the given taskId to be performed with the provided schedule
    // do not provide last parameter if this task is not a recurring type
    void scheduleNewTask(long taskId, void* taskData, TaskHandler taskHandler, const ACE_Time_Value& firstRunTime, const ACE_Time_Value& recurrenceInterval = ACE_Time_Value::zero);

    // Causes the task to be performed with a different schedule
    bool rescheduleTask(long taskId, const ACE_Time_Value& nextRunTime, const ACE_Time_Value& recurrenceInterval = ACE_Time_Value::zero);

    // Cancels all future events for this task
    void cancelScheduledTask(long taskId);
    
    // Activates our scheduler `active object'
    void start();

    // deactivates our active object
    void stop();

protected:

    // Inner class to handle timer events
    class ScheduledTaskHandler : public ACE_Event_Handler
    {
    public:
        ScheduledTaskHandler(EmsTaskScheduler_impl* scheduler) : _pScheduler(scheduler)
        {
        }

        int handle_timeout(const ACE_Time_Value &current_time,
                                   const void *act = 0);

        EmsTaskScheduler_impl* _pScheduler;
    };
    
    friend class ScheduledTaskHandler;
    
    typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_List> BaseClass;
    
    struct UserTaskDetails
    {
        UserTaskDetails() : _callBackFunc(0), _taskData(0)
        {
            // Need a default constructor to store in std::map as value
        }
        
        UserTaskDetails(TaskHandler callBackFunc, void* taskData) : _callBackFunc(callBackFunc), _taskData(taskData)
        {
            // Nothing to do here
        }

        TaskHandler _callBackFunc;
        void* _taskData;
    };

    ScheduledTaskHandler* _pHandler;

    std::map<long, long> TaskIdToTimerId;
    std::map<long, struct UserTaskDetails> taskDetails;

private:

    EmsTaskScheduler_impl();
    ~EmsTaskScheduler_impl();

    EmsTaskScheduler_impl(const EmsTaskScheduler_impl& rhs) {}
    EmsTaskScheduler_impl& operator=(const EmsTaskScheduler_impl& rhs) { return *this; }

    ACE_RW_Thread_Mutex _taskDetailsLock;

    friend class ACE_Singleton<EmsTaskScheduler_impl, ACE_Null_Mutex>;
};

typedef ACE_Singleton<EmsTaskScheduler_impl, ACE_Null_Mutex> EmsTaskScheduler;

#endif
