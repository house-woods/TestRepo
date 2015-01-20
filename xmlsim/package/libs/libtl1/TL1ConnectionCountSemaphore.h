#ifndef _CONNECTIONCOUNTSEMAPHORE_H_
#define _CONNECTIONCOUNTSEMAPHORE_H_

#include <ospace/thread.h>
#include <ospace/thread/mutex.h>
#include "ace/Synch.h"

namespace EMS_FW
{
typedef osi_sem_lock<osi_mutex_semaphore> MessageLock;

class TL1ConnectionCountSemaphore
{
public:
    TL1ConnectionCountSemaphore ();
    TL1ConnectionCountSemaphore (int min, int max);
    ~TL1ConnectionCountSemaphore();

    void wait();
    int post();
    int reset();
    bool posted();

    void setMin(int min);
    void setMax(int max);

private:
    unsigned int _minNumberOfResources;
    unsigned int _maxNumberOfResources;
    unsigned int _numberOfResources;

    osi_mutex _resourceMutex;
    bool _postState;
    ACE_Manual_Event _notify;
};
};

#endif
