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
#include "sysErrLog.h"
#include "tutil.h"
#include "TL1CommandConfigurator.h"


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


#define EMS_BUFFER_SIZE 70000

SocketHandler::SocketHandler()
{
    _buffer = new char[EMS_BUFFER_SIZE];
    _connection = 0;
    _shuttingDown = false;
    _isAServerSocket = false;
    msg_queue()->high_water_mark(CUSTOM_HWM);
    msg_queue()->low_water_mark(CUSTOM_LWM);
}


SocketHandler::~SocketHandler()
{
    delete [] _buffer;
}

SocketHandler::SocketHandler(AbstractSocketConnection* connection, bool isAServerSocket)
{
    _buffer = new char[EMS_BUFFER_SIZE];
    _connection = connection;
    _negState = false;
    _shuttingDown = false;
    _isAServerSocket = isAServerSocket;
    msg_queue()->high_water_mark(CUSTOM_HWM);
    msg_queue()->low_water_mark(CUSTOM_LWM);
}

SocketHandler::SocketHandler(AbstractSocketConnection* connection, bool negState, bool isAServerSocket)
{
    _buffer = new char[EMS_BUFFER_SIZE];
    _connection = connection;
    _negState = negState;
    _shuttingDown = false;
    _isAServerSocket = isAServerSocket;
    msg_queue()->high_water_mark(CUSTOM_HWM);
    msg_queue()->low_water_mark(CUSTOM_LWM);
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
        {
            ErrorLogger::logError(sourceName, "SocketHandler", "putMessage", __LINE__, TRACE11, "Queue was deactivated.");
        }
        delete [] msg;
        return false;
    }
    else
    {
        if (ErrorLogger::isErrTypeEnabled(TRACE11))
        {
            string duplicateMessage ;
            ErrorLogger::maskPassword(message, duplicateMessage);
            ErrorLogger::logVarError(sourceName, "SocketHandler", "putMessage", __LINE__, TRACE11, "Sent the following message: \n %s \n ", duplicateMessage.c_str());
        }
    }
    return true; // message was deposited to the queue
}



bool SocketHandler::getMessage(string& message)
{
    ACE_Message_Block* mb = 0;
    if (this->getq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        {
            // queue is deactivated
            ErrorLogger::logError(sourceName, "SocketHandler", "getMessage", __LINE__, TRACE11, "Queue was deactivated.");
            return false;
        }
        else
        {
            ErrorLogger::logError(sourceName, "SocketHandler", "getMessage", __LINE__, TRACE11, "Error from the queue will continue.");
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
    // get IP address and port of client socket connecting to me or a server socket I am connecting to
    ACE_INET_Addr remoteAddress;
    peer().get_remote_addr (remoteAddress);
    string remoteIP (remoteAddress.get_host_addr());
    unsigned long remotePort = remoteAddress.get_port_number();

    ACE_INET_Addr localAddress;
    peer().get_local_addr (localAddress);
    string localIP (localAddress.get_host_addr());
    unsigned long localPort = localAddress.get_port_number();

    // if this is a server socket and the connecting client is not local, deny
    if ((localIP != remoteIP) && (_isAServerSocket))
    {
        // disconnect the client
        peer().close();
        ErrorLogger::logVarError(sourceName, "SocketHandler", "open", TRACE1, "Client attempting to connect from >> IP [%s]  Port [%d] << to >> Local IP [%s]  Local Port [%d] << is not authorized to connect and has been disconnected", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
        return -1;
    }
    else
    {
        if (_isAServerSocket)
            ErrorLogger::logVarError(sourceName, "SocketHandler", "open", TRACE1, "Client connected from >> IP [%s]  Port [%d] << to >> Local IP [%s]  Local Port [%d] << ", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
        else
            ErrorLogger::logVarError(sourceName, "SocketHandler", "open", TRACE1, "Connected to host on >> IP [%s]  Port [%d] << from  >> Local IP [%s]  Local Port [%d] << ", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
        addHandlerToConnection ();
        activate();
        // handle_input does not get called until we do this, so handle_close
        // can't be called either making sure we don't do a delete this while open is running
        ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
        return 0;
    }
}

int SocketHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
    ErrorLogger::logError(sourceName, "SocketHandler", "handle_close", TRACE1, "Entering ....");
    if (_shuttingDown)
    {
        ErrorLogger::logError(sourceName, "SocketHandler", "handle_close", TRACE1, "Shutdown already in progress; Returning");
        return 0;
    }
    _shuttingDown = true;
    ErrorLogger::logError(sourceName, "SocketHandler", "handle_close", TRACE1, "Deactivating message queue used by the SVC thread loop");
    msg_queue()->deactivate();
    // wait for svc thread to exit
    ErrorLogger::logError(sourceName, "SocketHandler", "handle_close", TRACE1, "Waiting for the SVC thread to exit");
    wait();

    // In case handle_close got called because of removal of queue, we want to make sure that handle_close won't be called again
    ErrorLogger::logError(sourceName, "SocketHandler", "handle_close", TRACE1, "Remove the handler from Reactor");
    ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

    ACE_INET_Addr remoteAddress;
    peer().get_remote_addr (remoteAddress);
    string remoteIP (remoteAddress.get_host_addr());
    unsigned long remotePort = remoteAddress.get_port_number();

    ACE_INET_Addr localAddress;
    peer().get_local_addr (localAddress);
    string localIP (localAddress.get_host_addr());
    unsigned long localPort = localAddress.get_port_number();

    ErrorLogger::logVarError(sourceName, "SocketHandler", "handle_close", TRACE1, "Closing socket stream between >> Remote IP [%s]  Remote Port [%d] << and >> Local IP [%s]  Local Port [%d]", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
    peer().close();

    ErrorLogger::logError(sourceName, "SocketHandler", "handle_close", TRACE1, "Destryoing my instance");
    delete this;
    return 0;
}


int SocketHandler::handle_input(ACE_HANDLE)
{
    ErrorLogger::logError(sourceName, "SocketHandler", "handle_input", TRACE11, "Method started.");

    ssize_t num = peer().recv(_buffer, EMS_BUFFER_SIZE-1);
    _buffer[num] = '\0';

    //    string duplicateMessage ;
    //    ErrorLogger::maskPassword(_buffer, duplicateMessage);
    ErrorLogger::logVarError(sourceName, "SocketHandler", "handle_input", __LINE__, TRACE11, "Received the following message = \n %s \n", _buffer);

    if (num > 0)
    {
        if (_negState)
        {
            if (_isExpectedMsg(_buffer))
            {
                ErrorLogger::logVarError(sourceName, "SocketHandler", "handle_input", __LINE__, TRACE1,
                                         "Found the expected Msg: %s", _buffer);
                _connection->send_data (_ackMsg.c_str());
                _negotiateLock.post();
            }
            return 0;
        }
        sendDataToConnection(_buffer);
        return 0;
    }
    else
    {
        // unregister me from the connection
        removeHandlerFromConnection ();
        //    _connection->removeHandler (this);
        sendDataToConnection (EMS_CONNECTION_LOSS);
        //_connection->putMessage(EMS_CONNECTION_LOSS);
        ErrorLogger::logError(sourceName, "SocketHandler", "handle_input", __LINE__, TRACE3, "Connection Lost");
        return -1;

    }

    ErrorLogger::logError(sourceName, "SocketHandler", "handle_input", TRACE11, "Method finished.");
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
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "SocketHandler", "handle_svc", TRACE11, "Method started.");

    string dropConnectionMessage = "DROP_CONNECTION";
    int dropConnectionMessageSize = dropConnectionMessage.length();

    if (_negState)
    {
        if (_negWaitPeriod > 0)
        {
            if (_negotiateLock.wait(_negWaitPeriod*1000))
            {
                ErrorLogger::logVarError( sourceName, "SocketHandler", "svc",
                                          MAJOR, "Negoatiatiion Time Out. This handler will be removed. ");
                sendDataToConnection (EMS_CONNECTION_LOSS);
                //_connection->putMessage(EMS_CONNECTION_LOSS);
                return 0;
            }
            ErrorLogger::logVarError( sourceName, "SocketHandler", "svc",
                                      TRACE1, "Negoatiatiion is completed in Handler. ");
        }
    }
    setNegState(false);
    string msg;
    ErrorLogger::logError(sourceName, "SocketHandler", "svc", __LINE__, TRACE11, "Starting the SVC loop to read from the message queue for socket data");
    while (1)
    {
        if (msg_queue ()->deactivated())
        {
            ErrorLogger::logError(sourceName, "SocketHandler", "svc", __LINE__, TRACE1, "Message queue has been deactivated...returning");
            return 0;
        }
        if (!getMessage(msg))
        {
            continue;
        }
        if (msg.length() == 0)
        {
            continue;
        }
        if ( (msg.length() == dropConnectionMessageSize) && (msg == dropConnectionMessage) )
        {
            ErrorLogger::logError(sourceName, "SocketHandler", "svc", __LINE__, MAJOR, "Got the DROP_CONNECTION message; Sending CONNECTION_LOSS to the connection and aborting ");
            removeHandlerFromConnection ();
            sendDataToConnection (EMS_CONNECTION_LOSS);

            msg_queue ()->deactivate();
            continue;
        }
        int length = msg.length();

        if (ErrorLogger::isErrTypeEnabled(TRACE11))
        {
            string duplicateMessage;
            ErrorLogger::maskPassword(msg, duplicateMessage);
            ErrorLogger::logVarError(sourceName, "SocketHandler", "svc", __LINE__, TRACE11, "Sending the following message to the socket\n%s\n", duplicateMessage.c_str());
        }
        string cmdType("");
        string::size_type cmdPos = msg.find(":");
        if (cmdPos != string::npos)
        {
            cmdType = msg.substr(0,cmdPos);
            if (0 == cmdType.compare("CANC-USER"))
            {
                cmdType = "canc-user";
            }
        }
        //default timeout if cmdType is empty or cmdType not find.
        //getMSECTimeout returns millisecond
        unsigned long timeOut = TL1CommandConfigurator::instance()->getMSECTimeout (cmdType)/1000;
        ACE_Time_Value t(timeOut);
        peer().send_n(msg.c_str(), length, &t);
    }
    return 0;
}

bool SocketHandler::_isExpectedMsg(const char* message)
{
    return (_expectedMsg.compare(message) == 0);
}

void SocketHandler::deactivateQueue ()
{
    ACE_INET_Addr remoteAddress;
    peer().get_remote_addr (remoteAddress);
    string remoteIP (remoteAddress.get_host_addr());
    unsigned long remotePort = remoteAddress.get_port_number();

    ACE_INET_Addr localAddress;
    peer().get_local_addr (localAddress);
    string localIP (localAddress.get_host_addr());
    unsigned long localPort = localAddress.get_port_number();

    ErrorLogger::logVarError(sourceName, "SocketHandler", "deactivateQueue", TRACE1, "Closing socket stream between >> Remote IP [%s]  Remote Port [%d] << and >> Local IP [%s]  Local Port [%d]", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
    peer().close();
}

void SocketHandler::unregisterConnection (void)
{
    MutexLock lock (_connectionMutex)
    ;
    _connection = NULL;
}

bool SocketHandler::sendDataToConnection (const char* msg)
{
    MutexLock lock (_connectionMutex)
    ;

    return (_connection && _connection->putMessage(msg));
}

void SocketHandler::removeHandlerFromConnection ()
{
    MutexLock lock (_connectionMutex)
    ;
    if (_connection)
        _connection->removeHandler (this);
}

void SocketHandler::addHandlerToConnection ()
{
    MutexLock lock (_connectionMutex)
    ;
    if (_connection)
    {
        _connection->addHandler (_connection->getName(), this);
        if (_connection->getClassName() == "NBISocketAcceptor")
        {
            _connection->send_data ("\n\n                    TELLABS 7190 NBI INTERFACE (C) 2005 Tellabs Inc");
            _connection->send_data ("\n                    ------------------------------------------------ \n\n");
            _connection->send_data ("\nType 'help;' for a list of commands\n\n");
        }
    }
}

