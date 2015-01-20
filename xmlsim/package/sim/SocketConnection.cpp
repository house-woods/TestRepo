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


using namespace std;

#include "SocketConnection.h"
#include "SocketHandler.h"

#include <ospace/thread.h>

#include <iostream>

extern const char* sourceName;

using namespace EMS_FW;


SocketConnection::SocketConnection()
{
    _connector = 0;
    _conTimeout = 5;
    _conRetry = 0;
}

SocketConnection::SocketConnection(unsigned portNumber, const string& hostName, const string& conTimeOut, const string& conRetry)
        : AbstractSocketConnection(portNumber, hostName)
{
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

}



SocketConnection::~SocketConnection()
{
}

bool SocketConnection::connectTo()
{
    ACE_INET_Addr addr(_portNumber, _hostname.c_str());
    SocketHandler* handler = new SocketHandler(this);
    char buffer[10];

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
            _isActive = false;
            //return false;
        }
        else
            break;
    }

    if (n == _conRetry)
    {
        _isActive = false;
        if (handler)
        {
            delete handler;
            handler = NULL;
        }
        if (_connector)
        {
            delete _connector;
            _connector = NULL;
        }
        return false;
    }
    //   addHandler(key, handler);
    _isActive = true;
    return true;
}





bool SocketConnection::initialize()
{
    if (!connectTo())
        return false;

    if (!AbstractSocketConnection::initialize())
        return false;

    if (!connectTo())
        return false;

    return true;
}



bool SocketConnection::shutdown()
{


    if (_connector)
    {
        delete _connector;
        _connector = 0;
    }

    return AbstractSocketConnection::shutdown();
}


void SocketConnection::connectionLoss()
{

    AbstractSocketConnection::connectionLoss();

}



