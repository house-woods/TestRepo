/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Socket Acceptor
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   SocketAcceptor.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#include "SocketAcceptor.h"
#include "SocketHandler.h"
#include "sysErrLog.h"
#include <ospace/thread.h>


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


SocketAcceptor::SocketAcceptor()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketAcceptor", "SocketAcceptor", __LINE__, TRACE3, "We are in default constructor.");
    _acceptor = 0;
}

SocketAcceptor::SocketAcceptor(unsigned portNumber, const string& hostName)
        : AbstractSocketConnection(portNumber, hostName)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketAcceptor", "SocketAcceptor", __LINE__, TRACE3, "We are in constructor.");
    _acceptor = 0;
}



SocketAcceptor::~SocketAcceptor()
{
    _closeListener();
}

bool SocketAcceptor::listenFrom()
{
    try
    {
        osi_sem_write_lock lock (_acceptorLock)
            ;
        ACE_INET_Addr addr(_portNumber);
        _acceptor = new SocketListener(this, _expectedMsg, _ackMsg, _negWaitPeriod);
        //ACE_OS::sleep(1);
        if ((!_acceptor) || (_acceptor->open(addr, ACE_Reactor::instance()) == -1))
        {
            ErrorLogger::logError(sourceName, "SocketAcceptor", "listenFrom", __LINE__, TRACE1, "Fail to connect exiting...");
            return false;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "SocketAcceptor", "listenFrom", __LINE__, MINOR, "Exception happen on Read Sem Lock ->%s", ex.description(ex.code()));
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "SocketAcceptor", "listenFrom", __LINE__, MINOR, "Uknown Exception");
    }
    return true;
}





bool SocketAcceptor::initialize()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketAcceptor", "initialize", __LINE__, TRACE3, "Method start.");
    if (!listenFrom())
        return false;

    if (!AbstractSocketConnection::initialize())
        return false;

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketAcceptor", "initialize", __LINE__, TRACE3, "Method finished.");
    return true;
}



bool SocketAcceptor::shutdown()
{

    ErrorLogger::logError(sourceName, "SocketAcceptor", "shutDown", __LINE__, TRACE3, "Method started.");
    bool re = true;
    if (!AbstractSocketConnection::shutdown())
        re = false;
    return re;
}


void SocketAcceptor::connectionLoss()
{

    ErrorLogger::logError(sourceName, "SocketAcceptor", "connectionLoss", __LINE__, TRACE3, "Method started.");
    AbstractSocketConnection::connectionLoss();
    _closeListener();
    listenFrom();
    ErrorLogger::logError(sourceName, "SocketAcceptor", "connectionLoss", __LINE__, TRACE3,
                          "Method ended.");

}

void SocketAcceptor::_closeListener()
{
    try
    {
        osi_sem_write_lock lock (_acceptorLock)
            ;
        if (_acceptor)
        {
            _acceptor->close();
            delete _acceptor;
            _acceptor = NULL;
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "SocketAcceptor", "_closeListener", __LINE__, MINOR, "Exception happen on Read Sem Lock ->%s", ex.description(ex.code()));
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "SocketAcceptor", "_closeListener", __LINE__, MINOR, "Uknown Exception");
    }
}


