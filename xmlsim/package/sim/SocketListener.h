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
* Originator: $Author:   lzou  $
* File: $Workfile:   SocketListener.h  $
* Version: $Revision:   1.5  $
* Description: 
* 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _SocketListener_H_
#define _SocketListener_H_

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "AbstractSocketConnection.h"
#include "SocketHandler.h"
using namespace std;

namespace EMS_FW
{
class SocketHandler;

typedef ACE_Acceptor<SocketHandler, ACE_SOCK_Acceptor> TL1AcceptorType;

class SocketListener :
            public TL1AcceptorType
{
public:

    SocketListener();

    SocketListener(AbstractSocketConnection* socketConnection,
                   const string& expectedMsg, const string& ackMsg, int negWaitPeriod);

    ~SocketListener();


protected:

    int make_svc_handler(SocketHandler*& sh);

    int handle_input (ACE_HANDLE);

    //int accept_svc_handler (SocketHandler* sh);

private:

    //SocketHandler* _socketHandler;
    AbstractSocketConnection* _socketConnection;

    string _expectedMsg;

    string _ackMsg;

    int _negWaitPeriod;
};


};

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<EMS_FW::SocketHandler, ACE_SOCK_Acceptor>
;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<EMS_FW::SocketHandler,ACE_SOCK_Acceptor>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif
