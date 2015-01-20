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
#include "TL1ConnectionManager.h"



extern const char* sourceName;

using namespace EMS_FW;


#define EMS_BUFFER_SIZE 70000

SocketListener::SocketListener()
{
}

SocketListener::~SocketListener()
{
}

SocketListener::SocketListener(AbstractSocketConnection* socketConnection,
                               const string& expectedMsg, const string& ackMsg, int negWaitPeriod)
{
    _socketConnection = socketConnection;
    _expectedMsg = expectedMsg;
    _ackMsg = ackMsg;
    _negWaitPeriod = negWaitPeriod;
}



int SocketListener::make_svc_handler(SocketHandler*& sh)
{
    //   cout << "((((((((((((A client has connected. A new acceptor will be created))))))))))))))))";
    //TL1ConnectionManager::getInstance()->cloneAcceptor (NULL);

    //cout << "Socket-listener make_svc_handler invoked " << endl;
    if (_socketConnection)
    {
        //      cout << "@@@@@@@@@@@@Creating a brand new socket handler @@@@@@@@@@@@@" << endl;
        sh = new SocketHandler(_socketConnection, true);
        sh-> setExpectedMsg (_expectedMsg);
        sh-> setAckMsg (_ackMsg);
        sh-> setNegWaitPeriod (_negWaitPeriod);
        // get IP+PORT from socketConnection as the key
        // For this application, we only have one SVC handler for one connection
        // We don't care much key for now.
        //logging_peer_addr.get_host_name (filename, MAXHOSTNAMELEN);


        string key = _socketConnection->getConnectionId();
        //      cout << "Adding handler with key " << key << endl;
        //      _socketConnection->addHandler(key, sh);

    }
    return TL1AcceptorType::make_svc_handler(sh);
}

int SocketListener::handle_input (ACE_HANDLE listener)
{
    //cout << "+Socket liseneter handle_input " << endl;
    int handlersSize = _socketConnection->sizeofHandler();

    /* if (_socketConnection->sizeofHandler()>=1)
     {
      //int retcode = -1;
      //retcode = shutdown (this->get_handle(), SHUT_RDWR);
      return -1;
     }*/

    return TL1AcceptorType::handle_input (listener);
}


