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
* File: $Workfile:   SocketHandler.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* Class responsible to process events and send data to the socket.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#include "SocketHandler.h"
#include "SocketConnection.h"
#include "TL1SimSocketAcceptor.h"


extern const char* sourceName;

using namespace EMS_FW;



#define EMS_BUFFER_SIZE 70000

SocketHandler::SocketHandler()
{
    _buffer = new char[EMS_BUFFER_SIZE];
    _connection = 0;
}


SocketHandler::~SocketHandler()
{
    /* ACE_Reactor::instance()->remove_handler(this,ACE_Event_Handler::READ_MASK);
     msg_queue()->deactivate();
     wait();  // wait the threads to exit*/
    delete [] _buffer;
}

SocketHandler::SocketHandler(AbstractSocketConnection* connection)
{
    _buffer = new char[EMS_BUFFER_SIZE];
    _connection = connection;
    _negState = false;
    _connected = false;
    _shuttingDown = false;
}

SocketHandler::SocketHandler(AbstractSocketConnection* connection, bool negState)
{
    _buffer = new char[EMS_BUFFER_SIZE];
    _connection = connection;
    _negState = negState;
    _connected = false;
    _shuttingDown = false;

}

bool SocketHandler::putMessage(const string& message)
{
    char* msg = new char[message.size()];
    strcpy(msg, message.c_str());
    ACE_Message_Block* mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(strlen(msg),
                                         ACE_Message_Block::MB_DATA,
                                         0, msg), 0);
    if (putq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        { // queue is deactivated
        }
        delete [] msg;
        return false;
    }
    return true; // message was deposited to the queue
}



bool SocketHandler::getMessage(string& message)
{
    ACE_Message_Block* mb = 0;
    if (this->getq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        { // queue is deactivated
            return false;
        }
        else
        {
            return true;
        }
    }
    char* msg = mb->rd_ptr();
    message = (const char*) msg;
    delete [] msg;
    mb->release();
    return true;
}


int SocketHandler::open(void*)
{
    ACE_INET_Addr remoteAddress;
    peer().get_remote_addr (remoteAddress);
    string remoteIP (remoteAddress.get_host_addr());
    unsigned long remotePort = remoteAddress.get_port_number();

    ACE_INET_Addr localAddress;
    peer().get_local_addr (localAddress);
    string localIP (localAddress.get_host_addr());
    unsigned long localPort = localAddress.get_port_number();

    _clientIp = remoteIP;
    char buffer[20];
    sprintf (buffer, "%d", remotePort);
    _clientPort = string (buffer);
    _clientAddress = _clientIp + ":" + _clientPort;


    _localIp = localIP;
    char buffer2[20];
    sprintf (buffer2, "%d", localPort);
    _localPort = string (buffer2);
    _localAddress = _localIp + ":" + _localPort;


    cout << endl << "+ Client connected >> ";
    cout << endl << "   Remote IP [" << _clientIp << "] Remote Port [" << _clientPort << "]";
    cout << endl << "   Local  IP [" << _localIp << "]  Local  Port [" << _localPort << "] ";
    cout << endl;
    _connected = true;




    _connection->addHandler(_clientAddress, this);


    ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::READ_MASK);



    activate(); // This will create another thread


    return 0;
}

int SocketHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
    if (_connected == true)
    {
        cout << endl << "+ Client disconnected >> ";
        cout << endl << "   Remote IP [" << _clientIp << "] Remote Port [" << _clientPort << "]";
        cout << endl << "   Local  IP [" << _localIp << "]  Local  Port [" << _localPort << "] ";
        cout << endl;
        _connected = false;
    }
    _connection->eraseHandler (_clientAddress);
    if (_shuttingDown)
        return 0;
    _shuttingDown = true;
    msg_queue()->deactivate();
    // wait for svc thread to exit
    wait();
    delete this;
    return 0;
}





int SocketHandler::handle_input(ACE_HANDLE)
{

    memset(_buffer, 0, EMS_BUFFER_SIZE);
    ssize_t num = peer().recv(_buffer, EMS_BUFFER_SIZE);
    string msg((const char*) _buffer);

    //  if (_connection)

    //cout << endl << "+ Received following message from " << _clientAddress << endl << msg << endl << endl;

    //cout << "((((((((((((A client has connected. A new acceptor will be created))))))))))))))))";
    //  cout << "Received

    if (num > 0)
    {
        /*  if (_negState)
          {
           if (_isExpectedMsg(msg))
           {
            _connection->send_data (_ackMsg.c_str());
            _negotiateLock.post();
           }
           return 0;
          }*/
        _connection->putMessage(msg, _clientAddress);
        return 0;
    }
    else
    {
        //   _connection->putMessage(EMS_CONNECTION_LOSS,_clientAddress);
        return -1;
    }

}

int SocketHandler::handle_output(ACE_HANDLE)
{
    return 0;
}

int SocketHandler::handle_except(ACE_HANDLE)
{
    return 0;
}

int SocketHandler::svc(void)
{

    if (_negState)
    {
        //_negotiateLock.reset();
        /* if (_negotiateLock.wait(_negWaitPeriod*1000))
         {
           _connection->putMessage(EMS_CONNECTION_LOSS);
          return 0;
         }*/

    }
    setNegState(false);
    string msg;
    while (1)
    {

        if (!getMessage(msg))
        {
            return 0;
        }
        if (msg.size() == 0)
        {
            continue;
        }
        if (msg == EMS_CONNECTION_LOSS)
        {
            return 0;
        }
        int length = msg.length();

        //    cout << endl << "+ Sending following data to client at " << _clientAddress << endl << msg << endl << endl;

        peer().send_n(msg.c_str(), length);
    }
    return 0;
}

bool SocketHandler::_isExpectedMsg (const string& message)
{
    //string comMsg (message);
    if (message == _expectedMsg)
        return true;
    return false;
}




