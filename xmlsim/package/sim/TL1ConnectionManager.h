#ifndef __CONNECTIONMANAGER__
#define __CONNECTIONMANAGER__


#include <list>
#include <string>
#include <map>


#include "TL1SimSocketAcceptor.h"
#include "ConfigManager.h"
#include <ospace/thread/mutex.h>

using namespace std;

using namespace EMS_FW;
//typedef list<TL1SimSocketAcceptor*> ListAcceptors;

typedef osi_sem_lock<osi_mutex_semaphore> ConnectionLock;

class TL1ConnectionManager
{
public:

    ~TL1ConnectionManager();

    static TL1ConnectionManager* getInstance();
    bool initializeRne(string &tid, string &ip, string &port, string &ip2, string &port2, map<string, PropertiesList> &neList, ConfigManager *configManager);
    bool initializeGneb(string &tid, string &ip, string &port, map<string, PropertiesList> &neList, ConfigManager *configManager);
    bool startConnections(void);
    bool stopConnections(void);
    bool setNumberConnections(int numberConnections)
    {
        m_numberConnections = numberConnections;
        return true;
    };
    bool stopSingleConnection(string &port);
    bool sendAutonomous(string& port, string& response);
    bool setCommunicationState(string& neTid, string& messageTid);
    void cloneAcceptor (TL1SimSocketAcceptor *acceptor);




protected:
    TL1ConnectionManager()
    { }
    ;

private:
    int m_numberConnections;
    static TL1ConnectionManager* m_connectionMgr;
    osi_mutex m_connectionMutex;
    map<string, TL1SimSocketAcceptor*> m_listAcceptors;


};



#endif



