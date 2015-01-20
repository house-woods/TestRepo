/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 LocRecoverHandler
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1LocRecoverHandler.h  $
* Version: $Revision:   1.5  $
* Description: 
*  Base class to handle LOC, NE recovery, Resync From Log,
* Continuety Break and Call SGM etc.
* This class should be derived by CM or FM specific Handler and 
* should be registered with the TL1Connection as well as TL1EventReceiver
* 
* Revision History:
* Rev 1.1  Feb 07, 2003  aaali
*  Modified to support multiple NEs
* Rev 1.0  Jan 21, 2003  lzou
*  Init Version
* $Log:  $
**********************************************************************************************/


#ifndef _TL1LocRecoverHandler_H_
#define _TL1LocRecoverHandler_H_
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

#include "BaseException.h"
#include "Server_i.h"
#include "FWComponent.h"
#include "MessageQueue.h"
#include "sysErrLog.h"
#include <ospace/thread.h>
#include <ospace/thread/rwsem.h>

namespace EMS_FW
{

class TL1LocRecoverHandler : public FWComponent
{

public:

    enum MessageType
    {
        MESSAGE_GENERATESWALARM  = 1,
        MESSAGE_CLEARSWALARM  = 2,
        MESSAGE_NOTIFYSGM = 3,
        MESSAGE_SEND_NODE_UPDATE = 4,
        MESSAGE_SHUTDOWN_THREAD = 999
    };

    struct SendCmd
    {
        MessageType messageType;
        string tid;
        string neId;
        string protocol;
        ELogger::ErrorCode code;
        string data;
    };

    TL1LocRecoverHandler();

    ~TL1LocRecoverHandler();

    //   TL1LocRecoverHandler(const char* neId = "");

    // This method will handle Loss of Communication
//    virtual void handleCommunicationLoss(const string &neId, const string &tid, const string &description);
    virtual void handleCommunicationLoss(SendCmd &cmd);

    // This method will handle NE recovery
    virtual void handleCommunicationRecovery(const string &neId, const string &tid, const string& protocol, const string &description);

    static TL1LocRecoverHandler *instance();

    void StartMessageThread (void);

    void StopMessageThread (void);

    bool SendMessage (const SendCmd& cmd, const bool& waitOnQueue = false);

    void ReceiveMessage (SendCmd &cmd);

    bool getShutdownFlag ()
    {
        return _shutdown;
    };

    virtual void handleRequest (SendCmd &cmd);

private:

    string _protocol;

    static TL1LocRecoverHandler * _instance;


    osi_thread* _sendThread;

    osi_read_write_semaphore _queueSemaphore;

    MessageQueue<SendCmd>* _cmdqueue;

    bool _shutdown;

};

};

#endif




