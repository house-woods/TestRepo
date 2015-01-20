
#include "Server_i.h"
#ifdef SHARED
    #undef SHARED // Don't know who #defines this, but OCI doesn't like it
#endif

#include "ace/SSL/SSL_Context.h"

#include "fmi.hh"
#include "swalarmreceiver.hh"
#include "sysmonS.hh"

#include "BaseException.h"
#include "ComManager.h"
#include "ConfigFile.h"
#include "ConfigNames.h"
#include "ConfigObject.h"
#include "ConnectionMgr.h"
#include "CORBAUtil.h"
#include "EMSDefines.h"
#include "EmsUnixDefines.h"
#include "NetworkElement.h"
#include "NotificationManager.h"
#include "OracleConnMgr.h"
#include "RDbConfigNames.h"
#include "RDbFactory.h"
#include "RDbMgr.h"
#include "SystemConfigMgr.h"
#include "TimeCollector.h"
#include "ttime.h"
#include "tutil.h"

using namespace EMS_FW;
using namespace ELogger;
using namespace RDb;

static const char* className = "Server_i";
extern string NameOfServer;
extern string pgm;

Server_i* Server_i::_instance;

static osi_fifo_mutex _registerMutex;

/*****************************************************************
 * Default constructor
 *
 *****************************************************************/
Server_i::Server_i()
{
    _instance = this;
    _state = SysMon::inactive;
    _name = "Server";
    _primarySysmon = false;
}

/*****************************************************************
 * Copy constructor
 *
 *****************************************************************/
Server_i::Server_i(const Server_i &right)
{
}

/*****************************************************************
 * Destructor
 *
 *****************************************************************/
Server_i::~Server_i()
{
}

void Server_i::initialize ()
{
}

void Server_i::finalize ()
{
    ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1,
                             "Server stopped..");
}

void Server_i::configurationChanged ()
{
    ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1,
                             "Server configuration changed...");
}

void Server_i::run ()
{
}

/*****************************************************************
 * This method is responsible for
 * - copying the configuration from corba structure to the config map.
 * - calling initialize method for starting the server
 * - setting the state as active.
 *
 *****************************************************************/
SysMon::State Server_i::startServer (const SysMon::ServerConfiguration& configuration)
{
    const char *methodName = "startServer";

    try
    {
        _configuration.clear();
        int length = configuration.length();
        string emsId = "";

        bool debugMode = false; // for debugging those nasty startup problems
        for (int i = 0;i < length;i++)
        {
            const string& name = (const char*) configuration[i].name;
            const string& value = (const char*) configuration[i].value;

            if (name == EMS_FW::ConfigNames::ServerNameKey)
            {
                _name = value;
            }
            else if (name == EMS_FW::ConfigNames::EmsIdKey)
            {
                emsId = value;
            }
            else if (name == "debug" && value == "true")
            {
                debugMode = true;
            }

            setConfigValue(name, value);
        }

        const string& traceLevel = getConfigValue(ConfigNames::TraceLevelKey);
        const string& logSize = getConfigValue(ConfigNames::LogSizeKey);
        const string& logDir = getConfigValue(ConfigNames::LogDirectoryKey);
        string logName = logDir + EMSDEF::SLASH + _name + ".log";
        ErrorLogger::instance()->disableTrace(0x1fff);
        ErrorLogger::instance()->enableTrace(atol(traceLevel.c_str()));

        bool precisionProfilingEnabled = ( (getConfigValue (ConfigNames::PrecisionProfilingEnabledKey)) == ConfigNames::True);
        string profileLogFileName = logDir + EMSDEF::SLASH + _name + "_profile.log";
        TimeCollector::initializeDefaultProfile (precisionProfilingEnabled, profileLogFileName);

        string needRdbProfile = getDefaultConfigValue("NeedRdbProfile", "", ConfigNames::True);
        if (needRdbProfile == ConfigNames::True)
        {
            profileLogFileName = logDir + EMSDEF::SLASH + _name + "_RdbProfile.log";
            TimeCollector::initialize (true, profileLogFileName);
            setConfigValue("RdbProfileName", profileLogFileName);
        }

        ErrorLogger::logError(getServerName().c_str(), className, methodName, TRACE4, "Entering..." );

        // Define the name of the server for the signal handler
        NameOfServer = getServerName().c_str();

        if (debugMode)
        {
            ErrorLogger::logError(getServerName().c_str(), className, methodName, __LINE__, TRACE1, "Running in debug mode; Attach a debugger and set keepWaiting to false to continue ...");
            bool keepWaiting = true;
            while (keepWaiting)
            {
                osi_this_thread::milli_sleep(1000);
            }
        }
        else
            ErrorLogger::logError(getServerName().c_str(), className, methodName, __LINE__, TRACE1, "Running in non-debug mode");

        for (int counter = 0 ; counter < length ; counter++)
        {
            string name = (const char*) configuration[counter].name;
            string value = (const char*) configuration[counter].value;
            ErrorLogger::logVarError(getServerName().c_str(), className, methodName, __LINE__, TRACE1,
                                     "Config Name=[%s] Value=[%s]", name.c_str(), value.c_str() );
        }

        ComManager::instance()->setRootContext(emsId.c_str());

        if (_primarySysmon == false)
        {
            ComManager::instance()->bindObject(_serverPtr, _name.c_str());
        }

        _state = SysMon::active;
        ErrorLogger::instance()->disableTrace(0x1FFF);
        ErrorLogger::instance()->enableTrace(atol(traceLevel.c_str()));
        string heapCheck = getDefaultConfigValue(ConfigNames::GlobalHeapCheckKey, "", ConfigNames::False);
        if (heapCheck == ConfigNames::True)
        {
            // enable heap check for debug purposes
            ErrorLogger::instance()->setCheckHeap();
        }
        initialize();
        string serverId = getDefaultConfigValue(ConfigNames::IdKey, "0");
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, TRACE1,
                                 "Server %s:Started", getServerName().c_str());
    }
    catch (BaseException& ex)
    {
        _state = SysMon::failed;
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR,
                                 "Base exception thrown %s", ex.getDescription().c_str());
        throw SysMon::SysMonException(0, ex.getDescription().c_str() );
    }
    catch (...)
    {
        _state = SysMon::failed;
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, __LINE__, MAJOR,
                                 "unknown exception received");
        throw SysMon::SysMonException(0, "Unknown error ");
    }

    ErrorLogger::logError(getServerName().c_str(), className, methodName, TRACE4, "Returning..." );
    return _state;
}

/*****************************************************************
 * This method is responsible for
 * - updating the configuration from corba structure to the config map.
 * - calling configurationChanged method .
 *
 *****************************************************************/
SysMon::State Server_i::setServerConfiguration (const SysMon::ServerConfiguration& configuration)
{
    _configuration.clear();
    int length = configuration.length();
    for (int i = 0;i < length;i++)
    {
        string name = (const char*) configuration[i].name;
        string value = (const char*) configuration[i].value;
        setConfigValue(name, value);
    }
    configurationChanged();
    return _state;
}

/*****************************************************************
 * This method is responsible for
 * - converting and returning configuration stored in the config map.
 *
 *****************************************************************/
SysMon::ServerConfiguration* Server_i::getServerConfiguration ()
{
    SysMon::Configuration *config = new SysMon::Configuration();
    int length = _configuration.size();
    config->length(length);
    map<string, string>::iterator iter;
    int i = 0;
    for (iter = _configuration.begin(); iter != _configuration.end(); iter++)
    {
        (*config)[i].name = CORBA::string_dup((*iter).first.c_str());
        (*config)[i].value = CORBA::string_dup((*iter).second.c_str());
        i++;
    }
    return config;
}

/*****************************************************************
 * This method is responsible for
 * - stopping all the network elements
 * - removing network element from its map
 * - calling finalize method for stopping the server
 * - deleting the state change chaneel
 * - setting the state as inactive.
 *
 *****************************************************************/
SysMon::State Server_i::terminateServer ()
{
    TimeCollector::finalize();

    try
    {
        list<NetworkElement*>::iterator iter;

		finalize();
		
        _networkElements.clear();

        _configuration.clear();

        _state = SysMon::inactive;

        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1,
                                 "Server %s:Stopped", getServerName().c_str() );
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 "Unknown exception received...");
    }

    _state = SysMon::inactive;
    return _state;
}


/*****************************************************************
 * This method is responsible for
 * - returning the current state of the server.
 *
 *****************************************************************/
SysMon::State Server_i::ping ()
{
    ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE2,
                             "Server pinging ok ... ");
    return _state;
}

/*****************************************************************
 * This method is responsible for
 * - creating the appropriate instance of network element using a factory method
 * - adding it to the list of network elemets
 * - starting the network element
 *
 *****************************************************************/
CORBA::Boolean Server_i::addNetworkElement (const SysMon::NetworkElementConfiguration& configuration)
{
    try
    {
        const string &neId = getNetworkElementId (configuration);
        NetworkElement* ne = createNetworkElement();
        // set network element to read-only when it is added
        ne->setReadOnly(true);
        try
        {
            // added for MR: 08267 to protect the networkElements list
            osi_sem_write_lock lock (neListSem)
            ;
            _networkElements.insert (map<string, NetworkElement*>::value_type (neId, ne));
        }
        catch (osi_thread_toolkit_error& ex)
        {
            string desc = "Exception happened on sem write lock. ";
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", "addNetworkElement", __LINE__, MINOR, "Exception happen on addNetworkElement->%s", ex.description(ex.code()));
            throw BaseException(desc, "Server_i", "addNetworkElement");

        }

        ne->start(configuration);
        string serverId = getConfigValue(ConfigNames::IdKey);
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1,
                                 "Network element added in server %s", getServerName().c_str() );
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 ex.getDescription().c_str());
        throw SysMon::SysMonException(ex.getErrCode(), ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 "Unknown exception received...");
        throw SysMon::SysMonException(0, "unknown error");

    }
    return true;
}

/*****************************************************************
 * This method is responsible for
 * - setting NE readonly true or false
 * - start resync process if readonly was set to false
 * - should be implemented in CM and FM
 *
 *****************************************************************/
CORBA::Boolean Server_i::setNeReadOnly (bool rdOnly, const char* neId)
{
    try
    {
        osi_sem_read_lock lock (neListSem)
        ;
        map<string, NetworkElement*>::iterator iter = _networkElements.find (neId);
        if (iter != _networkElements.end())
        {
            NetworkElement* ne = (*iter).second;
            ne->setReadOnly(rdOnly);
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1,
                                     "NE set to read-only state %d in server %s", rdOnly, getServerName().c_str() );
            return true;
        }
        else
            return false;
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 ex.getDescription().c_str());
        return false;

    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 "Unknown exception received...");
        return false;

    }
    return true;
}

/*****************************************************************
 * This method is responsible for
 * - locating the network element for the resource id
 * - stopping the network element
 * - removing network element from the list of network elemets
 *
 *****************************************************************/
CORBA::Boolean Server_i::removeNetworkElement (const char* resourceId)
{
    try
    {
        NetworkElement* ne = locateNetworkElement(resourceId);
        if (ne != 0)
        {
            string serverId = getConfigValue(ConfigNames::IdKey);
            ne->stop();
            try
            {
                // added for MR: 08267 to protect the networkElements list
                osi_sem_write_lock lock (neListSem)
                ;
                map<string, NetworkElement*>::iterator iter = _networkElements.find (resourceId);
                if (iter != _networkElements.end())
                    _networkElements.erase (iter);
            }
            catch (osi_thread_toolkit_error& ex)
            {
                string desc = "Exception happened on sem write lock. ";
                ErrorLogger::logVarError(getServerName().c_str(), "Server_i", "addNetworkElement", __LINE__, MINOR, "Exception happen on addNetworkElement->%s", ex.description(ex.code()));
            }
            delete ne;
            ne = NULL;
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1,
                                     "NE removed from server %s", getServerName().c_str() );
        }
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 ex.getDescription().c_str());

    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 "Unknown exception received...");

    }
    return true;
}
/*****************************************************************
 * This method is responsible for
 * - locating the network element for the resource id
 * - running the network element by calling its run operation
 *
 *****************************************************************/
CORBA::Boolean Server_i::runNetworkElement (const char* resourceId)
{
    try
    {
        NetworkElement* ne = locateNetworkElement(resourceId);
        if (ne != 0)
        {
            ne->run();
        }
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), TRACE1, "%s",
                                 ex.getDescription().c_str());
        throw SysMon::SysMonException(0, ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 "Unknown exception received...");
        throw SysMon::SysMonException(0, "Unknown error");
    }
    return true;
}

/*****************************************************************
 * This method is responsible for
 * - returning the configuration for the specified network element
 *****************************************************************/
SysMon::NetworkElementConfiguration* Server_i::getNetworkElementConfiguration (const char* resourceId)
{
    SysMon::NetworkElementConfiguration *result = new SysMon::NetworkElementConfiguration();
    return result;
}


/*****************************************************************
 * This method is responsible for
 * - setting the configuration for the specified network element
 *****************************************************************/
CORBA::Boolean Server_i::setNetworkElementConfiguration (const SysMon::NetworkElementConfiguration& configuration)
{
    try
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE2,
                                 "setNetworkElementConfiguration called");
        string id = getNetworkElementId(configuration);

        NetworkElement* ne = locateNetworkElement(id);
        if (ne != 0)
        {
            ne->setConfiguration(configuration);
        }
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Exception: %s",
                                 ex.getDescription().c_str());
        throw SysMon::SysMonException(0, ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR,
                                 "Unknown exception received...");
        throw SysMon::SysMonException(0, "unknown exception");

    }
    return true;
}

/*****************************************************************
 * This method is responsible for
 * - performing the action for the specified operationname.
 *****************************************************************/
CORBA::Boolean Server_i::execCommand (const char* operationName, const char* neIdChar, const SysMon::EMSServer::Parameters& config)
{
    ConfigObject params;
    params.setConfiguration(config);
    string neId = neIdChar;
    string command = operationName;
    if (neId.length() == 0)
    {
        if (command == CommandGenerateError && config.length() == 1)
        {
            int errorCode = atoi(config[0].value);
            sendErrorEvent(neId, "", errorCode);
        }
        else if (command == EMS_FW::ConfigNames::CommandStartLogging)
        {
            string traceLevel = params.getConfigValue(ConfigNames::TraceLevelKey);
            string logSize = params.getConfigValue(ConfigNames::LogSizeKey);
            string logDir = params.getConfigValue(ConfigNames::LogDirectoryKey);
            string logName = logDir + EMSDEF::SLASH + _name + ".log";
            LocalErrorLogger::startLogging(logName, atol(traceLevel.c_str()), atol(logSize.c_str()));
        }
        else if (command == EMS_FW::ConfigNames::CommandSetTraceLevel)
        {
            string traceLevel = params.getConfigValue(ConfigNames::TraceLevelKey);
            ErrorLogger::instance()->disableTrace(0x1fff);
            ErrorLogger::instance()->enableTrace(atol(traceLevel.c_str()));
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR, "TRACE LEVEL SET TO %s", traceLevel.c_str());
        }
        else if (command == "SetConfig" || command == EMSDEF::UpdateSGMConfig)
        {
            string configName = config[0].name.in();
            string configValue = config[0].value.in();
            setConfigValue (configName, configValue);
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR, "CONFIG NAME %s SET TO %s", configName.c_str(), configValue.c_str());
        }
        else if (command == EMS_FW::ConfigNames::CommandStopLogging)
        {
            LocalErrorLogger::stopLogging();
        }
        else if (command == CommandSleepServer)
        {
            int period = 10;
            if (config.length() == 1)
            {
                period = atoi(config[0].value);
            }
            try
            {
                osi_this_thread::milli_sleep(period*1000);
            }
            catch (osi_thread_toolkit_error& ex)
            {
                ErrorLogger::logVarError(getServerName().c_str(), "Server_i", "execCommand", __LINE__, MINOR, "Exception happen on sleep->%s", ex.description(ex.code()));
            }
        }
    }
    else
    {
        NetworkElement* ne = locateNetworkElement(neId);
        if (ne == 0)
            return false;

        if (command == CommandGenerateError && config.length() == 1)
        {
            int errorCode = atoi(config[0].value);
            sendErrorEvent(neId, "", errorCode);
        }
        else if (command == CommandNePeerInitialize)
        {
            ne->synchronizeWithPeer();
        }
        else if (command == CommandNeFcpInitialize)
        {
            ne->synchronizeWithFcp();
        }
    }
    return true;
}

CORBA::Boolean Server_i::execQuery (const char* operationName, const char*
                                    neIdChar, const SysMon::EMSServer::Parameters& config,
                                    SysMon::EMSServer::Parameters_out results)
{
    ConfigObject params;
    params.setConfiguration(config);
    string neId = neIdChar;
    string command = operationName;
    results = new SysMon::EMSServer::Parameters;

    return true;
}

void Server_i::push(const ::CORBA::Any& event, ::EMS_Event::Event_Type type)
{
    NotificationManager::instance()->push(event, type);
}

/*****************************************************************
 * This factory method is responsible for
 * - creating and returning instance of appropriate subclass of network element
 *****************************************************************/
NetworkElement* Server_i::createNetworkElement ()
{
    return new NetworkElement();
}


void Server_i::sendStateChangeEvent (const string &stateType, const map<string, string> &properties, const string &description)
{
    const char* methodName = "sendStateChangeEvent";
    const string &serverName = getServerName();
    try
    {
        SysMon::StateEx state;
        state.type = CORBA::string_dup (stateType.c_str());
        state.description = CORBA::string_dup (description.c_str());
        state.sourceName = CORBA::string_dup (serverName.c_str());

        ErrorLogger::logError(serverName.c_str(), className, methodName, __LINE__, TRACE1, "Sending the following state to SysMon via stateChange >> ");
        ErrorLogger::logVarError(serverName.c_str(), className, methodName, __LINE__, TRACE1, "Type [%s]  Description [%s]  Properties >> ", stateType.c_str(), description.c_str());

        state.properties.length (properties.size());
        int i = 0;
        for (map<string, string>::const_iterator iter = properties.begin(); iter != properties.end();iter++)
        {
            const string &name = (*iter).first;
            const string &value = (*iter).second;
            ErrorLogger::logVarError(serverName.c_str(), className, methodName, __LINE__, TRACE1, "Name [%s]  Value [%s]", name.c_str(), value.c_str());
            state.properties[i].name = CORBA::string_dup (name.c_str());
            state.properties[i].value = CORBA::string_dup (value.c_str());
            i++;
        }

        CORBA::Any stateAny;
        stateAny <<= state;
        SysMon::EMSServer_var ptr = CORBAUtil::getServerReference ("SysMon");
        ptr->stateChange (stateAny);
    }
    catch (...)
    {
        ErrorLogger::logError(serverName.c_str(), className, methodName, __LINE__, MAJOR, "Could not send the state to SysMon via stateChange");
    }
}

/*****************************************************************
 * This method is responsible for
 * - sending a state change event to sysmon
 *****************************************************************/
void Server_i::sendStateChangeEvent(SysMon::State state, const string& neId,
                                    const string& description, ELogger::ErrorCode code, const string& neVersion,
                                    const string& nePassword)
{
    const char* methodName = "sendStateChangeEvent";
    const string &serverName = getServerName();
    try
    {
        string stateType;
        switch (code)
        {
        case ELogger::NEInActive:
            stateType = "NEInActive";
            break;
        case ELogger::NEActive:
            stateType = "NEActive";
            break;
        case ELogger::ServerGroupInActive:
            stateType = "ServerGroupInActive";
            break;
        case ELogger::ServerGroupActive:
            stateType = "ServerGroupActive";
            break;
        case ELogger::NEPasswordChange:
            stateType = "NEPasswordChange";
            break;
        default:
            stateType = "unknown";
        }

        map <string, string> properties;

        string serverId = getConfigValue(ConfigNames::IdKey);
        properties.insert (map<string, string>::value_type(ConfigNames::IdKey, serverId));
        properties.insert (map<string, string>::value_type(ConfigNames::NeIdKey, neId));
        properties.insert (map<string, string>::value_type(ConfigNames::NeVersionKey, neVersion));
        properties.insert (map<string, string>::value_type(ConfigNames::Password, nePassword));
        sendStateChangeEvent (stateType, properties, description);
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(serverName.c_str(), className, methodName, __LINE__, MAJOR, "Could not prepare stateChange event; %s", ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(serverName.c_str(), className, methodName, __LINE__, MAJOR, "Could not prepare stateChange event; Unknown exception");
    }
}

/*****************************************************************
 * This method is responsible for
 * - sending an error event to sysmon
 *****************************************************************/
void Server_i::sendErrorEvent(const string& neId, const string& description , ELogger::ErrorCode code)
{
    sendStateChangeEvent(SysMon::failed, neId, description, code);
}

/*****************************************************************
 * This method is responsible for
 * - getting the integer value of the specified config parameter.
 *****************************************************************/
int Server_i::getIntConfigValue (const string& name, const string& neId)
{
    const string& result = getConfigValue(name, neId);
    if (result.empty())
        return 0;
    else
        return atoi(result.c_str());
}

/*****************************************************************
 * This method is responsible for
 * - getting the string value of the specified config parameter.
 *****************************************************************/
string Server_i::getConfigValue (const string &name)
{
    map<string, string>::const_iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", MAJOR, "Could not get value for Config Name [%s]", name.c_str());
        throw UnresolvedNameEx(name, "Could not get config value for " + name);
    }
    else
        return (*iter).second;
}

string Server_i::getConfigValue (const string& name, const string& neId)
{
    if (neId.empty())
        return (getConfigValue (name));

    try
    {
        osi_sem_read_lock lock(neListSem);
        map<string, NetworkElement*>::const_iterator iter = _networkElements.find (neId);
        if (iter != _networkElements.end())
        {
            NetworkElement* ne = (*iter).second;
            return ne->getConfigValue (name);
        }
        else
        {
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Unable to get config value from NE with id [%s] for Config Name [%s] (NE possibly removed already)", neId.c_str(), name.c_str());
            throw BaseException ("Unable to get config value for [" + name + "]; NE with id [" + neId + "] does not exist");
        }
    }
    catch (UnresolvedNameEx& ex)
    {
        throw ex;
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Unable to get config value from NE with id [%s] for [%s] (Unknown error)", neId.c_str(), name.c_str());
        throw BaseException ("Unable to get config value for [" + name + "]; Unknown error");
    }
}

void Server_i::setConfigValue (const string &name, const string &neId, const string &value)
{
    if (neId.size() == 0)
        setConfigValue (name, value);
    else
    {
        try
        {
            osi_sem_read_lock lock(neListSem);
            map<string, NetworkElement*>::iterator iter = _networkElements.find (neId);
            if (iter != _networkElements.end())
            {
                NetworkElement* ne = (*iter).second;
                ne->setConfigValue (name, value);
                return;
            }
            else
            {
                ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Unable to set config value [%s] for name [%s] for NE with id [%s] (NE possibly resmoved already)", value.c_str(), name.c_str(), neId.c_str());
            }
        }
        catch (...)
        {
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Unable to set config value [%s] for name [%s] for NE with id [%s] (Unknown error)", value.c_str(), name.c_str(), neId.c_str());
        }
    }
}

bool Server_i::neExists (const string &neId)
{
    osi_sem_read_lock lock(neListSem);
    map<string, NetworkElement*>::iterator iter = _networkElements.find (neId);
    if (iter == _networkElements.end())
        return false;
    else
        return true;
}

/*****************************************************************
 * This method is responsible for
 * - getting the string value of the specified config parameter.
 * - or return default value if config parameter does not exist
 *****************************************************************/
string Server_i::getDefaultConfigValue (const string& name, const string& neId, const string& defaultValue)
{
    if (neId.empty())
        return (getDefaultConfigValue(name, defaultValue));

    try
    {
        osi_sem_read_lock lock(neListSem);
        map<string, NetworkElement*>::const_iterator iter = _networkElements.find (neId);
        if (iter != _networkElements.end())
        {
            NetworkElement* ne = (*iter).second;
            return ne->getDefaultConfigValue (name, defaultValue);
        }
        else
        {
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Unable to get config value from NE with id [%s] for Config Name [%s] (NE possibly removed already); returning default value [%s]", neId.c_str(), name.c_str(), defaultValue.c_str());
            return defaultValue;
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE1, "Unable to get config value from NE with id [%s] for [%s] (Unknown error); returning default value [%s]", neId.c_str(), name.c_str(), defaultValue.c_str());
        return defaultValue;
    }
}

/*****************************************************************
 * This method is responsible for
 * - getting the string value of the specified config parameter.
 * - or return default value if config parameter does not exist
 *****************************************************************/
string Server_i::getDefaultConfigValue (const string& name, const string& defaultValue)
{
    map<string, string>::iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", TRACE3, "Unable to get config value for [%s]; returning default value [%s]", name.c_str(), defaultValue.c_str());
        return defaultValue;
    }
    else
        return (*iter).second;
}

void Server_i::logEvent ()
{
}

void Server_i::main ()
{
}

void Server_i::start ()
{
}

/*****************************************************************
 * This method is responsible for
 * - locating the network element for the specified resource id.
 *
 *****************************************************************/
NetworkElement* Server_i::locateNetworkElement (const string& id)
{
    try
    {
        osi_sem_read_lock lock(neListSem);
        map<string, NetworkElement*>::iterator iter = _networkElements.find (id);
        if (iter != _networkElements.end())
            return (*iter).second;
        else
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", "locateNetworkElement",
                                     __LINE__, MINOR, "Could not locate network element with id [%s]; NE possibly removed already", id.c_str());
    }
    catch (osi_thread_toolkit_error& ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", "locateNetworkElement", MINOR,
                                 "Could not locate network element with id [%s]; [%s]",
                                 id.c_str(), ex.description(ex.code()));
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), "Server_i", "locateNetworkElement", MINOR,
                                 "Could not locate network element with id [%s]; Unknown error", id.c_str());
    }
    return NULL;
}

/*****************************************************************
 * This method is responsible for
 * - returning NeId from the corba configuration structure.
 *
 *****************************************************************/
string Server_i::getNetworkElementId (const SysMon::Configuration& config)
{
    string id;
    int length = config.length();
    for (int i = 0;i < length;i++)
    {
        string name = (const char*)config[i].name;
        string value = (const char*)config[i].value;
        if (name == EMS_FW::ConfigNames::NeIdKey)
        {
            id = value;
            break;
        }
    }
    return id;
}

/*****************************************************************
 * This method is responsible for
 * Displaying the version of the server.
 *
 *****************************************************************/
void Server_i::DisplayVersionReport()
{
    cout << endl << "Please do \"what ExecutableName\" to get version information." << endl;
}

// This is utility method used when the server is started manually
// to break the orb parameters string into words.

void fillParams(const string& text, vector<string>& words)
{
    words.clear();
    int n = text.length();
    int start, stop;
    string separators(" ");
    start = text.find_first_not_of(separators);
    cout << "Command parameters are:" << endl;
    while ((start >= 0) && (start < n))
    {
        stop = text.find_first_of(separators, start);
        if ((stop < 0) || (stop > n))
            stop = n;
        words.push_back(text.substr(start, stop - start));
        cout << text.substr(start, stop - start) << endl;
        start = text.find_first_not_of(separators, stop + 1);
    }
    cout << "These are all parameters" << endl;
}

/*****************************************************************
 * This method is called by main() and responsible for
 * - initializing the communication manager
 * - registering the server
 * - creating the corba tie for the EMSServer interface
 * - registering this tie object in the naming service
 * - waiting for the corba events.
 *
 *****************************************************************/

void Server_i::connectToDatabase (const string &serverName)
{
    const string &username = Tutil::getEnvVariableValue (RDbConfigNames::RDbUserName);
    const string &passwordFile = Tutil::getEnvVariableValue (RDbConfigNames::RDbPasswordFile);
    const string &connecString = Tutil::getEnvVariableValue (RDbConfigNames::RDbConnectString);
    const string &connectionPoolEnabled = Tutil::getEnvVariableValue (RDbConfigNames::RDbConnectionPool);

    ConnectionMgr* mgr = new OracleConnMgr (connecString, username, passwordFile, "", connectionPoolEnabled, serverName);
    ConnectionMgr::registerConnMgr(mgr);
}

void Server_i::startLogging (const string &serverName)
{
    const string &baseDirectory = SystemConfigMgr::instance()->getConfig ("GLOBAL", "", "BaseDirectory");
    const string &logDirectory = SystemConfigMgr::instance()->getDefaultConfig ("GLOBAL", "", "LogDirectory", "EMS/log");
    const string &logName = baseDirectory + EMSDEF::SLASH + logDirectory + EMSDEF::SLASH + serverName + ".log";
    const string &traceLevel = SystemConfigMgr::instance()->getDefaultConfig("HM", "", ConfigNames::TraceLevelKey, "3");
    const string &logSize = SystemConfigMgr::instance()->getDefaultConfig("GLOBAL", "", ConfigNames::LogSizeKey, "100000");

    // start out with a trace level of 3
    LocalErrorLogger::startLogging(logName, atol(traceLevel.c_str()), atol(logSize.c_str()));
}

void Server_i::initCommManager ()
{
    const string &sslSupportEnabledString = SystemConfigMgr::instance()->getDefaultConfig ("GLOBAL", "", "SSLSupportEnabled", "true");
    bool sslSupportEnabled = (sslSupportEnabledString == "true");
    ComManager::instance (sslSupportEnabled);
}

bool Server_i::isRTCorbaEnabled()
{
    return false;
}

void Server_i::execute(int argc, char** argv, bool multithreaded)
{
    const char* methodName = "execute";
    // Store the name of the program
    pgm = argv[0];

    if (argc < 3)
        throw (BaseException ("Invalid number of arguments; there should be at least 3"));

    connectToDatabase(argv[2]);

    startLogging (argv[2]);

    initCommManager ();

    ComManager* comm_manager = ComManager::instance ();

    bool rtCORBAEnabled = isRTCorbaEnabled();

    if (rtCORBAEnabled)
    {
        comm_manager->initializeRTORB (argc, argv);
        setServerName( static_cast<const char*>(argv[2]) );

        ComManager::Policies policies;
        _servant_poa = comm_manager->create_rtpoa_with_properties(string("POA_").append(getServerName()), policies );

        POA_SysMon::EMSServer_tie<Server_i> tie_servant(this, _servant_poa);

        CORBA::Object_var tempObj = comm_manager->activateObject(_servant_poa, &tie_servant);

        if (CORBA::is_nil(tempObj))
            exit(1);

        _serverPtr = SysMon::EMSServer::_narrow(tempObj);
        comm_manager->bindObject(_serverPtr, getServerName());

        int isReadyTimeOutInSeconds = atoi (Server_i::instance()->getDefaultConfigValue("IsReadyCallTimeoutInSeconds", "30").c_str());
        HM::HostManager_var hm = CORBAUtil::getHMServerReference(Tutil::getHostname(), isReadyTimeOutInSeconds);

        if (CORBA::is_nil (hm))
        {
            ErrorLogger::logVarError(getServerName().c_str(), "Server_i", methodName, MAJOR,
                                     "Exception: Null pointer returned while getting HM server reference");
            throw BaseException("Null pointer returned while getting HM server reference",
                                "Server_i", "execute");
        }
        hm->isReady(getServerName().c_str());
        comm_manager->run();

        return ;
    }

    //
    // At this time argc/argv lacks any -ORB paramters.
    //
    // Below is the code directly taken from the previous implementation.
    // There are a lot of places for improvements...
    //

    bool _startedBySysmon;
    SysMon::Configuration config;
    bool startServerFlag = false;
    bool addNeFlag = false;

    if (argc >= 3)
    {
        // started by sysmon
        comm_manager->initializeORB(argc, argv);
        setServerName( static_cast<const char*>(argv[2]) );
        _startedBySysmon = true;
    }
    else if ((argc == 2) && (stricmp(argv[1], "-v") == 0))
    {
        //  Display server version
        DisplayVersionReport();
        _startedBySysmon = true;
        exit(1);
    }
    else if (argc == 1)
    {
        cout << "Usage: " << endl;
        cout << "servername -v (display server version)" << endl;
        _startedBySysmon = false;
        exit(0);
    }
    else
    {
        _startedBySysmon = false;
        ConfigFile configFile(argv[1]);
        configFile.getConfiguration(config);
        string orbParams = configFile.getConfigValue("TEST_ORB_PARAMS");
        vector<string> params;
        fillParams(orbParams, params);
        int argc1 = params.size();
        char* argv1[30]; // no more than 30 params for now
        for (int count = 0; count < argc1; count++)
            argv1[count] = (char*) params[count].c_str();
        // initialize the com manager using the test parameters
        comm_manager->initializeORB(argc1, argv1);
        setServerName( configFile.getConfigValue(ConfigNames::ServerNameKey) );
        string startServerString = configFile.getConfigValue("StartServer");
        if (startServerString == ConfigNames::True)
            startServerFlag = true;

        string addNeString = configFile.getConfigValue("AddNe");
        if (addNeString == ConfigNames::True)
            addNeFlag = true;
    }

    ComManager::Policies policies;
    if (multithreaded == true)
    {
        policies[ComManager::thread_policy] = ComManager::orb_ctrl_model;
    }
    else
    {
        policies[ComManager::thread_policy] = ComManager::single_thread_model;
    }

    try
    {
        _servant_poa = comm_manager->create_poa_with_properties(
                           string("POA_").append(getServerName()),
                           policies );
    }
    catch (BaseException&)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR,
                                 "Exception: Unable to create servant's POA: wrong policy used. Aborting initialization...");
        throw;
    }

    //using tie approach, i.e. delegation
    POA_SysMon::EMSServer_tie<Server_i> tie_servant(this, _servant_poa);

    // PortableServer::ServantBase_var servant = tie_servant;

    CORBA::Object_var tempObj = comm_manager->activateObject(
                                    _servant_poa,
                                    &tie_servant);

    if (CORBA::is_nil(tempObj))
        exit(1);

    _serverPtr = SysMon::EMSServer::_narrow(tempObj);

    comm_manager->bindObject(_serverPtr, getServerName());

    ACE_SSL_Context* ctx = ACE_SSL_Context::instance();
    SSL_CTX_set_mode(ctx->context(), SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);

    if (_startedBySysmon == false)
    {
        // simulate sysmon behavior..
        try
        {
            if (startServerFlag == true)
            {
                startServer(config);
                run();
            }
        }
        catch (...)
        {
            ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR,
                                     "Exception: StartServer #############################");
        }

        if (addNeFlag == true)
        {
            addNetworkElement(config);
        }
    }
    else
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, TRACE1, "Ontaining HM reference");
        int isReadyTimeOutInSeconds = atoi (getDefaultConfigValue("IsReadyCallTimeoutInSeconds", "30").c_str());
        HM::HostManager_var hm = CORBAUtil::getHMServerReference(Tutil::getHostname(), isReadyTimeOutInSeconds);
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, TRACE1, "Atteempting to tell HM that I am ready with a timeout of [%d] seconds", isReadyTimeOutInSeconds);
        hm->isReady(getServerName().c_str());
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, TRACE1, "Told HM that I was ready successfully");
    }

    // Start processing requests.
    //
    ComManager::instance()->run();
}

/*****************************************************************
 * This method is responsible for
 * - setting the integer value of the specified config parameter.
 *
 *****************************************************************/
void Server_i::setIntConfigValue (const string& name, int value)
{
    char buffer[128];
    itoa(value, buffer, 10);
    map<string, string>::iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        _configuration.insert( pair <string, string> (name, buffer));
    }
    else
        (*iter).second = buffer;
}
/*****************************************************************
 * This method is responsible for
 * - setting the value of the specified config parameter.
 *
 *****************************************************************/
void Server_i::setConfigValue (const string& name, const string& value)
{
    map<string, string>::iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        _configuration.insert(pair <string, string> (name, value));
    }
    else
        (*iter).second = value;
}
/*****************************************************************
 * This method is responsible for
 * - returning the sole instance of this class
 *
 *****************************************************************/
Server_i* Server_i::instance ()
{
    if (!_instance)
    {
        _instance = new Server_i();
    }
    return _instance;
}

/*****************************************************************
 * This method is responsible for
 * - getting the component for the specified name
 *
 *****************************************************************/
EMS_FW::FWComponent* Server_i::getComponent(const string& name, const string& neId)
{
    if (neId.size() == 0)
    {
        map<string, EMS_FW::FWComponent*>::iterator iter = _components.find(name);
        if (iter == _components.end())
        {
            return NULL; // could not found a resource
        }
        else
            return (*iter).second;
    }
    NetworkElement* ne = locateNetworkElement(neId);
    if (ne)
        return ne->getComponent(name);
    return NULL; // ATTENTION need to throw exception here
}
/*****************************************************************
 * This method is responsible for
 * - registering the component
 *
 *****************************************************************/
void Server_i::registerComponent(EMS_FW::FWComponent* component)
{
    if (component == NULL)
    {
        ErrorLogger::logError(getServerName().c_str(), "Server_i", "unregisterComponent", MAJOR, "component is NULL...");
        throw BaseException("component is NULL", "Server_i", "unregisterComponent");
    }
    _components.insert(pair <string, FWComponent *> (component->getName(), component));
}
/*****************************************************************
 * This method is responsible for
 * - unregistering the component
 *
 *****************************************************************/
void Server_i::unregisterComponent(EMS_FW::FWComponent* component)
{
    if (component == NULL)
    {
        ErrorLogger::logError(getServerName().c_str(), "Server_i", "unregisterComponent", MAJOR, "component is NULL...");
        throw BaseException("component is NULL", "Server_i", "unregisterComponent");
    }
    _components.erase(component->getName());
}

/*****************************************************************
 * This method is responsible for
 * - checking if all the specified parameters has value in the config map.
 *
 *****************************************************************/
bool Server_i::checkMap(const list<string>& mapList, string& missingStrings)
{
    list<string>::const_iterator p;
    missingStrings = "MissingStrings in a Map->";
    bool res = true;
    for (p = mapList.begin(); p != mapList.end(); p++)
    {
        try
        {
            getConfigValue(*p);
        }
        catch (UnresolvedNameEx&)
        {
            missingStrings += *p;
            missingStrings += " ";
            res = false;
        }
    }
    return res;
}
/*****************************************************************
 * This method is responsible for
 * - printing server configuration.
 *
 *****************************************************************/
void Server_i::displayConfiguration()
{
    map<string, string>::iterator iter;
    for (iter = _configuration.begin(); iter != _configuration.end(); iter++)
    {
        string msg = "" + (*iter).first + "=" + (*iter).second;
        ErrorLogger::logError(getServerName().c_str(), "Server_i", TRACE1, msg.c_str());
    }
}

/*****************************************************************
 * This method is responsible for
 * - returning true if server is in debug mode.
 *
 *****************************************************************/
bool Server_i::isDebugMode()
{
    bool result = false;
    try
    {
        string mode = Server_i::getConfigValue("DebugMode");
        if (mode == EMS_FW::ConfigNames::True)
            result = true;

    }
    catch (...)
    {
    }

    return result;
}

/*****************************************************************
 * This method is responsible for
 * - returning the name of the server
 *
 *****************************************************************/
const string& Server_i::getServerName()
{
    return _name;
}

/*********************************************************************************
 * This method is responsible for
 * - Generating software alarm and sending it to the Fml associated with software ne.
 ********************************************************************************/
void Server_i::generateSwAutonomous (const string &condType, const string &source, const string &description, const string &eventType)
{
    const char *methodName = "generateSwAutonomous";

    string probableCause = condType;
    string errorMsg = description;
    ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE4, "Entering..." );

    // this is being defaulted to true so software alarms are enabled by default
    // unless explicitly disabled in any of the server.cfg files
    string startSa = Server_i::instance()->getDefaultConfigValue(ConfigNames::StartSoftwareAlarmNeKey, "", ConfigNames::True);
    if (startSa != ConfigNames::True)
        return ;
    FM::AlarmDetail* pAlarmDetail = new FM::AlarmDetail();

    pAlarmDetail->notificationID = (CORBA::ULong) 0;

    pAlarmDetail->neId = CORBA::string_dup("SoftwareAlarmNe");
    pAlarmDetail->src = CORBA::string_dup(source.c_str());

    string fullMoi = "0";
    string moc = "0";

    pAlarmDetail->managedObjectClass = CORBA::string_dup(moc.c_str());
    pAlarmDetail->managedObjectInstance = CORBA::string_dup(fullMoi.c_str());
    pAlarmDetail->typeOfEvent = CORBA::string_dup(eventType.c_str());
    pAlarmDetail->state = FM::AlarmState(1);
    if (eventType == "EMSALARM")
    {
        switch (getSeverityFromProfilEntry (source, condType))
        {
        case (SWAlarmProfile::nr):
        {
            delete pAlarmDetail;          // don't generate an alarm
            return ;
        }
        break;
        case (SWAlarmProfile::minor):
                        pAlarmDetail->perceivedSeverity = FM::Severity(3); // minor
            break;
        case (SWAlarmProfile::major):
                        pAlarmDetail->perceivedSeverity = FM::Severity(2); // major
            break;
        case (SWAlarmProfile::critical):
                        pAlarmDetail->perceivedSeverity = FM::Severity(1); // critical
            break;
        default:
            pAlarmDetail->perceivedSeverity = FM::Severity(2);
            break;
        }
    }
    else
        // Must be set to warning for all events for them to appear in the event window
        pAlarmDetail->perceivedSeverity = FM::Severity(4);

    osi_time_and_date timeDate;
    char str[40];
    timeDate = osi_time_and_date::now();
    osi_date Date = timeDate.utc_date();
    osi_time Time = timeDate.utc_time();
    EMS_FW::TTime::GetTimeInAgentFormat(Date, Time, str);
    pAlarmDetail->eventTime = CORBA::string_dup(str);
    pAlarmDetail->probableCause = CORBA::string_dup(probableCause.c_str());


    pAlarmDetail->indication = FM::TrendIndication(1);
    FM::SpecificProblemList* pSpecific = new FM::SpecificProblemList;
    pAlarmDetail->specificProblems = *pSpecific ;
    pAlarmDetail->backedUpStatus = 1;
    pAlarmDetail->backedUpObject.attributeId = CORBA::string_dup("");
    pAlarmDetail->backedUpObject.value = CORBA::string_dup("");
    FM::ThresholdInformation* tempThresh = new FM::ThresholdInformation;
    pAlarmDetail->threshInfo = *tempThresh;
    (pAlarmDetail->threshInfo).observedValue = (double) 0;
    ((pAlarmDetail->threshInfo).thresholdLevel).high = (double) 0;
    ((pAlarmDetail->threshInfo).thresholdLevel).low = (double) 0;

    FM::CorrelatedNotifications* tempCorr = new FM::CorrelatedNotifications;
    pAlarmDetail->notifications = *tempCorr;
    FM::RepairActionsList* pRepair = new FM::RepairActionsList;
    pAlarmDetail->proposedRepairActions = *pRepair;
    FM::StateChangeDefList * tempState = new FM::StateChangeDefList;
    pAlarmDetail->stateChangeDefs = *tempState;
    FM::MonitorAttributeList * tempAttr = new FM::MonitorAttributeList;
    pAlarmDetail->monitorAttributes = *tempAttr;
    pAlarmDetail->additionalText = CORBA::string_dup(errorMsg.c_str());
    FM::AdditionalInfoList* tempAdd = new FM::AdditionalInfoList;
    pAlarmDetail->additionalInfos = *tempAdd;

    char strClear[40];
    EMS_FW::TTime::GetCurrentAgentTime(strClear);
    pAlarmDetail->clearSource = CORBA::string_dup("");

    char strAck[40];
    EMS_FW::TTime::GetCurrentAgentTime(strAck);
    pAlarmDetail->acknowledgedSource = CORBA::string_dup("");

    if (eventType == "EMSALARM")
        pAlarmDetail->isStandingAlarm = (CORBA::Boolean) true;
    else
        pAlarmDetail->isStandingAlarm = (CORBA::Boolean) false;
    pAlarmDetail->serviceAffectedAlarm = (CORBA::UShort) 0;

    FMI::SwAlarmReceiver_var pFml = FMI::SwAlarmReceiver::_nil();
    try
    {
        CORBA::Object_var obj;
        string strFmlName = "Fml_";
        strFmlName += pAlarmDetail->neId;

        obj = ComManager::instance()->resolveObjectName(strFmlName.c_str());

        if (CORBA::is_nil(obj))
        {
            ErrorLogger::logError(getServerName().c_str(), className, methodName, __LINE__, MAJOR,
                                  "Null CORBA reference from resolveObjectName(), returning...");

            try
            {
                delete tempAdd;
                delete tempAttr;
                delete tempState;
                delete pRepair;
                delete tempCorr;
                delete tempThresh;
                delete pSpecific;
                delete pAlarmDetail;
                pAlarmDetail = NULL;
            }
            catch (...)
            {
                ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Error deleting memory" );
            }

            return ;
        }

        // set time-out
        string generateClearSWAlarmTimeoutString = Server_i::instance()->getDefaultConfigValue ("GenerateClearSWAlarmTimeout", "30");
        int generateClearSWAlarmTimeoutInSeconds = atoi (generateClearSWAlarmTimeoutString.c_str());
        if (generateClearSWAlarmTimeoutInSeconds)
            obj = ComManager::instance()->SetTimeOut (obj, generateClearSWAlarmTimeoutInSeconds * 1000);

        pFml = FMI::SwAlarmReceiver::_narrow(obj);

        if (CORBA::is_nil(pFml))
        {
            ErrorLogger::logVarError(getServerName().c_str(), className, methodName, __LINE__, MAJOR,
                                     "ERROR: NULL Corba reference to Server [%s] after _narrow(), returning...",
                                     strFmlName.c_str());
            try
            {
                delete tempAdd;
                delete tempAttr;
                delete tempState;
                delete pRepair;
                delete tempCorr;
                delete tempThresh;
                delete pSpecific;
                delete pAlarmDetail;
                pAlarmDetail = NULL;
            }
            catch (...)
            {
                ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Error deleting memory" );
            }
            return ;
        }

        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, TRACE3,
                                 "Attempting to generate software alarm for moc [%s], moi [%s], alarmId [%ld] with a timeout of [%d] seconds",
                                 (const char*) pAlarmDetail->managedObjectClass,
                                 (const char*) pAlarmDetail->managedObjectInstance,
                                 pAlarmDetail->alarmID,
                                 generateClearSWAlarmTimeoutInSeconds);

        pFml->logAlarm(*pAlarmDetail);

        ErrorLogger::logError(getServerName().c_str(), className, methodName, TRACE4,
                              "Alarm sent to FM logger.");
    }
    catch (CORBA::TIMEOUT &ex)
    {
        string failureReason = ex._info().c_str();
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR, "Could not generate software alarm; %s", failureReason.c_str());
    }
    catch (CORBA::COMM_FAILURE)
    {
        string failureReason = "CORBA communication failure";
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR, "Could not generate software alarm; %s", failureReason.c_str());
    }
    catch (CORBA::Exception &ex)
    {
        string failureReason = ex._info().c_str();
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR, "Could not generate software alarm; %s", failureReason.c_str());
    }
    catch (...)
    {
        ErrorLogger::logError(getServerName().c_str(), className, methodName, __LINE__, MAJOR, "Could not generate software alarm due to an unknown error");
    }

    try
    {
        delete tempAdd;
        delete tempAttr;
        delete tempState;
        delete pRepair;
        delete tempCorr;
        delete tempThresh;
        delete pSpecific;
        delete pAlarmDetail;
        pAlarmDetail = NULL;
    }
    catch (...)
    {
        ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Error deleting memory" );
    }

    ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE4, "Returning..." );
    return ;
}

void Server_i::generateSwEvent (const string &softwareEventType, const string &source, const string &description)
{
    generateSwAutonomous (softwareEventType, source, description, "EMSEVENT");
}

void Server_i::generateSwAlarm(const string &softwareAlarmType, const string &source, const string &description)
{
    generateSwAutonomous (softwareAlarmType, source, description, "EMSALARM");
}

/*********************************************************************************
 * This method is responsible for
 * - Generating clear alarm for the specifoed ne and sending it to the Fml
 *   associated with software ne.
 ********************************************************************************/
void Server_i::clearSwAlarm(string softwareAlarmType, string source, string description, string location, string eventType)
{
    const char *methodName = "clearSwAlarm";

    ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Clearing software alarm ..." );

    string probableCause = softwareAlarmType;
    string errorMsg = description;
    eventType = "EMSALARM";

    FM::AlarmDetail* pAlarmDetail = new FM::AlarmDetail();

    pAlarmDetail->notificationID = (CORBA::ULong) 0;

    pAlarmDetail->neId = CORBA::string_dup("SoftwareAlarmNe");
    pAlarmDetail->src = CORBA::string_dup(source.c_str());

    string fullMoi = "0";
    string moc = "0";

    pAlarmDetail->managedObjectClass = CORBA::string_dup(moc.c_str());
    pAlarmDetail->managedObjectInstance = CORBA::string_dup(fullMoi.c_str());
    pAlarmDetail->state = FM::AlarmState(2);
    pAlarmDetail->perceivedSeverity = FM::Severity(5); // cleared
    pAlarmDetail->typeOfEvent = CORBA::string_dup(eventType.c_str());


    osi_time_and_date timeDate;
    char str[40];
    timeDate = osi_time_and_date::now();
    osi_date Date = timeDate.utc_date();
    osi_time Time = timeDate.utc_time();
    EMS_FW::TTime::GetTimeInAgentFormat(Date, Time, str);
    pAlarmDetail->eventTime = CORBA::string_dup(str);

    pAlarmDetail->probableCause = CORBA::string_dup(probableCause.c_str());

    pAlarmDetail->indication = FM::TrendIndication(1);
    ;
    FM::SpecificProblemList* pSpecific = new FM::SpecificProblemList;
    pAlarmDetail->specificProblems = *pSpecific ;
    pAlarmDetail->backedUpStatus = 1;
    pAlarmDetail->backedUpObject.attributeId = CORBA::string_dup("");
    pAlarmDetail->backedUpObject.value = CORBA::string_dup("");
    FM::ThresholdInformation* tempThresh = new FM::ThresholdInformation;
    pAlarmDetail->threshInfo = *tempThresh;
    (pAlarmDetail->threshInfo).observedValue = (double) 0;
    ((pAlarmDetail->threshInfo).thresholdLevel).high = (double) 0;
    ((pAlarmDetail->threshInfo).thresholdLevel).low = (double) 0;

    FM::CorrelatedNotifications* tempCorr = new FM::CorrelatedNotifications;
    pAlarmDetail->notifications = *tempCorr;
    FM::RepairActionsList* pRepair = new FM::RepairActionsList;
    pAlarmDetail->proposedRepairActions = *pRepair;
    FM::StateChangeDefList * tempState = new FM::StateChangeDefList;
    pAlarmDetail->stateChangeDefs = *tempState;
    FM::MonitorAttributeList * tempAttr = new FM::MonitorAttributeList;
    pAlarmDetail->monitorAttributes = *tempAttr;
    pAlarmDetail->additionalText = CORBA::string_dup(errorMsg.c_str());
    FM::AdditionalInfoList* tempAdd = new FM::AdditionalInfoList;
    pAlarmDetail->additionalInfos = *tempAdd;

    char strClear[40];
    EMS_FW::TTime::GetCurrentAgentTime(strClear);
    pAlarmDetail->clearTime = CORBA::string_dup(strClear);
    pAlarmDetail->clearSource = CORBA::string_dup("");

    char strAck[40];
    EMS_FW::TTime::GetCurrentAgentTime(strAck);
    pAlarmDetail->acknowledgedSource = CORBA::string_dup("");

    pAlarmDetail->isStandingAlarm = (CORBA::UShort) 1;
    pAlarmDetail->serviceAffectedAlarm = (CORBA::UShort) 0;

    FMI::SwAlarmReceiver_var pFml = FMI::SwAlarmReceiver::_nil();
    try
    {
        CORBA::Object_var obj;
        string strFmlName = "Fml_";
        strFmlName += pAlarmDetail->neId;

        obj = ComManager::instance()->resolveObjectName(strFmlName.c_str());

        if (CORBA::is_nil(obj))
        {
            ErrorLogger::logError(getServerName().c_str(), className, methodName, __LINE__, MAJOR,
                                  "Null CORBA reference from resolveObjectName(), returning...");

            try
            {
                delete tempAdd;
                delete tempAttr;
                delete tempState;
                delete pRepair;
                delete tempCorr;
                delete tempThresh;
                delete pSpecific;
                delete pAlarmDetail;
                pAlarmDetail = NULL;
            }
            catch (...)
            {
                ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Error deleting memory" );
            }
            return ;
        }

        string generateClearSWAlarmTimeoutString = Server_i::instance()->getDefaultConfigValue ("GenerateClearSWAlarmTimeout", "30");
        int generateClearSWAlarmTimeoutInSeconds = atoi (generateClearSWAlarmTimeoutString.c_str());
        if (generateClearSWAlarmTimeoutInSeconds)
            obj = ComManager::instance()->SetTimeOut (obj, generateClearSWAlarmTimeoutInSeconds * 1000);

        pFml = FMI::SwAlarmReceiver::_narrow(obj);

        if (CORBA::is_nil(pFml))
        {
            ErrorLogger::logVarError(getServerName().c_str(), className, methodName, __LINE__, MAJOR,
                                     "ERROR: NULL Corba reference to Server [%s] after _narrow(), returning...",
                                     strFmlName.c_str());

            try
            {
                delete tempAdd;
                delete tempAttr;
                delete tempState;
                delete pRepair;
                delete tempCorr;
                delete tempThresh;
                delete pSpecific;
                delete pAlarmDetail;
                pAlarmDetail = NULL;
            }
            catch (...)
            {
                ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Error deleting memory" );
            }
            return ;
        }

        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, TRACE3,
                                 "Attempting to clear software alarm for moc [%s], moi [%s], alarmId [%ld] with a timeout of [%d] seconds",
                                 (const char*) pAlarmDetail->managedObjectClass,
                                 (const char*) pAlarmDetail->managedObjectInstance,
                                 pAlarmDetail->alarmID,
                                 generateClearSWAlarmTimeoutInSeconds);

        ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Clearing software alarm ..." );
        pFml->logAlarm(*pAlarmDetail);

        ErrorLogger::logError(getServerName().c_str(), className, methodName, TRACE4,
                              "Clear alarm sent to FM logger.");
    }
    catch (CORBA::TIMEOUT &ex)
    {
        string failureReason = ex._info().c_str();
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR, "Could not clear software alarm; %s", failureReason.c_str());
    }
    catch (CORBA::COMM_FAILURE)
    {
        string failureReason = "CORBA communication failure";
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR, "Could not clear software alarm; %s", failureReason.c_str());
    }
    catch (CORBA::Exception &ex)
    {
        string failureReason = ex._info().c_str();
        ErrorLogger::logVarError(getServerName().c_str(), className, methodName, MAJOR, "Could not clear software alarm; %s", failureReason.c_str());
    }
    catch (...)
    {
        ErrorLogger::logError(getServerName().c_str(), className, methodName, __LINE__, MAJOR, "Could not clear software alarm due to an unknown error");
    }

    try
    {
        delete tempAdd;
        delete tempAttr;
        delete tempState;
        delete pRepair;
        delete tempCorr;
        delete tempThresh;
        delete pSpecific;
        delete pAlarmDetail;
        pAlarmDetail = NULL;
    }
    catch (...)
    {
        ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE1, "Error deleting memory" );
    }
    ErrorLogger::logError( getServerName().c_str(), className, methodName, TRACE4, "Returning..." );
    return ;
}

/*****************************************************************
 * This method is responsible for
 * - registers an framework component in a specific group
 *
 *****************************************************************/
void Server_i::registerGroupComponent(EMS_FW::FWComponent* component, const string& groupName, const string& componentName, const string& neId)
{
    _registerMutex.lock();
    if (neId.size() == 0)
    {
        list<EMS_FW::FWComponent*>* groupEntry;
        map<string, list<EMS_FW::FWComponent*>* >::iterator iter = _groups.find(groupName);
        if (iter == _groups.end())
        {
            groupEntry = new list<EMS_FW::FWComponent*>();
            _groups.insert(pair <string, list<EMS_FW::FWComponent*> *> (groupName, groupEntry));
        }
        else
            groupEntry = (*iter).second;
        //check if the component with the same already exists in the list
        list<EMS_FW::FWComponent*>::iterator itera;
        string name = component->getName();
        for (itera = groupEntry->begin();itera != groupEntry->end();itera++)
        {
            if (((*itera)->getName()) == name)
            {
                (*itera) = component;
                break;
            }
        }
        if (itera == groupEntry->end())
            groupEntry->push_back(component);
        registerComponent(component);
    }
    else
    {
        NetworkElement* ne = locateNetworkElement(neId);
        if (ne)
            ne->registerGroupComponent(component, groupName, componentName);
    }
    _registerMutex.unlock();
}

/*****************************************************************
 * This method is responsible for
 * - unregistering the group component
 *
 *****************************************************************/
void Server_i::unregisterGroupComponent(EMS_FW::FWComponent* component, const string& groupName, const string& componentName, const string& neId)
{
    _registerMutex.lock();
    if (neId.size() == 0)
    {
        list<EMS_FW::FWComponent*>* groupEntry;
        map<string, list<EMS_FW::FWComponent*>* >::iterator p = _groups.find(groupName);
        if (p != _groups.end()) //means that it found the group
        {
            groupEntry = (*p).second;
            list<EMS_FW::FWComponent*>::iterator iter;
            for (iter = groupEntry->begin();iter != groupEntry->end();iter++)
            {
                if (((*iter)->getName()) == componentName)
                {
                    groupEntry->erase(iter);
                    break;
                }
            }
            if (groupEntry->empty())
                _groups.erase(p);
        }
        unregisterComponent(component);
    }
    else
    {
        NetworkElement* ne = locateNetworkElement(neId);
        if (ne)
            ne->unregisterGroupComponent(component, groupName, componentName);
    }
    _registerMutex.unlock();
}


/*****************************************************************
 * This method is responsible for
 * - registers an framework component in a specific group
 *
 *****************************************************************/
list<EMS_FW::FWComponent*> Server_i::getComponents(const string& groupName, const string& neId)
{
    if (neId.size() == 0)
    {
        map<string, list<EMS_FW::FWComponent*>* >::iterator iter = _groups.find(groupName);
        if (iter == _groups.end())
        {
            list<EMS_FW::FWComponent*> emptyList;
            return emptyList;
        }
        else
            return *((*iter).second);
    }
    else
    {
        NetworkElement* ne = locateNetworkElement(neId);
        if (ne)
            return (ne->getComponents(groupName, neId));
        else
        {
            list<EMS_FW::FWComponent*> emptyList;
            return emptyList;
        }
    }
}


void Server_i::setServerName(const string& name)
{
    _name = name;
}

void Server_i::notifyFromAM(const SysMon::ServerState& neState)
{
}

void Server_i::notifyFromServer(const SysMon::ServerState& neState)
{
}

void Server_i::serverDown(const char* serverName)
{
}

void Server_i::serverDownTW (const char* serverName)
{
}

void Server_i::stateChange(CORBA::Any event)
{
}

string Server_i::getIpAddress (void)
{
    return (Tutil::getIPAddress());
}

NetworkElement* Server_i::getNetworkElement (const string &neId)
{
    return locateNetworkElement (neId);
}

void Server_i::addProfileEntries (const list<SWAlarmProfileEntry> &entries)
{
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());
    try
    {
        for (list<SWAlarmProfileEntry>::const_iterator iter = entries.begin();iter != entries.end();iter++)
        {
            string sql = "insert into FM_ALARM_PROFILE (FM_ALARM_PROFILE_ENTITY,FM_ALARM_PROFILE_CONDTYPE,FM_ALARM_PROFILE_SEVERITY) ";
            sql += " values (";
            sql += " '" + iter->entityName + "'";
            sql += ",'" + iter->condType + "'";
            sql += "," + Tutil::numberToString (iter->severity);
            sql += ")";
            dbMgr->setSQL (sql);
            dbMgr->executeUpdate();
        }
        dbMgr->commitTransaction();
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "addProfileEntries", __LINE__, MAJOR,
                                 "Error adding profile entries; %s", ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "addProfileEntries", __LINE__, MAJOR,
                                 "Unknown error adding profile entries");
    }
}


void Server_i::removeProfileEntries (const list<SWAlarmProfileEntry> &entries)
{
}

void Server_i::removeProfileEntries (const string &entity)
{
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());
    try
    {
        string sql = "delete from FM_ALARM_PROFILE where FM_ALARM_PROFILE_ENTITY='" + entity + "'";
        dbMgr->setSQL (sql);
        dbMgr->executeUpdate();
        dbMgr->commitTransaction();
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "removeProfileEntries", __LINE__, MAJOR,
                                 "Error removing profile entries for [%s] ; %s", entity.c_str(), ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "removeProfileEntries", __LINE__, MAJOR,
                                 "Unknown error removing profile entries for [%s]", entity.c_str());
    }
}

void Server_i::removeProfileEntries (const list<string> &entities)
{
}

void Server_i::updateProfileEntries (const list<SWAlarmProfileEntry> &entries)
{
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());
    try
    {
        for (list<SWAlarmProfileEntry>::const_iterator iter = entries.begin();iter != entries.end();iter++)
        {
            string sql = "update FM_ALARM_PROFILE ";
            sql += " set FM_ALARM_PROFILE_SEVERITY = " + Tutil::numberToString (iter->severity);
            sql += " where FM_ALARM_PROFILE_ENTITY = '" + iter->entityName + "' ";
            sql += " and FM_ALARM_PROFILE_CONDTYPE = '" + iter->condType + "' ";
            dbMgr->setSQL (sql);
            dbMgr->executeUpdate();
        }
        dbMgr->commitTransaction();
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "updateProfileEntries", __LINE__, MAJOR,
                                 "Error updating profile entries; %s", ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "updateProfileEntries", __LINE__, MAJOR,
                                 "Unknown error updating profile entries");
    }
}

void Server_i::getProfileEntries (const string &entity, list<SWAlarmProfileEntry> &entries)
{
}

void Server_i::getProfileEntries (const list<string>& entities, list<SWAlarmProfileEntry> &entries)
{
}

void Server_i::getProfileEntries (list<SWAlarmProfileEntry> &entries)
{
}

SWAlarmProfile::AlarmSeverity Server_i::getSeverityFromProfilEntry (const string &entity, const string &condType)
{
    // set default severity
    SWAlarmProfile::AlarmSeverity severity = SWAlarmProfile::major;

    try
    {
        //CPE 7345 SRS require the severity of SNMPTRAP to be critical.
        if ( 0 == condType.compare("SNMPTRAP") )
        {
            severity = SWAlarmProfile::critical;
        }
        else
        {
            auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());
            string sql = "select FM_ALARM_PROFILE_SEVERITY from FM_ALARM_PROFILE where ";
            sql += " FM_ALARM_PROFILE_ENTITY='" + entity + "' and FM_ALARM_PROFILE_CONDTYPE='" + condType + "'";
            dbMgr->setSQL (sql);
            auto_ptr<ResultSet> rs(dbMgr->executeQuery());
            if (rs->moveNext())
            {
                severity = (SWAlarmProfile::AlarmSeverity) (rs->getInt(1));
            }
            else
            {
                ErrorLogger::logVarError(getServerName().c_str(), className, "getSeverityFromProfilEntry", __LINE__, MAJOR,
                                         "No profile configured for entity [%s] and condition type [%s]; Will return default severity [%d]", entity.c_str(), condType.c_str(), severity);
            }
        }
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "getSeverityFromProfilEntry", __LINE__, MAJOR,
                                 "Could not get severity for entity [%s] and condition type [%s]; %s; Will return default severity [%d]", entity.c_str(), condType.c_str(), ex.getDescription().c_str(), severity);
    }
    catch (...)
    {
        ErrorLogger::logVarError(getServerName().c_str(), className, "getSeverityFromProfilEntry", __LINE__, MAJOR,
                                 "Unknown error getting severity for entity [%s] and confition type [%s]; Will return default severity [%d]", entity.c_str(), condType.c_str(), severity);
    }
    return severity;
}
//****************************************************************
// Firewall may reset the TCP connection if it is idle for a certain period of time.
// Client will call this interface to keep it alive.
//****************************************************************
void Server_i::keepAlive()
{
}

