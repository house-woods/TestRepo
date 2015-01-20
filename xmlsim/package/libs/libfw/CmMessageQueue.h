

#ifndef CMMESSAGE_QUEUE_H
#define CMMESSAGE_QUEUE_H



#include <deque>
#include <queue>
#include <ospace/thread.h>
#include <ospace/thread/lock.h>

#include "Server_i.h"



#include "BaseCmdInfo.h"
#include "BaseException.h"



using namespace std;
using namespace EMS_FW;

//class EMS_FW::BaseCmdInfo;
//class EMS_FW::BaseCmdInfoList;


class CmMessageQueue
{
public:

    ACE_RW_Mutex _eventMutex; // used by the CmEventQueue

    CmMessageQueue(int maxTimespanInSeconds, const unsigned& maxSize = 1);
    ~CmMessageQueue()
    { }
    ;

    bool SendMessage(BaseCmdInfo* msg, const bool& waitOnQueue = false);
    void SendReportMessage(BaseCmdInfoList* msg, const bool& waitOnQueue = false);

    bool ReceiveMessage(BaseCmdInfo*& msg, BaseCmdInfoList*& list);
    unsigned QueueLength( void);
    unsigned QueueReportLength( void);
    bool flush (void);

    void setMaxDelayThreshold(int seconds);
    bool ignoreQueueDelay(bool ignore = true);

private:
    queue< BaseCmdInfo*, deque<BaseCmdInfo*> > messages_;
    queue< BaseCmdInfoList*, deque<BaseCmdInfoList*> > messageReports_;
    typedef osi_sem_lock<osi_mutex_semaphore> MessageLock;

    bool _ignoreQueueDelay;
    int _maxDelayThreshold;
    const unsigned maxSize_;
    unsigned currentSize_;

    const unsigned maxReportSize_;
    unsigned currentReportSize_;

    osi_mutex messageMutex_;
    osi_counting_semaphore messageNotice_;

    const unsigned& GetMaxSize( void) const;
    const unsigned& GetCurrentSize( void) const;
    void BumpCurrentSize( void);
    void DropCurrentSize( void);

    const unsigned& GetMaxReportSize( void) const;
    const unsigned& GetCurrentReportSize( void) const;
    void BumpReportCurrentSize( void);
    void DropReportCurrentSize( void);

};

#endif
