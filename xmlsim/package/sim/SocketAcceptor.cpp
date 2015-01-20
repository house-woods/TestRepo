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

#include <ospace/thread.h>


extern const char* sourceName;

using namespace EMS_FW;



SocketAcceptor::SocketAcceptor()
{
    _acceptor = 0;
}

SocketAcceptor::SocketAcceptor(unsigned portNumber, const string& hostName)
        : AbstractSocketConnection(portNumber, hostName)
{
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
        ACE_INET_Addr addr(_portNumber, _hostname.c_str());
        _acceptor = new SocketListener(this, _expectedMsg, _ackMsg, _negWaitPeriod);
        if ((!_acceptor) || (_acceptor->open(addr, ACE_Reactor::instance(), 1) == -1))
        {
            cout << "Coud not open a listener ";
            return false;
        }

        /*   _acceptor2 =  new SocketListener(this, _expectedMsg, _ackMsg, _negWaitPeriod);
            //if((!_acceptor)||(_acceptor->open(addr, ACE_Reactor::instance(),1)==-1))
            if (_acceptor2->open(addr, ACE_Reactor::instance(),1) == -1)
            {
              cout << "Coud not open a listener ";
              return false;      
            }*/

        /*    while (1)
            {
              cout << "Waiting for a client ";
            ACE_SOCK_Stream peer;
            ACE_INET_Addr peer_addr;
              int status = _acceptor->accept (peer, &peer_addr);
              if (status)
              {
                cout << " Client connected from " << peer_addr;
              }
         
            }*/


    }
    catch (...)
    {}
    return true;
}

bool SocketAcceptor::initialize()
{
    if (!listenFrom())
        return false;

    if (!AbstractSocketConnection::initialize())
        return false;

    return true;
}



bool SocketAcceptor::shutdown()
{

    bool re = true;
    if (!AbstractSocketConnection::shutdown())
        re = false;
    return re;
}


void SocketAcceptor::connectionLoss()
{

    /* AbstractSocketConnection::connectionLoss();
     _closeListener();
     listenFrom();*/

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
    {}
    catch (...)
    {}
}


