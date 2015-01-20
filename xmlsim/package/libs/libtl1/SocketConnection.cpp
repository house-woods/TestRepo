/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Socket Connection
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   SocketConnection.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* Class responsible to maintain the connection to the socket.
* 
* Revision History:
* Rev 1.2  6/18/2003  lzou
*  add Timeout for connectTo
* Rev 1.1  2/2003   lzou  
*  add AbstractSocketConnection
* Rev 1.0  init version  dtchalak
* $Log:  $
**********************************************************************************************/

#include "SocketConnection.h"
#include "SocketHandler.h"
#include "sysErrLog.h"
#include <ospace/thread.h>
#include "EmsUnixDefines.h"


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


SocketConnection::SocketConnection()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketConnection", "SocketConnection", __LINE__, TRACE3, "We are in default constructor.");
    _connector = 0;
    _conTimeout = 5;
    _conRetry = 0;
}

SocketConnection::SocketConnection(unsigned portNumber, const string& hostName, const string& conTimeOut, const string& conRetry)
        : AbstractSocketConnection(portNumber, hostName)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketConnection", "SocketConnection", TRACE11, "We are in constructor.");
    _connector = 0;
    if (conTimeOut.empty())
    {
        _conTimeout = 5;
    }
    else
    {
        _conTimeout = atoi (conTimeOut.c_str());
        if (_conTimeout < 0)
            _conTimeout = 5;
    }
    if (conRetry.empty())
    {
        _conRetry = 0;
    }
    else
    {
        _conRetry = atoi (conRetry.c_str());
        if (_conRetry < 0 )
            _conRetry = 0;
    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "SocketConnection", "SocketConnection", TRACE11,
                                 "End constructor with Timeout Value %d and Retry Value %d .", _conTimeout, _conRetry);

}



SocketConnection::~SocketConnection()
{}

bool SocketConnection::connectTo()
{
    ACE_INET_Addr addr(_portNumber, _hostname.c_str());
    SocketHandler* handler = new SocketHandler(this);
    char buffer[10];
    string key = _hostname + "." + itoa(_portNumber, buffer, 10);
    _connector = new SocketConnector;
    //ACE_OS::sleep(1);

    int n = -1;

    ACE_Time_Value timeout (_conTimeout);
    ACE_Synch_Options option (ACE_Synch_Options::USE_TIMEOUT,
                              timeout);
    for (; n < _conRetry; n++)
    {
        if (_connector->connect(handler, addr, option) == -1)
        {
            ErrorLogger::logVarError(sourceName, "SocketConnection", "connectTo", __LINE__, MINOR,
                                     "Failed to connect after [%d] of [%d] attempts.", n + 2, _conRetry + 1);
            _isActive = false;
            //return false;
        }
        else
            break;
    }

    if (n == _conRetry)
    {
        _isActive = false;
        if (_connector)
        {
            delete _connector;
            _connector = NULL;
        }
        return false;
    }
    ErrorLogger::logVarError(sourceName, "SocketConnection", "connectTo", TRACE1, "Established connection to %s", key.c_str());
    // The socket handler should register itself with the connection
    //   addHandler(key, handler);
    _isActive = true;
    return true;
}





bool SocketConnection::initialize()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketConnection", "initialize", TRACE11, "Method start.");

    if (!AbstractSocketConnection::initialize())
        return false;


    if (!connectTo())
        return false;

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketConnection", "initialize", TRACE11, "Method finished.");
    return true;
}



bool SocketConnection::shutdown()
{

    ErrorLogger::logError(sourceName, "SocketConnection", "shutDown", __LINE__, TRACE3, "Method started.");

    if (_connector)
    {
        delete _connector;
        _connector = 0;
    }

    return AbstractSocketConnection::shutdown();
}


void SocketConnection::connectionLoss()
{

    ErrorLogger::logError(sourceName, "SocketConnection", "connectionLoss", __LINE__, TRACE3, "Method started.");
    AbstractSocketConnection::connectionLoss();
    ErrorLogger::logError(sourceName, "SocketConnection", "connectionLoss", __LINE__, TRACE3,
                          "Method ended.");

}



