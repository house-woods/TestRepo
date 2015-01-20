/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: Abstract Socket Connection
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   AbstractSocketConnection.cpp  $
* Version: $Revision:   1.5  $
* Description:
* Class responsible to maintain the connection to the socket.
*
* Revision History:
*
* Rev 2.0   4/2005    aaali
*                     BellSouth FP3.0.1 patch
*                     added functionality to catch fatal processing errors
*                     and voluntarily drop connections when they occur
*
* Rev 1.1  4/2003  lzou
change the processMessage to handle long response broken by '>'
change the processMessage to handle multiple messages in the buffer
* Rev 1.0  02/2003  init version by lzou
* $Log:  $
**********************************************************************************************/

#include "AbstractSocketConnection.h"
#include "SocketHandler.h"
#include "sysErrLog.h"
#include <ospace/thread.h>
#include "BaseException.h"
#include "TL1Parser.h"
#include "Server_i.h"
#include <ospace/time.h>


extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;
using namespace TL1_MA::ParseTL1;

// Thread that pumps the reactor Loop
osi_thread* _reactorLoopThread = NULL;

AbstractSocketConnection::AbstractSocketConnection() :
        _shuttingDown(false)
{
    msg_queue()->high_water_mark(CUSTOM_HWM);
    msg_queue()->low_water_mark(CUSTOM_LWM);
}

AbstractSocketConnection::AbstractSocketConnection(unsigned portNumber, const string& hostName) :
        _shuttingDown(false)
{
    _portNumber = portNumber;
    _hostname = hostName;
    msg_queue()->high_water_mark(CUSTOM_HWM);
    msg_queue()->low_water_mark(CUSTOM_LWM);
}

AbstractSocketConnection::~AbstractSocketConnection()
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "~AbstractSocketConnection", __LINE__, TRACE11, "We are in destructor.");
}

void AbstractSocketConnection::shutdownHandlers (void)
{}

bool AbstractSocketConnection::sendDataToHandlers (const string &data, const string &key)
{
    MutexLock lock (_handlersMutex);

    if (_handlers.size() <= 0)
    {
        ErrorLogger::logError(sourceName, "AbstractSocketConnection", "sendDataToHandlers", __LINE__, MAJOR, "No SVC Handler available to send data");
        return false;
    }
    if (key.empty() || _handlers.size() == 1)
    {
        map <string, SocketHandler*>::iterator iter = _handlers.begin();
        SocketHandler* handler = (*iter).second;
        handler->putMessage (data);

        if (ErrorLogger::isErrTypeEnabled(TRACE7))
        {
            string duplicateMessage ;
            ErrorLogger::maskPassword(data, duplicateMessage);
            ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "sendDataToHandler", __LINE__, TRACE7, "Sent data on the message handler=%d length = %d ->%s", handler, duplicateMessage.length(), duplicateMessage.c_str());
        }
        return true;
    }
    else
    {
        map <string, SocketHandler*>::iterator iter = _handlers.find(key);
        if (iter == _handlers.end())
        {
            ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "sendDataToHandlers", __LINE__, MAJOR, "Couldn't find a socket handler in the map with the key %s", key.c_str());
            return false;
        }
        else
        {
            SocketHandler* handler = (*iter).second;
            handler->putMessage (data);

            if (ErrorLogger::isErrTypeEnabled(TRACE7))
            {
                string duplicateMessage ;
                ErrorLogger::maskPassword(data, duplicateMessage);
                ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "sendDataToHandler", __LINE__, TRACE7, "Sent data on the message handler=%d length = %d ->%s", handler, duplicateMessage.length(), duplicateMessage.c_str());
            }
            return true;
        }
    }
}

bool AbstractSocketConnection::addHandler(const string& key, SocketHandler* socketHandler)
{
    MutexLock lock (_handlersMutex);

    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "addHandler", TRACE11, "Method started.");

    map <string, SocketHandler*>::iterator iter = _handlers.find(key);
    if (iter != _handlers.end())
    {
        ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "addHandler", MAJOR, "There is no entry existing in the map with the key %s", key.c_str());

        return false;
    }
    else
    {
        _handlers.insert( pair<string, SocketHandler*>(key, socketHandler));
    }

    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "addHandler", TRACE11, "Method finished.");
    return true;

}

void AbstractSocketConnection::removeHandler (SocketHandler *handler)
{
    MutexLock lock (_handlersMutex);

    _handlers.clear(); // What?!? this works because there is only one handler per connection
}


bool AbstractSocketConnection::putMessage(const char* message)
{
    size_t len = strlen(message) + 1;
    char* msg = new char[len];
    memcpy(msg, message, len);

    ACE_Message_Block* mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(len, ACE_Message_Block::MB_DATA, 0, msg), 0);
    if (putq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        {
            // queue is deactivated
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "putMessage", __LINE__, TRACE3, "Queue appears to be deactivated.");
        }
        delete [] msg;
        return false;
    }
    return true; // message was deposited to the queue
}

bool AbstractSocketConnection::getMessage(char*& message)
{
    ACE_Message_Block* mb = 0;
    if (this->getq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        {
            // queue is deactivated
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "getMessage", __LINE__, TRACE1, "Queue appears to be deactivated.");
            return false;
        }
        else
        {
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "getMessage", __LINE__, TRACE3, "Error from the queue will continue.");
            return true;
        }
    }

    message = mb->rd_ptr();
    mb->release();

    return true;
}

bool AbstractSocketConnection::initialize()
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "initialize", __LINE__, TRACE11, "Method start.");

    if (_reactorLoopThread == NULL)
    {
        // start the thread that is pumping the reactor Loop
        try
        {
            _reactorLoopThread = new osi_thread(AbstractSocketConnection::reactorLoop, (void*) this, 0, true);
        }
        catch (osi_thread_toolkit_error& err)
        {
            ErrorLogger::
            logVarError(sourceName, "AbstractSocketConnection", "initialize", __LINE__,
                        CRITICAL, "run()->%s",
                        err.description(err.code()));
            return false;
        }
    }

    activate ();

    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "initialize", __LINE__, TRACE11, "Method finished.");

    return true;
}



bool AbstractSocketConnection::shutdown()
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "shutDown", __LINE__, TRACE1, "Shutting down socket handlers");

    return true;
}

void* AbstractSocketConnection::reactorLoop(void* arg)
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "reactorLoop", __LINE__, TRACE3, "Method start.");

    ACE_Time_Value t(10);
    ACE_Reactor::instance()->owner(ACE_OS::thr_self());
    while (1)
    {
        ACE_Reactor::instance()->handle_events();
    }

    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "reactorLoop", __LINE__, TRACE3, "Method finished.");

    return 0;
}

void AbstractSocketConnection::processMessages()
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE11, "Started processing messasges in a loop");

    // max socket buffer size is set by default to 4 megabytes = 4194304 bytes
    const string &maxSocketBufferSizeString = Server_i::instance()->getDefaultConfigValue ("MaxSocketBufferSize", "4194304");
    unsigned long maxSocketBufferSize = atol (maxSocketBufferSizeString.c_str());

    // max incomplete msg is set by default to 6K (6x1024=6144 bytes )
    const string &maxInCompleteMsgSizeString = Server_i::instance()->getDefaultConfigValue ("MaxInCompleteMsgSize", "6144");
    unsigned long maxInCompleteSize = atol (maxInCompleteMsgSizeString.c_str());

    const string& servername = Server_i::instance()->getServerName();

    char* pNewData = 0;
    string storedMsg;
    while (1)
    {
        try
        {
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE11, "Waiting on a queue to get message.");
            // WE SHOULD ONLY RETURN FROM HERE IF SPECIFICALLY ASKED TO
            if (msg_queue ()->deactivated())
            {
                ErrorLogger:: logVarError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE1, "Msg queue has been deactivated. CLOSING SVC THREAD AND PROCESSMESSAGES");
                return ;
            }
            if (!getMessage(pNewData))
            {
                ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE1, "Error reading from the queue. Will wait for it to deactivate before returning");
                continue;
            }

            if (ErrorLogger::isErrTypeEnabled(TRACE11))
            {
                string duplicateMessage ;
                ErrorLogger::maskPassword(pNewData, duplicateMessage);
                ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "processMessages", TRACE11, "Received the following message from socket handler\n %s \n", duplicateMessage.c_str());
            }

            size_t msg_len = strlen(pNewData);
            //ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","processMessages",TRACE1,"Got the following raw message from socket length = %d ->\n\n%s\n", msg_len, pNewData);
            if (msg_len == 0)
            {
                // ignore this message
                ErrorLogger::logError(sourceName, "AbstractSocketConnection", "handle_svc", __LINE__, MAJOR, "Got an empty message from the queue.");
                continue;
            }
            if (msg_len > maxSocketBufferSize)
            {
                string error = string ("THE FOLLOWING MESSAGE BUFFER HAS EXCEEDED THE MAXIMUM LIMIT [" + maxSocketBufferSizeString + "] ON THE SIZE OF THE SOCKET BUFFER \n\n") + pNewData + "\n\n";
                throw BaseException(error, "AbstractSocketConnection", "processMessages");
            }
            if (strcmp(pNewData, EMS_CONNECTION_LOSS) == 0)
            {
                string error = string ("EMS_CONNECTION_LOSS message received from Socket Handler; SOCKET CONNECTION TO ") + _hostname + " LOST";
                ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", TRACE1, error.c_str());
                connectionLoss();
                storedMsg = ""; // remove the stored content
                continue;
            }
            storedMsg += pNewData;
            delete [] pNewData;

            string tl1msg;
            while (!incompleteMessage(storedMsg, tl1msg))
            {
                ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE11, "Processing the following complete message chunk from the message = %d ->\n\n%s", tl1msg.length(), tl1msg.c_str());

                processMessage(tl1msg.c_str());

                ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE11, "Part of full message left to be processed length = %d ->\n\n%s", storedMsg.length(), storedMsg.c_str());
                // storedMsg = ""; // remove the stored content
            } // end inner while

            msg_len = storedMsg.length();
            if (msg_len > maxInCompleteSize)
            {
                string error = string ("THE FOLLOWING MESSAGE BUFFER HAS EXCEEDED THE MAXIMUM LIMIT [" + maxInCompleteMsgSizeString + "] ON THE SIZE OF INCOMPLETE MESSAGE \n\n").append(storedMsg).append("\n\n");
                throw BaseException(error, "AbstractSocketConnection", "processMessages");
            }

        }
        catch (BaseException &ex)
        {
            string error = string ("Error from " + ex.getExClassName() + ":" + ex.getMethodName() + " while processing the following message \n\n") + storedMsg + "\n\n" + ex.getDescription();
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, MAJOR, error.c_str());
            ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, MAJOR, "Flushing the message buffer and voluntarily causing a connection loss on %s:%d", _hostname.c_str(), _portNumber);
            storedMsg = "";
            abortConnection();
        }
        catch (ParseError &err)
        {
            string error = string ("Error while parsing the following message \n\n") + storedMsg + "\n\n" + err.m_reason + " " + err.m_code;
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, MAJOR, error.c_str());
            ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, MAJOR, "Flushing the message buffer and voluntarily causing a connection loss on %s:%d", _hostname.c_str(), _portNumber);
            storedMsg = "";
            abortConnection();
        }
        catch (...)
        {
            string error = string ("Unknown error while processing the following message \n\n") + storedMsg;
            ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, MAJOR, error.c_str());
            ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, MAJOR, "Flushing the message buffer and voluntarily causing a connection loss on %s:%d", _hostname.c_str(), _portNumber);
            storedMsg = "";
            abortConnection();
        }
    } // end outer while

    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "processMessages", __LINE__, TRACE11, "Ended message processing loop");
}

void AbstractSocketConnection::abortConnection (void)
{
    if (!_shuttingDown)
    {
        MutexLock lock (_handlersMutex);
        send_data ("DROP_CONNECTION");
        removeHandler(NULL);
    }
}

bool AbstractSocketConnection::send_data(const char* data, const string& key)
{
    if (ErrorLogger::isErrTypeEnabled(TRACE11))
    {
        string duplicateMessage(data);
        try
        {
            ErrorLogger::maskPassword(data, duplicateMessage);
        }
        catch (...)
            {}
        ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "send_data", __LINE__, TRACE11, "Sending the following data to socket handler\n%s\n", duplicateMessage.c_str());
    }

    bool result = sendDataToHandlers (data, key);
    return result;
}

int AbstractSocketConnection::svc(void)
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "svc", TRACE11, "Method started.");
    processMessages();
    return 0;
}

void AbstractSocketConnection::connectionLoss()
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "connectionLoss", TRACE3, "Method started.");
    shutdown();
}

bool AbstractSocketConnection::isInDoubleQuote (const string& msg, int iPos, int& index)
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "isInDoubleQuote", __LINE__, TRACE10,
                          "Method Begins.");

    if (iPos == osi_npos)
        return false;

    int pointer = 0;
    while (1)
    {
        int leftQuote = msg.find("\\\"", pointer);
        if ((leftQuote == osi_npos) || (iPos < leftQuote))
            break;
        pointer = leftQuote + 1;
        int rightQuote = msg.find("\\\"", pointer);
        if (rightQuote == osi_npos)
            break;
        if (iPos < rightQuote)
        {
            index = rightQuote + 1;
            ErrorLogger::logVarError(sourceName, "AbstractSocketConnection", "isInDoubleQuote", __LINE__, TRACE10,
                                     "In the Double Quote, try next one from index %d.", index);

            return true;
        }
        pointer = rightQuote + 1;
    }

    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "isInDoubleQuote", __LINE__, TRACE10,
                          "Method Ends.");

    return false;
}

bool AbstractSocketConnection::finalize()
{
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "finalize", TRACE1, "Unregistering connection from the Socket Handlers");

    _shuttingDown = true;

    MutexLock lock (_handlersMutex);

    map <string, SocketHandler*>::iterator iter;
    SocketHandler* handler = NULL;
    for (iter = _handlers.begin();iter != _handlers.end(); iter++)
    {
        handler = (*iter).second;
        // make sure handler does not call connection
        handler->unregisterConnection ();
    }
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "finalize", TRACE1, "Deactiviating connection's message queue");
    msg_queue()->deactivate();
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "finalize", TRACE1, "Waiting for connection's message queue to be deactivated");
    wait();
    ErrorLogger::logError(sourceName, "AbstractSocketConnection", "finalize", TRACE1, "Deactiviating message queue for the socket handler");
    for (iter = _handlers.begin();iter != _handlers.end(); iter++)
    {
        handler = (*iter).second;
        // handler will commit suicide when we deactivate their queue
        handler->deactivateQueue();
    }
    _handlers.clear();

    return true;
}

