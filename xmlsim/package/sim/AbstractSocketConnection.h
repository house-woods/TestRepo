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

#include <string>
#include <map>
#include "AbstractConnection.h"
using namespace std;



#include <ospace/thread.h>
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"





namespace EMS_FW
{
class SocketHandler;

typedef osi_sem_lock<osi_mutex_semaphore> MutexLock;




class AbstractSocketConnection : public ACE_Task<ACE_SYNCH>

{

public:

    AbstractSocketConnection();

    AbstractSocketConnection(unsigned portNumber, const string& hostName);

    virtual ~AbstractSocketConnection();

    bool eraseHandler(const string& key);

    bool initialize();

    virtual bool finalize();

    bool shutdown();

    virtual bool send_data(const string& data, const string& key = "");

    bool putMessage(const string& message, const string &key);

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

protected:

    map< string, SocketHandler* > _handlers;

    int svc(void);

    virtual void processMessage(const string& message) = 0;

    virtual void processMessage (const string& message, const string &key) = 0;

    static void* reactorLoop(void*);

    void processMessages();

    virtual void processThread()
    { }

    virtual void connectionLoss();

    bool getMessage(string& message, string &key);

    virtual bool incompleteMessage(const string& msg, int& iPos)
    {
        return false;
    }

    virtual bool isInDoubleQuote (const string& msg, int iPos, int& index);

private:

    string _connectionID;

    ACE_thread_t thread_names[2];

    osi_mutex _handlersMutex;

    void shutdownHandlers (void);
    bool sendDataToHandlers (const string &data, const string &key);

public:
    string _neName;
    string _hostname;
    unsigned _portNumber;
};

};

#endif
