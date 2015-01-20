#ifndef __TL1CONNECTIONAMANGER_H__
#define __TL1CONNECTIONAMANGER_H__

#include <string>
#include <list>
#include <map>
#include <ospace/thread.h>
#include "TL1Message.h"
#include "TL1Connection.h"
#include "TL1ConnectionResponse.h"


extern const char* sourceName;

//using namespace TL1_MA;
//using namespace EMS_FW;
//using namespace ELogger;
//using namespace SysMon;
//using namespace EMS;

namespace EMS_FW
{

class TL1Connection;
class TL1Session;

typedef struct Association
{
    TL1Connection *con;
    TL1Session *session;
    bool enabled;
    string tid;
}
Association;

typedef osi_sem_lock<osi_mutex_semaphore> MessageLock;

class TL1ConnectionManager
{
public:
    static TL1ConnectionManager *instance();
    TL1Connection *getConnection (string, unsigned long);
    void addAssociation (const string &tid, TL1Session *session, TL1Connection *con, bool enabled);
    void removeAssociation (TL1Session*, TL1Connection *);
    void removeAssociation(TL1Session*, const string& host, unsigned long port);
    int getNumberOfAssociatedSessions (TL1Connection *con);
    int getNumberOfAssociatedConnections (TL1Session *session);
    int getNumberOfEnabledAssociations (TL1Connection *con);
    int getNumberOfEnabledAssociations (TL1Session *session);
    void sendDataToSession (TL1Connection *con, const char *data, TL1Message* msg);
    void sendDataToConnection (TL1Session *session, const char *data);
    TL1Connection *getConnectionInUse (TL1Session *session, string &connectionName);

    TL1Session *getSessionByTID (const string &tid);
    TL1Session *getSessionByCTAG (const string &tid);

    void removeAllAssociations (TL1Session *session);
    void removeConnection (TL1Connection *con);
    void setAssociationState (TL1Session *session, TL1Connection *con, bool state);
    TL1Connection * getStandbyConnection (TL1Session *session, TL1Connection *con, string &standbyConnectionName);
    void handleConnectionLoss (TL1Connection *con);
    void handleConnectionRecovery (TL1Connection *con);
    void recoveryLoop();
    bool startRecoveryThread ();
    bool stopRecoveryThread ();
    void addInactiveConnectionsToBeRecoverd (TL1Session *session);
    void removeFromRecoverableList (TL1Connection *con);
    TL1Connection *getConnectionByIndex (TL1Session *session, int index, string &connectionName);
    void dropActiveConnection (TL1Session *session);
    void xPingActiveConnections ();

    void configureMetering(const string& tid, bool enable, short delayPeriodForBlock = -1, short percentDropForUnblock = -1); // -1 means leave unchanged
    void unblockMetering(const string& tid);

private:

    unsigned int _heartBeatCounter;
    unsigned int _heartBeatTimer;

    // guards for the connection list
    osi_mutex _messageMutex;
    osi_mutex _assocMutex;
    osi_mutex _connectionPoolMutex;

    osi_counting_semaphore _notifyRecoverLoop;
    // connection recovery thread
    osi_thread* _recoveryThread;

    TL1ConnectionManager (): _notifyRecoverLoop (0), _messageMutex(false)
    {}
    ;
    static TL1ConnectionManager * _instance;  // Singleton instance
    // list of available connections
    map <string, TL1Connection *> _connectionPool;
    // list of associations
    list <Association> _associations;
    // add connection based on ip+port
    //void addConnection (string, TL1Connection *);
    // get connection by key
    //TL1Connection *getConnection (string);
    // connect to an ip + port
    TL1Connection *startConnection (string, unsigned long);
    // list of connections to be recovered
    list <TL1Connection *> _recoverableConnections;
    // list of connections used internally by the recovery thread

    void addConnectionToBeRecovered (TL1Connection *);

    void recoverInactiveConnections (void);

    bool hasAssociationToRemove (TL1Session *session, TL1Connection* &con);

    list<TL1Session *> getLostSessions (TL1Connection *con);

    list<TL1Session *> getSessionsToRecover (TL1Connection *con);

    void reportConnectionLossToSessions (TL1Connection *con) ;

}
; // end class declaration
}
; // end namespace
#endif
