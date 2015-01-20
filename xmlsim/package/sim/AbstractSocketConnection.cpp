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
* Rev 1.1  4/2003  lzou
change the processMessage to handle long response broken by '>'
change the processMessage to handle multiple messages in the buffer
* Rev 1.0  02/2003  init version by lzou
* $Log:  $
**********************************************************************************************/


using namespace std;
#include "AbstractSocketConnection.h"
#include "SocketHandler.h"
//#include "sysErrLog.h"



extern const char* sourceName;

using namespace EMS_FW;
//using namespace ELogger;


osi_thread* _reactorLoopThread;

// Thread that pumps the reactor Loop
//osi_thread* _reactorLoopThread = NULL;



AbstractSocketConnection::AbstractSocketConnection()
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","AbstractSocketConnection",__LINE__,TRACE3,"We are in default constructor.");
    _handlers.clear();
    _reactorLoopThread = NULL;
}

AbstractSocketConnection::AbstractSocketConnection(unsigned portNumber, const string& hostName)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","AbstractSocketConnection",__LINE__,TRACE3,"We are in constructor.");
    _handlers.clear();
    _portNumber = portNumber;
    _hostname = hostName;
}


bool AbstractSocketConnection::putMessage(const string& message, const string &key)
{

    // We should have the mutex here since potentially multiple SVC handler could
    // access here at the same time.
    // socket places messages in a q here to be read by the connection add header for identifying the client

    //cout << endl << "Socket puts message in connection's q (Add key here) " << endl;

    string messageWithKey = "[" + key + "]" + message;

    char* msg = new char[messageWithKey.size() + 1];
    strcpy(msg, messageWithKey.c_str());
    ACE_Message_Block* mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(strlen(msg),
                                         ACE_Message_Block::MB_DATA,
                                         0, msg), 0);
    if (putq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        {
            // queue is deactivated
            //ErrorLogger::logError(sourceName,"AbstractSocketConnection","putMessage",__LINE__,TRACE3,"Queue was deactivated.");
        }
        delete [] msg;
        return false;
    }
    return true; // message was deposited to the queue
}



bool AbstractSocketConnection::getMessage(string& message, string &key)
{
    // connection gets its message socket has placed from the qu here

    ACE_Message_Block* mb = 0;
    if (this->getq(mb) == -1)
    {
        if (errno == ESHUTDOWN)
        {
            // queue is deactivated
            // ErrorLogger::logError(sourceName,"AbstractSocketConnection","getMessage",__LINE__,TRACE3,"Queue was deactivated.");
            return false;
        }
        else
        {
            //ErrorLogger::logError(sourceName,"AbstractSocketConnection","getMessage",__LINE__,TRACE3,"Error from the queue will continue.");
            return true;
        }
    }
    char* msg = mb->rd_ptr();
    string messageWithKey((const char*) msg);

    int keystartpos = messageWithKey.find_first_of ("[");
    int keyendpos = messageWithKey.find_first_of ("]");
    key = messageWithKey.substr (keystartpos + 1, keyendpos - keystartpos - 1);
    //  cout << "passing back message from q with key " << key;

    message = messageWithKey.substr (keyendpos + 1, messageWithKey.size() - keyendpos);

    delete [] msg;
    mb->release();
    return true;
}



AbstractSocketConnection::~AbstractSocketConnection()
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","~AbstractSocketConnection",__LINE__,TRACE3,"We are in destructor.");

    // let the derived class call finalize() so that thread
    // finish before the derived class got desctructed

}


bool AbstractSocketConnection::initialize()
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","initialize",__LINE__,TRACE3,"Method start.");


    if (_reactorLoopThread == NULL)
    {
        // start the thread that is pumping the reactor Loop
        try
        {
            _reactorLoopThread = new osi_thread(AbstractSocketConnection::reactorLoop, (void*) this, 0, true);
        }
        catch (osi_thread_toolkit_error& err)
        {
            //ErrorLogger::
            // logVarError(sourceName,"AbstractSocketConnection","initialize",__LINE__,
            // CRITICAL,"run()->%s",
            // err.description(err.code()));
            return false;

        }
    }
    activate(THR_NEW_LWP,
             2,    //three new threads
             0,
             ACE_DEFAULT_THREAD_PRIORITY,
             -1,
             this,
             0,
             0,
             0,
             thread_names);

    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","initialize",__LINE__,TRACE3,"Method finished.");
    return true;
}



bool AbstractSocketConnection::shutdown()
{

    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","shutDown",__LINE__,TRACE3,"Method started.");
    /*   MutexLock (_handlersMutex);
       map <string, SocketHandler*>::iterator iter;
       SocketHandler* handler = NULL;
       for (iter = _handlers.begin();iter!= _handlers.end(); iter++)
       {
             handler = (*iter).second;
             if (handler)
             {
                delete handler;
                handler = NULL;
             }
             _handlers.erase(iter);
       }
     
       _handlers.clear();*/


    return true;

}

void* AbstractSocketConnection::reactorLoop(void* arg)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","reactorLoop",__LINE__,TRACE3,"Method start.");

    //ACE_Reactor::instance()->handle_events();
    ACE_Time_Value t(10);
    ACE_Reactor::instance()->owner(ACE_OS::thr_self());
    while (1)
    {
        //ACE_OS::sleep(10);
        //ACE_Reactor::instance()->handle_events(t);
        ACE_Reactor::instance()->handle_events();
    }
    // if(ErrorLogger::isTraceListOn())
    //  ErrorLogger::logError(sourceName,"AbstractSocketConnection","reactorLoop",__LINE__,TRACE3,"Method finished.");
    return 0;
}

void AbstractSocketConnection::processMessages()
{
    // if(ErrorLogger::isTraceListOn())
    //  ErrorLogger::logError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE3,"Method start.");


    string msg;
    string key;
    string storedMsg;
    while (1)
    {
        //ErrorLogger::logError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE3,"Waiting on a queue to get message.");
        if (!getMessage(msg, key))
        {
            //ErrorLogger::logError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,MAJOR,"Error reading the queue exiting...");
            return ;
        }
        try
        {
            //if(ErrorLogger::isTraceListOn())
            // ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE3,"Got the message length = %d ->%s",msg.length(),msg.c_str());
        }
        catch (...)
            {}
        if (msg.length() == 0)
        {
            // ignore this message
            continue;
        }
        if (msg == EMS_CONNECTION_LOSS)
        {
            //ErrorLogger::logError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE3,"Calling ConnectionLoss");
            connectionLoss();
            storedMsg = ""; // remove the stored content
            continue;
        }
        // Follows are temp solution for '>' sign in response/msg --- lzou
        // After NE implements it correctly (having the header for continuous
        // message block), we should
        // 1. add a member varible in TL1BaseCmd to hold the msg
        // 2. if m_finished is false, save the msg
        // 3. if new msg coming, concatenate the msg
        // 4. if m_finished is true, start to process

        // After we finalize, we should have a virtual function to handle it
        // We want to skip some chars such as '>' for OTS
        //int index = msg.find_first_of('>');
        //while (index!=osi_npos) {
        // msg = msg.erase (index, 1);
        //index = msg.find_first_of('>');
        //}


        // End of skip portion
        storedMsg.append(msg);
        int iPos = 0;
        while (!incompleteMessage(storedMsg, iPos))
        {
            try
            {
                int len = storedMsg.length();
                // if(ErrorLogger::isTraceListOn())
                // ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE10,"To be processed message length = %d ->%s",len,storedMsg.c_str());
                // break the msg str into a complete TL1 message
                const string& tl1msg = storedMsg.substr(0, iPos + 1);
                //if(ErrorLogger::isTraceListOn())
                // ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE7,"One of the complete TL1 message: length = %d ->%s",tl1msg.length(),tl1msg.c_str());

                processMessage(tl1msg, key);
                storedMsg.erase(0, iPos + 1);
                // if(ErrorLogger::isTraceListOn())
                //  ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE10,"Left Message in the Queue:length = %d ->%s",storedMsg.length(),storedMsg.c_str());
                iPos = 0;
            }
            catch (...)
                {}

            // storedMsg = ""; // remove the stored content
        }
    }

    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","processMessages",__LINE__,TRACE3,"Method finished.");
}


bool AbstractSocketConnection::send_data(const string& data, const string& key)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","send_data",__LINE__,TRACE3,"Method started.");

    //  cout << "Messege being sent to client at [" << key << "] >> " << endl << data << endl;

    MutexLock lock (_handlersMutex)
    ;
    //string sdata(data);
    if (_handlers.size() <= 0)
    {
        // if(ErrorLogger::isTraceListOn())
        //    ErrorLogger::logError(sourceName,"AbstractSocketConnection","send_data",__LINE__,MAJOR,"No SVC Handler available");
        return false;
    }
    else if (key.empty())
    {
        // if key is empty we send to all (e.g. for autonmous messages)
        map <string, SocketHandler*>::iterator iter = _handlers.begin();
        for (iter = _handlers.begin();iter != _handlers.end();iter++)
        {
            SocketHandler* handler = (*iter).second;
            handler->putMessage(data);
        }
    }
    else
    {
        map <string, SocketHandler*>::iterator iter = _handlers.find(key);
        if (iter == _handlers.end())
            return false;
        else
        {
            SocketHandler* handler = (*iter).second;
            handler->putMessage(data);
        }
    }
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","send_data",__LINE__,TRACE3,"Method finished.");
    return true;
}



int AbstractSocketConnection::svc(void)
{
    // Event Loop
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","svc",__LINE__,TRACE3,"Method started.");
    if (ACE_Thread::self() == thread_names[0])
        processMessages();
    else
    {
        // ErrorLogger::logError(sourceName,"AbstractSocketConnection","svc",__LINE__,TRACE3,"Before processThread.");
        processThread();
    }
    return 0;
}

void AbstractSocketConnection::connectionLoss()
{

    //ErrorLogger::logError(sourceName,"AbstractSocketConnection","connectionLoss",__LINE__,TRACE3,"Method started.");
    // shutdown();

}

bool AbstractSocketConnection::addHandler(const string& key, SocketHandler* socketHandler)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","addHandler",__LINE__,TRACE3,"Method started.");
    MutexLock lock (_handlersMutex)
    ;
    map <string, SocketHandler*>::iterator iter = _handlers.find(key);
    if (iter != _handlers.end())
    {
        //  if (ErrorLogger::isTraceListOn())
        //    ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","addHandler",__LINE__,MAJOR,"There is entry existing in the map with the key %s",key.c_str());
        return false;
    }
    else
    {
        _handlers.insert( pair<string, SocketHandler*>(key, socketHandler));
    }
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","addHandler",__LINE__,TRACE3,"Method finished.");
    return true;

}

bool AbstractSocketConnection::eraseHandler(const string& key)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","eraseHandler",__LINE__,TRACE3,"Method started.");
    MutexLock lock (_handlersMutex)
    ;
    map <string, SocketHandler*>::iterator iter = _handlers.find(key);
    if (iter == _handlers.end())
    {
        //   if (ErrorLogger::isTraceListOn())
        //    ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","eraseHandler",__LINE__,MAJOR,"Couldn't find the entry in the map with the key %s",key.c_str());
        return false;
    }
    else
    {
        SocketHandler *handler = (*iter).second;
        _handlers.erase(iter);
    }
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","eraseHandler",__LINE__,TRACE3,"Method finished.");

    return true;

}

bool AbstractSocketConnection::isInDoubleQuote (const string& msg, int iPos, int& index)
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","isInDoubleQuote",__LINE__,TRACE10,
    // "Method Begins.");

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
            //if(ErrorLogger::isTraceListOn())
            // ErrorLogger::logVarError(sourceName,"AbstractSocketConnection","isInDoubleQuote",__LINE__,TRACE10,
            // "In the Double Quote, try next one from index %d.", index);

            return true;
        }
        pointer = rightQuote + 1;
    }
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","isInDoubleQuote",__LINE__,TRACE10,
    // "Method Ends.");

    return false;
}

bool AbstractSocketConnection::finalize()
{
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","finalize",__LINE__,TRACE4,
    // "Method Begin.");

    // ATTENTION something need to go here
    shutdown();
    // ATTENTION need to find out how to shut down the threads
    msg_queue()->deactivate();
    // if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","finalize",__LINE__,TRACE4,
    //"Before Wait for thread exit.");
    wait();  // wait the threads to exit
    // if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logError(sourceName,"AbstractSocketConnection","finalize",__LINE__,TRACE4,
    // "Method End.");

    try
    {
        if (_reactorLoopThread == NULL)
            return true;

        _reactorLoopThread->terminate();
        while (_reactorLoopThread->exists())
        {
            ACE_OS::sleep(5);

        }
        delete _reactorLoopThread;
        _reactorLoopThread = NULL;
    }
    catch (...)
    {
        if (_reactorLoopThread)
            delete _reactorLoopThread;

        _reactorLoopThread = NULL;
        return false;
    }

    cout << "reactor thread has been terminated!" << endl;

    return true;
}

