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
* File: $Workfile:   TL1LocRecoverHandler.cpp  $
* Version: $Revision:   1.5  $
* Description: 
*  Base class to handle LOC, NE recovery, Resync From Log,
* Continuety Break and Call SGM etc.
* This class should be derived by CM or FM specific Handler and 
* should be registered with the TL1Connection as well as TL1EventReceiver
* 
* Revision History:
* Rev 1.0  Jan 21, 2003  lzou
*  Init Version
* $Log:  $
**********************************************************************************************/
#include "TL1LocRecoverHandler.h"
#include "BaseCmdFactory.h"
#include "BaseException.h"
#include "Server_i.h"
#include <ospace/thread.h>
#include "ConfigNames.h"

extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;
using namespace SysMon;


TL1LocRecoverHandler* TL1LocRecoverHandler::_instance = NULL;

TL1LocRecoverHandler* TL1LocRecoverHandler::instance ()
{
    if (_instance == NULL)
    {
        _instance = new TL1LocRecoverHandler();
    }
    return _instance;
}

void* procFun(void* arg)
{
    TL1LocRecoverHandler* handler = (TL1LocRecoverHandler*) arg;
    string servername = Server_i::instance()->getServerName ();
    TL1LocRecoverHandler::SendCmd cmd;

    ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "procFunc", TRACE1, "Starting loop to process messages");
    while ( handler->getShutdownFlag() == false)
    {
        handler->ReceiveMessage (cmd);
        if (cmd.messageType == TL1LocRecoverHandler::MESSAGE_SHUTDOWN_THREAD)
        {
            ErrorLogger::logVarError(sourceName, "TL1LocRecoverHandler", "procFunc", __LINE__, TRACE1, "Received shutdown message");
            break;
        }
        ErrorLogger::logVarError(sourceName, "TL1LocRecoverHandler", "procFunc", TRACE1, "Received a message of type [%d]; processing it", cmd.messageType);
        handler->handleRequest (cmd);
    }
    ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "procFunc", TRACE1, "Exiting message processing loop; thread is shutting down");
    return 0;
}

TL1LocRecoverHandler::TL1LocRecoverHandler(void) :
    _shutdown(false),
    _cmdqueue(new MessageQueue<SendCmd>(9999)),
    _sendThread(NULL)
{
}

void TL1LocRecoverHandler::StartMessageThread (void)
{
    ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "TL1LocRecoverHandler", TRACE1, "Trying to start the recovery handler thread");
    try
    {
        _sendThread = new osi_thread(procFun, (void*) this, 0, false);
        ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "TL1LocRecoverHandler", TRACE1, "Started the recovery handler thread");
    }
    catch (osi_thread_toolkit_error& err)
    {
        ErrorLogger::logVarError(sourceName, "TL1LocRecoverHandler", "TL1LocRecoverHandler", __LINE__, CRITICAL, "run()->%s", err.description(err.code()));
    }
}


void TL1LocRecoverHandler::StopMessageThread (void)
{
    ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "TL1LocRecoverHandler", TRACE1, "Trying to stop the recovery handler thread");
    _shutdown = true;
    // send a message to the thread so it can be awakened from getmessage
    SendCmd cmd;
    cmd.messageType = MESSAGE_SHUTDOWN_THREAD;
    SendMessage (cmd);
    osi_this_thread::wait_for_thread(*_sendThread);
    ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "TL1LocRecoverHandler", TRACE1, "Stopped the recovery handler thread");
}

TL1LocRecoverHandler::~TL1LocRecoverHandler()
{
    if (_sendThread != NULL)
    {
        delete _sendThread;
        _sendThread = NULL;
    }
    delete _cmdqueue;
}

void TL1LocRecoverHandler::ReceiveMessage (SendCmd& cmd)
{
    // no need to synchronize this as only one thread will do this
    cmd = _cmdqueue->ReceiveMessage();
    ErrorLogger::logVarError(sourceName, "TL1LocRecoverHandler", "ReceiveMessage", TRACE2,
                          "Current Msg Queue Size: %d", _cmdqueue->QueueLength());
}

bool TL1LocRecoverHandler::SendMessage (const SendCmd& cmd, const bool& waitOnQueue)
{
    osi_sem_write_lock lock (_queueSemaphore)
        ;
    ErrorLogger::logVarError(sourceName, "TL1LocRecoverHandler", "SendMessage", TRACE2,
                          "Current Msg Queue Size: %d", _cmdqueue->QueueLength());
    if (!_cmdqueue->SendMessage(cmd, waitOnQueue))
    {
        ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "SendMessage", MAJOR,
                          "Failed to send the message into the queue!");
    }
    else
        return true;
}

void TL1LocRecoverHandler::handleCommunicationLoss(SendCmd &cmd)
{}

void TL1LocRecoverHandler::handleCommunicationRecovery(const string &neId, const string &tid, const string& protocol, const string &statusDescription)
{}

void TL1LocRecoverHandler::handleRequest (SendCmd &cmd)
{
    ErrorLogger::logError(sourceName, "TL1LocRecoverHandler", "handleRequest", TRACE1,
                          "Not implemented");
}
