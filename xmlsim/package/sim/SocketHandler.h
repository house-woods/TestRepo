/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Socket Handler
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   SocketHandler.h  $
* Version: $Revision:   1.5  $
* Description: 
* Class responsible to process events and send data to the socket.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _SocketHandler_H_
#define _SocketHandler_H_
#include <ospace/thread.h>
#include <string>


#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"
#include "ace/Connector.h"
using namespace std;

#define EMS_CONNECTION_LOSS "CONN_LOSS"

namespace EMS_FW
{

class AbstractSocketConnection;

class SocketHandler :
            public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:

    SocketHandler();

    SocketHandler(AbstractSocketConnection* connection);

    SocketHandler(AbstractSocketConnection* connection, bool negState);

    ~SocketHandler();

    int open(void*);

    int handle_close(ACE_HANDLE, ACE_Reactor_Mask);


    bool putMessage(const string& message);

    bool getMessage(string&);

    bool inNegState()
    {
        return _negState;
    }

    void setNegState(bool negState)
    {
        _negState = negState;
    }
    void setExpectedMsg (const string& expectedMsg)
    {
        _expectedMsg = expectedMsg;
    }
    void setAckMsg (const string& ackMsg)
    {
        _ackMsg = ackMsg;
    }
    void setNegWaitPeriod (int negWaitPeriod)
    {
        _negWaitPeriod = negWaitPeriod;
    }

    //void releaseNegLock() {_negotiateLock.post();}


protected:

    int handle_input(ACE_HANDLE);

    int handle_output(ACE_HANDLE);

    int handle_except(ACE_HANDLE);

    int svc(void);

private:
    char* _buffer;
    AbstractSocketConnection* _connection;
    bool _negState;
    int _negWaitPeriod;
    string _expectedMsg;
    string _ackMsg;
    osi_mutex _negotiateLock;
    bool _isExpectedMsg (const string& message);


private:

    bool _shuttingDown;
    bool _connected;
    string _clientIp;
    string _clientPort;
    string _clientAddress;
    string _localIp;
    string _localPort;
    string _localAddress;

};


};

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */ 
/*
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Svc_Tuple<EMS_FW::SocketHandler>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
#pragma instantiate ACE_Svc_Tuple<PEER_HANDLER>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<EMS_FW::SocketHandler> *, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */




#endif
