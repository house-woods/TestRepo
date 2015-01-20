#ifndef __Server_i_H__INCLUDED__
#define __Server_i_H__INCLUDED__

#include <string>
#include <map>
#include <list>

#include "tao/PortableServer/PortableServer.h"

#include "SWAlarmProfile.hh"
#include "sysmon.hh"

#include "EMSDefines.h"
#include "sysErrLog.h"

#define CommandGenerateError "generateError"
#define CommandNePeerInitialize "NePeerInitialize"
#define CommandNeFcpInitialize "NeFcpInitialize"
#define CommandServerPeerInitialize "ServerPeerInitialize"
#define CommandServerFcpInitialize "ServerFcpInitialize"
#define CommandSleepServer "sleepServer"

struct SWAlarmProfileEntry
{
    SWAlarmProfileEntry(const std::string& initEntityname, const std::string& initCondtype, const SWAlarmProfile::AlarmSeverity& initSeverity)
    {
        entityName = initEntityname;
        condType = initCondtype;
        severity = initSeverity;
    }
    std::string entityName, condType;
    SWAlarmProfile::AlarmSeverity severity;
};

void * tempTaoCallHandler(void*);

namespace Client_Session
{
    class ClientSession;
};

namespace ProxyI
{
    class Proxy_i;
};

namespace EMS_FW
{
class FWComponent;
class NetworkElement;

class Server_i
{
public:

    friend class BaseCmdInfo;
    friend class CmAutodiscoveryCmd;
    friend class EmsGUIHandeler;
    friend class EmsObject;
    friend class EventManager;
    friend class AlarmLogger;
    friend class AlarmSeverityReceiver;
    friend class Client_Session::ClientSession;
    friend class ClientSession;
    friend class TL1FmEventReceiver;
    friend class ProxyI::Proxy_i;
    friend class TL1ProxyLocRecoverHandler;
    friend class NetworkElementAccess;
    friend class ProtocolProductDecisionMaker;
    friend class tutil;
    friend class TL1AutodiscoveryCmd;

    // Constructor.
    Server_i();

    // Destructor
    //
    virtual ~Server_i();

    //
    // This operation is called by the sysmon to start an EMS server.
    SysMon::State
    startServer(const SysMon::ServerConfiguration &configuration);

    //
    // This operation is called by the sysmon to modify the
    // configuration. It calls configuration Changed() method
    // which shiuld be extended by each subclass of the server
    // to implement the new configuration.
    SysMon::State
    setServerConfiguration(const SysMon::ServerConfiguration &configuration) ;

    //
    // This operation is called by the sysmon to retrieve the
    // server configuration..
    SysMon::ServerConfiguration
    *getServerConfiguration();

    //
    // This operation is called by the sysmon to terminate the EMS server.
    virtual SysMon::State
    terminateServer();

    //
    // This operation is called by the sysmon to indicate to
    // the  EMS server to start processing events.
    virtual void
    run();

    //
    // This operation is called by the sysmon to retrieve the
    // state of the server.
    virtual SysMon::State
    ping();

    //TLIaa54672 Added by jhu
    void setState(SysMon::State state)
    {
        _state = state;
    }

    //
    // This operation is called by the sysmon to add a network
    // element in a EMS server.
    virtual CORBA::Boolean
    addNetworkElement(const SysMon::NetworkElementConfiguration &configuration);

    // added by lzou for TL1
    // set NE readonly true or false, used for LOC or recovery
    virtual CORBA::Boolean
    setNeReadOnly(bool rdOnly, const char* neId);
    //
    // This operation is called by the sysmon to remove a
    // network element from  a EMS server.
    virtual CORBA::Boolean
    removeNetworkElement(const char* resourceId);

    //
    // This operation is called by the sysmon to run a
    // network element from  a EMS server.
    CORBA::Boolean
    runNetworkElement(const char* resourceId);

    //
    // This operation is called by the sysmon to get the NE
    // configuration.
    virtual SysMon::NetworkElementConfiguration
    *getNetworkElementConfiguration(const char* resourceId);

    //
    // This operation is called by sysmon to change the ne
    // configuration of a ne managed by the EMS server.
    virtual CORBA::Boolean
    setNetworkElementConfiguration(const SysMon::NetworkElementConfiguration &configuration);

    //
    // This operation is called by the sysmon to perform a
    // generic action like resynchronize etc on the EMS server.
    virtual CORBA::Boolean
    execCommand(
        const char* operationName,
        const char* neId,
        const SysMon::EMSServer::Parameters &cmdParameters);

    // This operation is called by the sysmon to perform a
    // generic action like resynchronize etc on the EMS server.
    virtual CORBA::Boolean
    execQuery(
        const char* operationName,
        const char* neId,
        const SysMon::EMSServer::Parameters &cmdParameters,
        SysMon::EMSServer::Parameters_out results
    );

    void push(const ::CORBA::Any& event, ::EMS_Event::Event_Type type);

    //
    // -------- Non-CORBA methods --------

    //
    // Operation to activate servant's POA manager and instruct
    // ORB to begin processing incoming requests.
    // virtual void process();

    //
    // This is a factory method which has to be implemented by
    // the subclasses to create an appropriate instance of
    // network element.
    virtual NetworkElement* createNetworkElement();

    // This operation returns the value of the config parameter.
    std::string getConfigValue (const std::string& name, const std::string& neId);
    std::string getConfigValue (const std::string &name);

    // This operation returns the value of the config parameter or default value if absent
    std::string getDefaultConfigValue (const std::string& name, const std::string& neId , const std::string& defaultValue);
    std::string getDefaultConfigValue (const std::string& name, const std::string& defaultValue);

    // This operation returns the integer value of the config parameter.
    int getIntConfigValue (const std::string& name, const std::string& neId = "");


    // This method is used by the server implementations to
    // send the state chage event to the sysmon.

    void sendStateChangeEvent (SysMon::State state, const std::string& neId = "",
                               const std::string& description = "" , ELogger::ErrorCode code = 0,
                               const std::string& neVersion = "", const std::string& nePassword = "");

    void sendStateChangeEvent (const std::string &stateType, const std::map<std::string, std::string> &properties, const std::string &description = "");

    // This method is used by the server to send Error event
    // to the sysmon.
    void sendErrorEvent(const std::string& neId = "", const std::string& description = "" , ELogger::ErrorCode code = 0);

    // This method is used by the server to send log & state
    // change events to the sysmon.
    void logEvent ();

    // This method is called to indicate to the server to
    // prepare all the resources but not to start processing
    // events.
    // This method should be implemented by the subclasses.
    virtual void initialize();

    // This method is called prior to terminating the server
    // process. Server implementation can use this method to
    // release all the resources.
    virtual void finalize();

    // The main method of EMS server process.
    void main ();

    // This method implements start server operation and calls
    // the initialize method.
    void start ();


    // This method has to be implented by subclasses to
    // implement new configuration.
    virtual void configurationChanged ();

    // Locates the NetworkElement instance for the specified NE
    // id.
    // Additional Public Declarations


    // This operation returns NeId in the configuration
    std::string getNetworkElementId (const SysMon::Configuration& config);

    virtual void execute(int argc, char** argv, bool multithreade = false);

    void connectToDatabase (const std::string &serverName);

    void startLogging (const std::string &serverName);

    void initCommManager ();

    //displays the version of the server
    static void DisplayVersionReport();

    // This operation sts the value of the config parameter.
    void setConfigValue (const std::string& name, const std::string& value);

    void setConfigValue (const std::string &name, const std::string &neId, const std::string &value);

    bool neExists (const std::string &neId);

    // This operation sets the integer value of the config parameter.
    void setIntConfigValue (const std::string& name, int value);

    // This operation returns the the singleton instance
    static Server_i* instance ();

    // This operation returns the component for the specified name and neid
    FWComponent* getComponent(const std::string& name, const std::string& neId = "");

    // This operation registers the component
    void registerComponent(FWComponent* component);
    // This operation runregisters the component
    void unregisterComponent(FWComponent* component);

    bool checkMap(const std::list<std::string>& mapList, std::string& missingStrings);

    // This operation returns all the network elements in the server.
    //const list<NetworkElement*> get_networkElements () const;
    // This operation sets all the network elements in the server.
    //void set_networkElements (std::list<NetworkElement*> value);

    // This operation displays server configuration
    void displayConfiguration();

    // This operation sets the server name
    //
    void setServerName(const std::string& name);

    // This operation gets the server name
    //
    const std::string& getServerName();

    // This operation returns true if server is in debug mode.
    bool isDebugMode();

    // this methode is used to generate software alarms
    void generateSwAlarm (const std::string &softwareAlarmType, const std::string &source, const std::string &description);
    void generateSwEvent (const std::string &softwareEventType, const std::string &source, const std::string &description);

    void generateSwAutonomous (const std::string &condtype, const std::string &source, const std::string &description, const std::string &eventType);

    // this methode is used to clear software alarms
    void clearSwAlarm(std::string moc, std::string moi, std::string errorMsg = "", std::string location = "", std::string eventType = "x721communicationsAlarm");

    // Return Servant POA for TIE
    PortableServer::POA_ptr getServantPOA()
    {
        return _servant_poa;
    };


    //added by lzou
    // This method registers an framework component in a specific group
    void registerGroupComponent(FWComponent* component, const std::string& groupName, const std::string& componentName, const std::string& neId = "");
    // This method registers an framework component in a specific group
    void unregisterGroupComponent(FWComponent* component, const std::string& groupName, const std::string& componentName, const std::string& neId = "");
    //    This method gets a std::list of components in a specified group.
    std::list<FWComponent* > getComponents(const std::string& groupName, const std::string& neId = "");

    virtual void notifyFromAM(const SysMon::ServerState& neState);

    virtual void notifyFromServer(const SysMon::ServerState& neState);

    //This Method is called by HM..telling SysMon/SGM that EMS/ThirdParty server is down.
    virtual void serverDown(const char* serverName);

    // this is a blocking version of the above
    virtual void serverDownTW(const char* serverName);

    //This method is called by EMS Server to send state change event to Sysmon/SGM
    virtual void stateChange(CORBA::Any event);

    static bool stopit;

    // temporarily added so that link discovery in OTS can work
    NetworkElement *getNetworkElement (const std::string &neId);

    void addProfileEntries (const std::list<SWAlarmProfileEntry> &entries);

    void removeProfileEntries (const std::list<SWAlarmProfileEntry> &entries);
    void removeProfileEntries (const std::string &entity);
    void removeProfileEntries (const std::list<std::string> &entities);

    void updateProfileEntries (const std::list<SWAlarmProfileEntry> &entries);

    void getProfileEntries (const std::string &entity, std::list<SWAlarmProfileEntry> &entries);
    void getProfileEntries (const std::list<std::string>& entities, std::list<SWAlarmProfileEntry> &entries);
    void getProfileEntries (std::list<SWAlarmProfileEntry> &entries);

    SWAlarmProfile::AlarmSeverity getSeverityFromProfilEntry (const std::string &entity, const std::string &condType);

    virtual bool isRTCorbaEnabled();
    virtual void keepAlive();

protected:

    static Server_i* _instance;         // singleton instance
    bool _primarySysmon;  // true indicates primary sysmon
    PortableServer::POA_var _servant_poa;
    SysMon::EMSServer_var _serverPtr;
    std::map<std::string, NetworkElement*> _networkElements;    // list of network elements
    void constructAndPublishStructuredEvent(CORBA::Any& eventBody, const EMS_FW::SESSIONIDTYPE& lNotifyID);

    virtual std::string getIpAddress (void);

    NetworkElement* locateNetworkElement (const std::string& id);

private:
    // Assignment and copy operators
    // are intentionally not implemented.
    //
    Server_i(const Server_i&);
    Server_i& operator=(const Server_i&);

    std::string _serverName;             // name of the server
    std::map<std::string, std::string> _configuration;      // server configuration std::map
    std::map<std::string, FWComponent*> _components; // server component std::map

    SysMon::State _state;             // server state
    std::string _name;                 // name of the resource

    std::string _serverIORstring;

    osi_read_write_semaphore neListSem;

    // new attributes added by lzou
    std::map<std::string, std::list<FWComponent*>* > _groups;    // Local cache to store group information.
};

};

#endif // __Server_i_H__INCLUDED__
