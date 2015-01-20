/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Socket Listener
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   SocketListener.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#include "SocketListener.h"
#include "SocketConnection.h"
#include "sysErrLog.h"
#include "EmsUnixDefines.h"


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


#define EMS_BUFFER_SIZE 70000

SocketListener::SocketListener()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketListener", "SocketListener", __LINE__, TRACE3, "We are in default constructor.");
}


SocketListener::~SocketListener()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketListener", "~SocketListener", __LINE__, TRACE3, "We are in destructor.");

}

SocketListener::SocketListener(AbstractSocketConnection* socketConnection,
                               const string& expectedMsg, const string& ackMsg, int negWaitPeriod)
{
    _socketConnection = socketConnection;
    _expectedMsg = expectedMsg;
    _ackMsg = ackMsg;
    _negWaitPeriod = negWaitPeriod;
    if (_negWaitPeriod)
    {
        ErrorLogger::logVarError( sourceName, "SocketListener", "SocketListener",
                                  TRACE1, "Waiting for a connect request form fm [FM_CONNECT] or CM [CM_CONNECT] with a timeout of [%d] seconds", _negWaitPeriod);
    }
}



int SocketListener::make_svc_handler(SocketHandler*& sh)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketListener", "make_svc_handler", __LINE__, TRACE4, "Method Starts.");

    if (_socketConnection)
    {
        if (ErrorLogger::isTraceListOn())
            ErrorLogger::logError(sourceName, "SocketListener", "make_svc_handler", __LINE__, TRACE10, "Socket Handler is not empty");
        sh = new SocketHandler(_socketConnection, true, true);
        sh-> setExpectedMsg (_expectedMsg);
        sh-> setAckMsg (_ackMsg);
        sh-> setNegWaitPeriod (_negWaitPeriod);
        // get IP+PORT from socketConnection as the key
        // For this application, we only have one SVC handler for one connection
        // We don't care much key for now.
        string key = _socketConnection->getConnectionId();

        // Socket handler will auto-regiester, check SocketHandler::open
        //      _socketConnection->addHandler(key, sh);

    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketListener", "make_svc_handler", __LINE__, TRACE4, "Method Ends.");
    return TL1AcceptorType::make_svc_handler(sh);
}

int SocketListener::handle_input (ACE_HANDLE listener)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketListener", "handle_input", __LINE__, TRACE4, "Method Starts.");
    if (_socketConnection->sizeofHandler() >= 1)
    {
        //int retcode = -1;
        //retcode = shutdown (this->get_handle(), SHUT_RDWR);
        return -1;
    }

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketListener", "handle_input", __LINE__, TRACE4, "Method Ends.");
    return TL1AcceptorType::handle_input (listener);
}


