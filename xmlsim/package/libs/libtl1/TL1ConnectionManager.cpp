#include "TL1ConnectionManager.h"
#include "TL1ConnectionResponse.h"
#include "TL1Connection.h"
#include "BaseException.h"
#include "sysErrLog.h"
#include <ospace/thread.h>
#include "EmsUnixDefines.h"
#include "Server_i.h"



extern const char* sourceName;

//using namespace TL1_MA;
using namespace EMS_FW;
using namespace ELogger;
using namespace SysMon;
using namespace EMS;

extern const char* sourceName;
static const char* className = "TL1ConnectionManager.cpp";

TL1ConnectionManager* TL1ConnectionManager::_instance = NULL;

TL1ConnectionManager* TL1ConnectionManager::instance ()
{
    if (_instance == NULL)
    {
        _instance = new TL1ConnectionManager();
    }
    return _instance;
}

void* runRecoveryThread( void *arg )
{
    TL1ConnectionManager* manager = (TL1ConnectionManager*) arg;
    ErrorLogger::logError(sourceName, "TL1ConnectionManager", "runRecoveryThread", __LINE__, TRACE1,
                          "Connection Recovery thread starting the recovery loop");
    while ( true )
    {
        try
        {
            manager->recoveryLoop();
        }
        catch (...)
        {
            ErrorLogger::logError(sourceName, "TL1ConnectionManager", "runRecoveryThread", __LINE__, CRITICAL,
                                  "Caught an exception running the recovery loop");
        }
    }
    ErrorLogger::logError(sourceName, "TL1ConnectionManager", "runRecoveryThread", __LINE__, TRACE1,
                          "Connection Recovery thread exiting");
    return ((void*) 0);
}

void TL1ConnectionManager::addConnectionToBeRecovered (TL1Connection *con)
{
    MessageLock lock (_messageMutex)
    ;
    if (con)
    {
        string name = con->getName();
        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addConnectionToBeRecovered", TRACE1, "Adding connection to host [%s] for recovery", name.c_str());
        bool connectionFound = false;
        for (list<TL1Connection *>::iterator i = _recoverableConnections.begin();i != _recoverableConnections.end();i++)
        {
            TL1Connection *conInList = (*i);
            if (conInList == con)
            {
                connectionFound = true;
                break;
            }
        }
        if (!connectionFound)
        {
            _recoverableConnections.push_back (con);
            string name = con->getName();
            ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addConnectionToBeRecovered", TRACE1, "Connection to host %s added for recovery", name.c_str());
        }
    }
}



void TL1ConnectionManager::recoverInactiveConnections (void)
{
    list<TL1Connection*> recoveredConnections;

    try
    {
        MessageLock lock(_messageMutex);
        list<TL1Connection *>::iterator i = _recoverableConnections.begin();
        while (i != _recoverableConnections.end())
        {
            TL1Connection *con = (*i);
            if (con == NULL)
            {
                i = _recoverableConnections.erase (i);
                break; // Not sure why this is here?
            }
            else
                if (con->isActive())
                {
                    i = _recoverableConnections.erase (i);
                    break; // Not sure why this is here?
                }
                else
                {
                    const string &name = con->getName();
                    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "recoverInactiveConnections", TRACE1, "Trying to recover connection to host on %s", name.c_str());
                    if (con->connectTo())
                    {
                        string name = con->getName();
                        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "recoverInactiveConnections", TRACE1, "Was able to recover host on %s", name.c_str());
                        recoveredConnections.push_back(con);
                        i = _recoverableConnections.erase (i);
                        break; // Not sure why this is here?
                    }
                }
            i++;
        }
    }
    catch(...)
    {
    }

    // Separate the handling of connection recovery from outside the scope of the _messageMutex to avoid deadlocks
    //
    for (list<TL1Connection*>::iterator it = recoveredConnections.begin(); it != recoveredConnections.end(); it++)
    {
        handleConnectionRecovery(*it);
    }
}


void TL1ConnectionManager::recoveryLoop()
{
    try
    {
        recoverInactiveConnections ();
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "recoveryLoop", MINOR, "Exception while recovering connections; Ignoring and continuing");
    }
    ACE_OS::sleep (10);
    try
    {
        // Send HeartBeat Check. Set _heartBeatTimer to 0 to disable service
        if (_heartBeatTimer > 0)
        {
            if (_heartBeatCounter >= _heartBeatTimer)
            {
                _heartBeatCounter = 1;
                xPingActiveConnections();
            }
            else
            {
                _heartBeatCounter++;
            }
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "recoveryLoop", __LINE__, MINOR, "Exception happened while sending heartbeat check to idle connections");
    }

}


bool TL1ConnectionManager::startRecoveryThread ()
{
    try
    {
        ErrorLogger::logError(sourceName, "TL1ConnectionManager", "startRecoveryThread", TRACE1, "Trying to start the connection recovery thread...");

        // Initialize values here for the heart beat timer and counter.
        string heartBeatTimerValue = Server_i::instance()->getDefaultConfigValue("HeartBeatTimerValue", "30"); // multiply by 10 seconds
        _heartBeatTimer = atoi (heartBeatTimerValue.c_str());
        _heartBeatCounter = 1;

        _recoveryThread = new osi_thread(runRecoveryThread, (void*) this, 0, true);
        ErrorLogger::logError(sourceName, "TL1ConnectionManager", "startRecoveryThread", TRACE1, "Started the connection recovery thread.");
    }
    catch (osi_thread_toolkit_error&)
    {
        ErrorLogger::logError(sourceName, "TL1ConnectionManager", "startRecoveryThread", CRITICAL, "Coult not start the connection recovery thread.");
        return false;
    }
    return true;
}

bool TL1ConnectionManager::stopRecoveryThread ()
{
    try
    {
        ErrorLogger::logError(sourceName, "TL1ConnectionManager", "stopRecoveryThread", TRACE1, "Trying to stop the connection recovery thread...");
        if (_recoveryThread)
        {
            _recoveryThread->terminate();
            delete _recoveryThread;
            _recoveryThread = 0;
        }
        ErrorLogger::logError(sourceName, "TL1ConnectionManager", "stopRecoveryThread", TRACE1, "Stopped the connection recovery thread.");
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, "TL1ConnectionManager", "stopRecoveryThread", TRACE1, "Caught an unknow exception while trying to terminate the connection recovery thread.");
        return false;
    }
    return true;
}

//-----------------------Connection management routines-----------------//
TL1Connection* TL1ConnectionManager::getConnection (string ip, unsigned long port)
{
    MessageLock lock (_connectionPoolMutex)
    ;
    char buffer[10];
    string key = ip + "." + itoa(port, buffer, 10);

    TL1Connection *con = NULL;
    if (key.size() != 0)
    {
        map <string, TL1Connection *>::iterator i = _connectionPool.find (key);
        if (i != _connectionPool.end())
            con = (*i).second;
    }
    if (con == NULL)
    {
        con = startConnection (ip, port);
        // add connection
        _connectionPool.insert (pair<string, TL1Connection *>(key, con));
    }
    return con;
}

TL1Connection *TL1ConnectionManager::startConnection (string ip, unsigned long port)
{
    TL1Connection *con = new TL1Connection ("", port, ip, false);
    string maxConnectRetries = Server_i::instance()->getDefaultConfigValue("NEConnectionRetries", "1");
    string waitBeforeRetry = Server_i::instance()->getDefaultConfigValue("NEConnectionWaitBeforeRetry", "1");

    int tries = atoi (maxConnectRetries.c_str());
    int waitBeforeRetryValue = atoi (waitBeforeRetry.c_str());

    for (int i = 1;i < tries + 1;i++)
    {
        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "startConnection", __LINE__, TRACE1, "Attemping to connect to %s on port %d try %d of %d", ip.c_str(), port, i, tries);
        if (con->initialize())
        {
            ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "startConnection", __LINE__, TRACE1, "Connected to %s on port %d", ip.c_str(), port);
            break;
        }
        else
        {
            if (i < tries)
            {
                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "startConnection", __LINE__, MAJOR, "Could not connect to %s on port %d on attenpt %d of %d Will try again after %d seconds", ip.c_str(), port, i, tries, waitBeforeRetryValue);
                sleep (waitBeforeRetryValue);
            }
        }
    }
    return con;
}

void TL1ConnectionManager::removeConnection (TL1Connection *con)
{
    MessageLock lock(_connectionPoolMutex);

    string connectionName = con->getName();
    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeConnection", __LINE__, TRACE3, "Removing connection to %s.", connectionName.c_str());
    map <string, TL1Connection *>::iterator i = _connectionPool.begin();
    while (i != _connectionPool.end())
    {
        if (con == (*i).second)
        {
            _connectionPool.erase (i);
            removeFromRecoverableList (con);
            delete con;
            con = NULL;
            ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeConnection", __LINE__, TRACE3, "Connection to %s removed from recovery list and connection pool", connectionName.c_str());
            break;
        }
        i++;
    }
}

void TL1ConnectionManager::xPingActiveConnections ()
{
    MessageLock lock (_connectionPoolMutex)
    ;

    TL1Connection *con = NULL;
    for (map <string, TL1Connection *>::iterator i = _connectionPool.begin(); i != _connectionPool.end(); i++)
    {
        con = (*i).second;
        if (con->isActive())
        {
            // if there are no sessions using this connection, sent a heartbeat
            if (!getNumberOfEnabledAssociations(con))
            {
                //Create a new connection see if it allows us to connect using socket from idling connections.
                unsigned port = con->getPortNumber();
                string hostname (con->getHostName());
                ACE_Time_Value timeout (10); // timeout is 10 seconds
                ACE_INET_Addr addr(port, hostname.c_str());
                ACE_SOCK_Stream stream;
                ACE_SOCK_Connector tempConnection;

                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "xPingActiveConnections", __LINE__, TRACE1, "Sending a heart-beat check to %s on port %d", hostname.c_str(), port);
                // If there is error during initialize, abort connection of the socket connection to terminate it.
                if (tempConnection.connect(stream, addr, &timeout) == -1)
                {
                    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "xPingActiveConnections", __LINE__, TRACE1, "Could not connect to %s on port %d (network cable unplugged?) Aborting connection!", hostname.c_str(), port);
                    // only abort the connection if it is active already
                    if (con->isActive())
                    {
                        // check if this is still idle
                        if (!getNumberOfEnabledAssociations(con))
                            con->abortConnection();
                    }
                }
                else
                {
                    ACE_INET_Addr remoteAddress;
                    stream.get_remote_addr (remoteAddress);
                    string remoteIP (remoteAddress.get_host_addr());
                    unsigned long remotePort = remoteAddress.get_port_number();

                    ACE_INET_Addr localAddress;
                    stream.get_local_addr (localAddress);
                    string localIP (localAddress.get_host_addr());
                    unsigned long localPort = localAddress.get_port_number();
                    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "xPingActiveConnections", TRACE1, "Connected to host on >> IP [%s]  Port [%d] << from  >> Local IP [%s]  Local Port [%d] << ", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
                    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "xPingActiveConnections", TRACE1, "Closing socket stream between >> Remote IP [%s]  Remote Port [%d] << and >> Local IP [%s]  Local Port [%d]", remoteIP.c_str(), remotePort, localIP.c_str(), localPort);
                    stream.close();
                }
            }
        }
    }
}



//-----------------------Session-Connection association management routines-----------------//
void TL1ConnectionManager::addAssociation (const string &tid, TL1Session *session, TL1Connection *con, bool enabled)
{
    // create association if it is not in association list.
    bool alreadyExist = false;
    string connectionName = con->getName();
    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addAssociation", __LINE__, TRACE3, "Creating new association to %s for %s.", connectionName.c_str(), tid.c_str());
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        TL1Session *sessionInAssoc = (*i).session;
        if ((sessionInAssoc == session) && (con == conInAssoc))
        {
            alreadyExist = true;
            break;
        }
        i++;
    }
    if (!alreadyExist)
    {
        if (enabled)
            ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addAssociation", __LINE__, TRACE3, "Creating new enabled association to %s for %s.", connectionName.c_str(), tid.c_str());
        else
            ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addAssociation", __LINE__, TRACE3, "Creating new disabled association to %s for %s.", connectionName.c_str(), tid.c_str());
    
        MessageLock lock (_assocMutex);
        Association newAssociation;
        newAssociation.con = con;
        newAssociation.session = session;
        newAssociation.enabled = enabled;
        newAssociation.tid = tid;
        _associations.push_back (newAssociation);
    }
}

void TL1ConnectionManager::removeAssociation (TL1Session *session, TL1Connection *con)
{
    list <TL1Connection*> removableConnections;
    try
    {
        MessageLock lock(_assocMutex);
        list <Association>::iterator i = _associations.begin();
        while (i != _associations.end())
        {
            TL1Connection *conInAssoc = (*i).con;
            TL1Session *sessionInAssoc = (*i).session;
            if ((sessionInAssoc == session) && (con == conInAssoc))
            {
                string connectionName = con->getName();
                string tid = session->gettid();
                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeAssociation", __LINE__, TRACE1, "Erasing association to %s for NE %s.", connectionName.c_str(), tid.c_str());
                _associations.erase (i);

                // Iterate through the association list again and if no other sessions are 
                // using this connection, add this to a temp list for removal
                //
                bool canRemoveConnection = true;
                for (list<Association>::const_iterator itAssoc = _associations.begin(); itAssoc != _associations.end(); itAssoc++)
                {
                    if (itAssoc->con == con)
                    {
                        canRemoveConnection = false;
                        break;
                    }
                }
                if (canRemoveConnection)
                {
                    removableConnections.push_back(con);
                }
                break;
            }
            i++;
        }
    }
    catch(...)
    {
    }
    
    try
    {
        for (list<TL1Connection*>::iterator itRemovableConn = removableConnections.begin(); itRemovableConn != removableConnections.end(); itRemovableConn++)
        {
            removeConnection(*itRemovableConn);
        }
    }
    catch(...)
    {
    }
}

void TL1ConnectionManager::removeAssociation(TL1Session* session, const string& host, unsigned long port)
{
    list <TL1Connection*> removableConnections;
    try
    {
        MessageLock lock (_assocMutex);

        list <Association>::iterator i = _associations.begin();
        char buff[10];
        string conName = host + "." + itoa(port, buff, 10);
        while (i != _associations.end())
        {
            TL1Connection* conInAssoc = (*i).con;
            TL1Session* sessionInAssoc = (*i).session;
            if ((sessionInAssoc == session) && (conInAssoc->getName() == conName))
            {
                const string& tid = session->gettid();
                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeAssociation", __LINE__, TRACE1, "Erasing association to %s for NE %s.", conName.c_str(), tid.c_str());
                _associations.erase(i);

                // Iterate through the association list again and if no other sessions are 
                // using this connection, add this to a temp list for removal
                //
                bool canRemoveConnection = true;
                for (list<Association>::const_iterator itAssoc = _associations.begin(); itAssoc != _associations.end(); itAssoc++)
                {
                    if (itAssoc->con == conInAssoc)
                    {
                        canRemoveConnection = false;
                        break;
                    }
                }
                if (canRemoveConnection)
                {
                    removableConnections.push_back(conInAssoc);
                }

                break;
            }
            i++;
        }
    }
    catch(...)
    {
    }

    try
    {
        for (list<TL1Connection*>::iterator itRemovableConn = removableConnections.begin(); itRemovableConn != removableConnections.end(); itRemovableConn++)
        {
            removeConnection(*itRemovableConn);
        }
    }
    catch(...)
    {
    }
}

void TL1ConnectionManager::setAssociationState (TL1Session *session, TL1Connection *con, bool state)
{
    MessageLock lock (_assocMutex)
    ;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        TL1Session *sessionInAssoc = (*i).session;
        if ((sessionInAssoc == session) && (con == conInAssoc))
        {
            bool oldState = (*i).enabled;
            (*i).enabled = state;
            if ((oldState == true) && (state == false))
            {
                con->clearCmdMapForNe (session->getNeId());
                session->ignoreConnectionLoss ();
            }
            break;
        }
        i++;
    }
    // get number of sessions associated with connection,..if zero remove the connection

}

int TL1ConnectionManager::getNumberOfAssociatedSessions (TL1Connection *con)
{
    MessageLock lock (_assocMutex)
    ;
    int numberOfSessions = 0;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        if (con == conInAssoc)
            numberOfSessions ++;
        i++;
    }
    return numberOfSessions;
}

int TL1ConnectionManager::getNumberOfEnabledAssociations (TL1Connection *con)
{
    MessageLock lock (_assocMutex)
    ;
    int numberOfSessions = 0;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        bool enabled = (*i).enabled;
        if ( (con == conInAssoc) && (enabled) )
            numberOfSessions ++;
        i++;
    }
    return numberOfSessions;
}


int TL1ConnectionManager::getNumberOfAssociatedConnections (TL1Session *session)
{
    MessageLock lock (_assocMutex)
    ;
    int numberOfConnections = 0;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Session *sessionInAssoc = (*i).session;
        if (sessionInAssoc == session)
            numberOfConnections ++;
        i++;
    }
    return numberOfConnections;
}

TL1Connection *TL1ConnectionManager::getConnectionByIndex (TL1Session *session, int index, string &connectionName)
{
    MessageLock lock (_assocMutex)
    ;

    int iterator = 0;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        TL1Session *sessionInAssoc = (*i).session;
        if (sessionInAssoc == session)
        {
            if (iterator == index)
            {
                connectionName = conInAssoc->getName();
                return conInAssoc;
            }
            iterator++;
        }
        i++;
    }
    return NULL;
}

int TL1ConnectionManager::getNumberOfEnabledAssociations (TL1Session *session)
{
    MessageLock lock (_assocMutex)
    ;
    int numberOfConnections = 0;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Session *sessionInAssoc = (*i).session;
        bool enabled = (*i).enabled;
        if ((sessionInAssoc == session) && (enabled))
            numberOfConnections ++;
        i++;
    }
    return numberOfConnections;
}


void TL1ConnectionManager::sendDataToSession (TL1Connection *con, const char *data, TL1Message* msg)
{
    TL1Session *session = NULL;
    if (msg->m_type == TL1Message::Autonomous)
        TL1Session::forwardEventToSession (data, msg, con, con->getName());
    else
        TL1Session::forwardResponseToSession (data, msg, con, con->getName());
}

TL1Connection *TL1ConnectionManager::getConnectionInUse (TL1Session *session, string &connectionName)
{
    MessageLock lock (_assocMutex)
    ;
    TL1Connection *con = NULL;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Session *sessionInAssoc = (*i).session;
        bool enabled = (*i).enabled;
        if ((sessionInAssoc == session) && (enabled))
        {
            con = (*i).con;
            if (con)
                connectionName = con->getName();
            break;
        }
        i++;
    }
    return con;
}

void TL1ConnectionManager::sendDataToConnection (TL1Session* session, const char* data)
{
    if ( getNumberOfAssociatedConnections(session) )
    {
        MessageLock lock (_assocMutex)
        ;
        list <Association>::iterator i = _associations.begin();
        while (i != _associations.end())
        {
            TL1Session* sessionInAssoc = (*i).session;
            TL1Connection* con = (*i).con;

            bool enabled = (*i).enabled;
            if ((sessionInAssoc == session) && (enabled))
            {
                session->logTraffic("", data, "CMD", con->getName()); // TID is extracted by logTraffic()

                string duplicateMessage ;
                ErrorLogger::maskPassword(data, duplicateMessage);
                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "sendDataToConnection", TRACE1, "Sending the following command via [%s]:\n\n%s\n", con->getName().c_str(), duplicateMessage.c_str());
                con->send_data (data);
            }
            i++;
        }
    }
}

TL1Session *TL1ConnectionManager::getSessionByTID (const string &tid)
{
    // only send back session in an enabled association
    // check for empty size
    if (tid.empty())
    {
        string error = "Could not get a session to route to as TID is empty";
        throw BaseException (error, "TL1ConnectionManager", "getSessionByTID");
    }
    MessageLock lock (_assocMutex)
    ;
    TL1Session *session = NULL;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        string tidInAssoc = (*i).tid;
        bool enabled = (*i).enabled;
        if ((tidInAssoc == tid) && (enabled))
        {
            session = (*i).session;
            break;
        }
        i++;
    }
    return session;
}

TL1Session *TL1ConnectionManager::getSessionByCTAG (const string &ctag)
{
    TL1Session *session = NULL;

    return session;
}

bool TL1ConnectionManager::hasAssociationToRemove (TL1Session *session, TL1Connection* &con)
{
    MessageLock lock (_assocMutex)
    ;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        if ( ((*i).session) == session)
        {
            con = (*i).con;
            return true;
        }
        i++;
    }
    return false;
}

void TL1ConnectionManager::removeAllAssociations (TL1Session *session)
{
    bool atLeastOneAssociation = false;
    do
    {
        TL1Connection *conInAssoc;
        atLeastOneAssociation = hasAssociationToRemove (session, conInAssoc);
        if (atLeastOneAssociation)
        {
            removeAssociation (session, conInAssoc);
            if (getNumberOfAssociatedSessions (conInAssoc) == 0)
                removeConnection (conInAssoc);
        }
    }
    while (atLeastOneAssociation);
}

//-----------------------Connection loss routines ----------------------------------------//
list<TL1Session *> TL1ConnectionManager::getLostSessions (TL1Connection *con)
{
    list<TL1Session*> sessionList;

    MessageLock lock (_assocMutex)
    ;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        bool enabled = (*i).enabled;
        if ( (con == conInAssoc) && (enabled) )
        {
            TL1Session *session = (*i).session;
            sessionList.push_back (session);
            //   session->connectionRecovery(con);
        }
        i++;
    }

    return sessionList;
}

void TL1ConnectionManager::reportConnectionLossToSessions (TL1Connection *con)
{
    MessageLock lock (_assocMutex)
    ;
    string connectionName = con->getName();
    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "reportConnectionLossToSessions", TRACE1, "Reporting loss of connection to [%s] to all sessions", connectionName.c_str());
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        bool enabled = (*i).enabled;
        if (con == conInAssoc)
        {
            TL1Session *session = (*i).session;
            // notify all sessions about the connection loss
            string connectionName = con->getName();
            session->connectionLoss(connectionName);
            // notify all sessions using this connection to drop
            if (enabled)
                session->closeSession();
        }
        i++;
    }
}

void TL1ConnectionManager::handleConnectionLoss (TL1Connection *con)
{
    string connectionName = con->getName();
    if ( (connectionName == "CM") || (connectionName == "FM") )
        return ;
    reportConnectionLossToSessions (con);
    addConnectionToBeRecovered (con);
}

void TL1ConnectionManager::dropActiveConnection (TL1Session *session)
{
    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "dropActiveConnection", __LINE__, TRACE1, "Started...");
    MessageLock lock (_assocMutex)
    ;
    TL1Connection *con = NULL;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Session *sessionInAssoc = (*i).session;
        bool enabled = (*i).enabled;
        if ((sessionInAssoc == session) && (enabled))
        {
            ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "dropActiveConnection", __LINE__, TRACE1, "Connection for session found.");
            con = (*i).con;
            break;
        }
        i++;
    }
    TL1Connection *activeConnection = con;
    // if an active connection was being used by this session, notify the session
    if (activeConnection)
    {
        activeConnection->clearCmdMapForNe (session->getNeId());
        session->closeSession();
    }
    else
        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "dropActiveConnection", __LINE__, TRACE1, "No connection found for session... not calling closeSession.");


}

TL1Connection* TL1ConnectionManager::getStandbyConnection (TL1Session *session, TL1Connection *current, string &standbyConnectionName)
{
    MessageLock lock (_assocMutex)
    ;

    if (current)
    {
        if (current->isActive())
            ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "We are currently using an ACTIVE connection to %s", current->getName().c_str());
        else
            ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "We are currently using an INACTIVE connection to %s", current->getName().c_str());
    }
    else
    {
        ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "We are currently using NO ACTIVE connections");
    }

    list <TL1Connection *> otherConnections;
    list <Association>::iterator i = _associations.begin();

    // build a list of other connections
    while (i != _associations.end())
    {
        TL1Session *sessionInAssoc = (*i).session;
        if (sessionInAssoc == session)
        {
            TL1Connection *thisCon = (*i).con;
            if (thisCon != current)
            {
                if (thisCon->isActive())
                    ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "We have an alternate ACTIVE connection to %s", thisCon->getName().c_str());
                else
                    ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "We have an alternate INACTIVE connection to %s ", thisCon->getName().c_str());
                otherConnections.push_back ( thisCon );
            }
        }
        i++;
    }

    // no go through the list and pick an active connection
    while (otherConnections.size() > 0)
    {
        TL1Connection *thisCon = otherConnections.front();
        otherConnections.pop_front();
        if (thisCon->isActive())
        {
            standbyConnectionName = thisCon->getName();
            ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "Returning %s as the new ACTIVE connection", standbyConnectionName.c_str());
            return thisCon;
        }
        else
        {
            string mode = Server_i::instance()->getDefaultConfigValue("NEALWAYSALIVEMODE", "NO");
            if (0 == mode.compare("YES"))
            {
                standbyConnectionName = thisCon->getName();
                ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "Returning %s as the new ACTIVE connection in NEALWAYSALIVEMODE", standbyConnectionName.c_str());
                return thisCon;
            }
        }
    }

    // we will get here only if no other connection was active
    if (current)
    {
        if (current->isActive())
        {
            standbyConnectionName = current->getName();
            ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "Returning the original (%s) as the new ACTIVE connection", standbyConnectionName.c_str());
            return current;
        }
    }

    ErrorLogger::logVarError( sourceName, "TL1ConnectionManager", "getStandbyConnection", TRACE1, "Could not find any active connection. Returing NULL");
    return NULL;
}

list<TL1Session *> TL1ConnectionManager::getSessionsToRecover (TL1Connection *con)
{
    list<TL1Session*> sessionList;

    MessageLock lock (_assocMutex)
    ;
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        if (con == conInAssoc)
        {
            TL1Session *session = (*i).session;
            sessionList.push_back (session);
            //   session->connectionRecovery(con);
        }
        i++;
    }

    return sessionList;
}

void TL1ConnectionManager::handleConnectionRecovery (TL1Connection *con)
{
    string connectionName = con->getName();
    string message = "Recovered socket connection to host at " + connectionName;
    string servername = Server_i::instance()->getServerName();

    MessageLock lock(_assocMutex);
    list <Association>::iterator i = _associations.begin();
    while (i != _associations.end())
    {
        TL1Connection *conInAssoc = (*i).con;
        if (con == conInAssoc)
        {
            string connectionName = con->getName();
            TL1Session *session = (*i).session;
            session->connectionRecovery(connectionName);
        }
        i++;
    }
}

void TL1ConnectionManager::addInactiveConnectionsToBeRecoverd (TL1Session *session)
{
    list<TL1Connection*> potentialRecoverableConnections;
    
    try
    {
        MessageLock assoclock(_assocMutex);
        list <Association>::iterator i = _associations.begin();
        while (i != _associations.end())
        {
            TL1Session *sessionInAssoc = (*i).session;
            bool enabled = (*i).enabled;
            // we want to recover connections in disabled associations
            if ((sessionInAssoc == session) && (enabled == false))
            {
                TL1Connection *con = (*i).con;
                if ((con) && (con->isActive() == false))
                {
                    string name = con->getName();
                    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addInactiveConnectionsToBeRecoverd", TRACE1, "Adding connection to host [%s] for recovery", name.c_str());
                    potentialRecoverableConnections.push_back(con);
                }
            }
            i++;
        }
    }
    catch(...)
    {
    }

    try
    {
        MessageLock connectionslock(_messageMutex);
        for (list<TL1Connection*>::iterator itPotential = potentialRecoverableConnections.begin(); itPotential != potentialRecoverableConnections.end(); itPotential++)
        {
            bool connectionFound = false;

            for (list<TL1Connection *>::iterator i = _recoverableConnections.begin();i != _recoverableConnections.end(); i++)
            {
                TL1Connection* conInList = (*i);
                if (conInList == (*itPotential))
                {
                    connectionFound = true;
                    break;
                }
            }

            if (!connectionFound)
            {
                _recoverableConnections.push_back((*itPotential));
                string name = (*itPotential)->getName();
                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "addInactiveConnectionsToBeRecoverd", TRACE1, "Connection to host %s added for recovery", name.c_str());
            }
        }
    }
    catch(...)
    {
    }
}


void TL1ConnectionManager::removeFromRecoverableList (TL1Connection *con)
{
    ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeFromRecoverableList", __LINE__,
                             TRACE4, "removeFromRecoverableList begins ...");
    try
    {
        MessageLock lock (_messageMutex)
        ;
        for (list<TL1Connection *>::iterator i = _recoverableConnections.begin();i != _recoverableConnections.end();i++)
        {
            if (con == (*i))
            {
                string connName = con->getName();
                ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeFromRecoverableList", __LINE__,
                                         TRACE10, "Found the connection (%s) to be removed. ", connName.c_str());
                _recoverableConnections.erase(i);
                break;
            }
        }
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1ConnectionManager", "removeFromRecoverableList", __LINE__, MINOR, "Exception happen on releaseReceiver->%s", ex.description(ex.code()));
    }

}

void TL1ConnectionManager::configureMetering(const string& tid, bool enable, short delayPeriodForBlock, short percentDropForUnblock)
{
    MessageLock lock (_assocMutex)
    ;
    for (list <Association>::iterator i = _associations.begin(); i != _associations.end(); i++)
    {
        if (i->tid == tid && i->enabled)
        {
            i->session->configureMetering(enable, delayPeriodForBlock, percentDropForUnblock);
        }
    }
}

void TL1ConnectionManager::unblockMetering(const string& neId)
{
    MessageLock lock (_assocMutex);

    for (list <Association>::iterator i = _associations.begin(); i != _associations.end(); i++)
    {
        if (i->session->getNeId() == neId && i->enabled)
        {
            i->session->unblockMetering();
        }
    }
}
