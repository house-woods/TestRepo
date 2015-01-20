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
* Version: $Revision:   1.5  $
* Description:
*
*
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _AbstractSocketConnection_H_
#define _AbstractSocketConnection_H_

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "AbstractConnection.h"
#include <string>
#include <map>
#include <ospace/thread.h>


namespace EMS_FW
{

typedef osi_sem_lock<osi_mutex_semaphore> MutexLock;


class SocketHandler;


class AbstractSocketConnection : public ACE_Task<ACE_SYNCH> , public AbstractConnection
{

public:

    AbstractSocketConnection();

    AbstractSocketConnection(unsigned portNumber, const string& hostName);

    virtual ~AbstractSocketConnection();


    bool initialize();

    virtual bool finalize();

    bool shutdown();

    void abortConnection (void);

    virtual bool send_data(const char* data, const string& key = "");

    bool putMessage(const char* message);

    bool addHandler(const string& key, SocketHandler* socketHandler);

    void removeHandler (SocketHandler* socketHandler);

    int sizeofHandler()
    {
        return _handlers.size();
    };

    void setConnectionId(const string& id)
    {
        _connectionID = id;
    };

    const string& getConnectionId()
    {
        return _connectionID;
    };

    void setSocketLostFlag ();
    enum 
    {
        CUSTOM_HWM = 32 * 1024,
        CUSTOM_LWM = 31 * 1024
    };

protected:

    map< string, SocketHandler* > _handlers;

    int svc(void);

    virtual void processMessage(const char* message) = 0;

    static void* reactorLoop(void*);

    void processMessages();

    virtual void processThread()
    { }

    virtual void connectionLoss();

    bool getMessage(char*& message);

    virtual bool incompleteMessage(string& msg, string& retTL1Msg)
    {
        return false;
    }

    virtual bool isInDoubleQuote (const string& msg, int iPos, int& index);

private:

    bool _shuttingDown;
    string _connectionID;

    ACE_thread_t thread_names[2];

    osi_mutex _handlersMutex;

    void shutdownHandlers (void);
    bool sendDataToHandlers (const string &data, const string &key);

};

};

#endif


