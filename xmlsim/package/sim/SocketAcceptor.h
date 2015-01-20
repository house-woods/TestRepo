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
* File: $Workfile:   SocketAcceptor.h  $
* Version: $Revision:   1.5  $
* Description: 
* 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _SocketAcceptor_H_
#define _SocketAcceptor_H_


#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "AbstractSocketConnection.h"
#include "SocketListener.h"
using namespace std;

namespace EMS_FW
{


class SocketAcceptor : public AbstractSocketConnection
{

public:

    SocketAcceptor();

    SocketAcceptor(unsigned portNumber, const string& hostName = "");

    virtual ~SocketAcceptor();


    bool initialize();

    bool listenFrom();

    bool shutdown();

protected:

    virtual void processMessage (const string& message, const string &key) = 0;

    virtual void processMessage(const string& message) = 0;

    //void processMessages();

    virtual void processThread()
    { }

    virtual void connectionLoss();

    virtual bool incompleteMessage(const string& msg, int& iPos)
    {
        return false;
    }

    string getExpectedMsg()
    {
        return _expectedMsg;
    }
    string getAckMsg()
    {
        return _ackMsg;
    }
    int getNegWaitPeriod()
    {
        return _negWaitPeriod;
    }

    string _expectedMsg;

    string _ackMsg;

    int _negWaitPeriod;

private:

    SocketListener* _acceptor;

    SocketListener* _acceptor2;

    osi_read_write_semaphore _acceptorLock;

    void _closeListener();






};

};

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<EMS_FW::SocketHandler, ACE_SOCK_Acceptor>
;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<EMS_FW::SocketHandler, ACE_SOCK_Acceptor>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif


