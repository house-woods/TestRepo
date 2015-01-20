#include "ace/Guard_T.h"

#include "EmsTaskScheduler.h"

using std::map;

EmsTaskScheduler_impl::EmsTaskScheduler_impl() : _pHandler(new ScheduledTaskHandler(this))
{
}

EmsTaskScheduler_impl::~EmsTaskScheduler_impl()
{
    delete _pHandler;
}

void EmsTaskScheduler_impl::scheduleNewTask(long taskId,
                                       void* taskData,
                                       EmsTaskScheduler_impl::TaskHandler callbackHandler,
                                       const ACE_Time_Value& firstRunTime,
                                       const ACE_Time_Value& recurrenceInterval)
{
    long id = BaseClass::schedule(_pHandler, (void*)taskId, firstRunTime, recurrenceInterval);

    {
        ACE_Write_Guard<ACE_RW_Thread_Mutex> guard(_taskDetailsLock);
        TaskIdToTimerId[taskId] = id;
        taskDetails[taskId]     = UserTaskDetails(callbackHandler, taskData);
    }
}

bool EmsTaskScheduler_impl::rescheduleTask(long taskId,
                                      const ACE_Time_Value& nextRunTime,
                                      const ACE_Time_Value& recurrenceInterval)
{
    bool bTaskFound = false;

    TaskHandler callBackFunc = 0;
    void*       taskData     = 0;

    {
        ACE_Read_Guard<ACE_RW_Thread_Mutex> guard(_taskDetailsLock);
        map<long, struct UserTaskDetails>::const_iterator it = taskDetails.find(taskId);
        if (it != taskDetails.end())
        {
            bTaskFound = true;

            callBackFunc = (*it).second._callBackFunc;
            taskData     = (*it).second._taskData;
        }
    }

    if (callBackFunc)
    {
        cancelScheduledTask(taskId);
        scheduleNewTask(taskId, taskData, callBackFunc, nextRunTime, recurrenceInterval);
    }

    return bTaskFound;
}

void EmsTaskScheduler_impl::cancelScheduledTask(long taskId)
{
    ACE_Write_Guard<ACE_RW_Thread_Mutex> guard(_taskDetailsLock);

    map<long, long>::iterator it = TaskIdToTimerId.find(taskId);
    if (it != TaskIdToTimerId.end())
    {
        long TimerId = (*it).second;

        BaseClass::cancel(TimerId);

        TaskIdToTimerId.erase(it);
        taskDetails.erase(taskId);
    }
}

void EmsTaskScheduler_impl::start()
{
    BaseClass::activate();
}

void EmsTaskScheduler_impl::stop()
{
    BaseClass::deactivate();
}

int EmsTaskScheduler_impl::ScheduledTaskHandler::handle_timeout (const ACE_Time_Value &current_time,
                                                            const void *act)
{
    long taskId = reinterpret_cast<long>(act);
    TaskHandler callBackFunc = 0;
    void*        taskData    = 0;

    {
        ACE_Read_Guard<ACE_RW_Thread_Mutex> guard(_pScheduler->_taskDetailsLock);
        map<long, struct UserTaskDetails>::const_iterator it = _pScheduler->taskDetails.find(taskId);
        if (it != _pScheduler->taskDetails.end())
        {
            callBackFunc = (*it).second._callBackFunc;
            taskData     = (*it).second._taskData;
        }
    }

    return callBackFunc?callBackFunc(taskId, taskData):-1;
}
