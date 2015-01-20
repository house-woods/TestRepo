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
* File: $Workfile:   SocketConnection.h  $
* Version: $Revision:   1.5  $
* Description: 
* Class responsible to maintain the connection to the socket.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _SocketConnection_H_
#define _SocketConnection_H_

#include "AbstractSocketConnection.h"
#include "ace/Synch.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"


namespace EMS_FW
{


class SocketHandler;


typedef ACE_Connector<SocketHandler, ACE_SOCK_Connector> SocketConnector;

class SocketConnection : public AbstractSocketConnection
{

public:

    SocketConnection();

    SocketConnection(unsigned portNumber, const string& hostName,
                     const string& conTimeOut = "", const string& conRetry = "");

    virtual ~SocketConnection();


    bool initialize();

    bool connectTo();

    bool shutdown();


    //bool send_data(const char* data);

    //bool putMessage(const string& message);

protected:

    bool _isActive;
    //int svc(void);

    virtual void processMessage(const char* message) = 0;

    //static void* reactorLoop(void*);

    //void processMessages();

    virtual void processThread()
    { }

    virtual void connectionLoss();

    //bool getMessage(string& message);

    virtual bool incompleteMessage(string& msg, string& retTL1Msg)
    {
        return false;
    }

private:

    //SocketHandler* _handler;
    SocketConnector* _connector;
    //ACE_thread_t thread_names[2];
    int _conTimeout;
    int _conRetry;


};

};

#endif


