#ifndef __SCHEDULER__
#define __SCHEDULER__

#include <ospace/thread.h>
#include "ConfigManager.h"
using namespace std;
class AutonomousScheduler
{
public:

    ~AutonomousScheduler();
    static AutonomousScheduler* getInstance();

    static void* scheduleLoop(void*);

    bool firstInitialize();
    bool initialize(list<string> &atids);
    bool finialize(void);
    bool start(list<string> &atids);
    bool stop(void);
    bool onDemandRequest(string neTid, string tid);


private:

    static list<int> m_schedulePeriod;
    static AutonomousScheduler* m_autonomous;
    static osi_thread* _scheduleThread;
    static ScheduleQueue m_scheduleQueue;
    static ScheduleQueue onDemandQueue;
    static ScheduleQueue::iterator iter;

protected:
    bool compileResponse(ScheduleEntry *entry);
    AutonomousScheduler();
};

#endif
