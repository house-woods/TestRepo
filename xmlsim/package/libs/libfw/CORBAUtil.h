#ifndef _CORBAUTIL_H_
#define _CORBAUTIL_H_

#include <string>
#include <list>
#include <map>

using namespace std;

#include "sysmon.hh"
#include "HostManager.hh"
#include "sm.hh"
#include "ProxyS.hh"

class CORBAUtil
{
public:
    static string getNeVersion (const EMS::Attributes& arguments, int timeOutInSeconds = 0);
    static void getNeVersionEonInfo (const EMS::Attributes& arguments, string& neVersion, string& eonType, int timeOutInSeconds = 0);

    static string getNeCLLI (const EMS::Attributes& arguments);
    static bool getSNMPNEDetails(const EMS::Attributes& arguments, string& snmp_auth_eng_Id, string& snmp_sysdesc, string& snmp_sysname,string & snmp_neversion, int timeOutInSeconds = 0);
    static string getNeSTSVCMode (const EMS::Attributes& arguments, int timeOutInSeconds = 0);
    static void getEONType (const EMS::Attributes& arguments, string& eonType, string& dcnif, string& eonState, int timeOutInSeconds = 0);
    static HM::HostManager_var getHMServerReference (const string &hostname, int timeOutInSeconds = 0);
    static SysMon::EMSServer_var getServerReference (const string &serverName, int timeOutInSeconds = 0);
    static SM::ServerService_var getSMServerReference (const string &interfaceName, int timeOutInSeconds = 0);
    static SM::ClientService_var getSMClientReference (const string &interfaceName, int timeOutInSeconds = 0);
    static SM::AdminService_var getSMAdminReference (const string &interfaceName, int timeOutInSeconds = 0);
    static void getCORBAAttributeValue (const EMS::Attributes& attributes, const string &attributeName, string &attributeValue);
    static bool extractCORBAAttribute (const EMS::Attributes& attributes, int index, string &attributeName, string &attributeValue, string &extractionErrors);
    static void insertAttribute (EMS::Attributes_out &results, const string &name, const string &value);
    static void insertAttribute (EMS::Attributes &results, const string &name, const string &value);
    static void throwNodeException (const string &message);
    static void throwSysmonException (const string &message);
    static void throwNodeNAVException (const string &message);
    static void insertProperty (Node::NodeSummary &summary, const string &name, const string &value);
    static void setCORBAConfig (SysMon::Configuration &corbaConfiguration, const string &name, const string &value);
    static void setCORBAConfiguration (SysMon::Configuration &corbaConfiguration, const map<string, string> &localConfiguration);
    static void launchServerProcess (const string &hostname, const string &commandLine, const string &registeredName, const string &parentServerName, bool waitForIsReadyCallback, int timeOutInSeconds);
    static void terminateServerProcess (const string &hostname, const string &registeredName, int timeOutInSeconds);
    static void startServer (const string &registeredName, const SysMon::Configuration &serverConfig);
    static void stopServer (const string &registeredName);
    static void sendCORBAConfig (const string &registeredName, const string &configName, const string &configValue, int timeOutInSeconds = 300);
    static void updateAttribute (EMS::Attributes& attributes, const string &attributeName, string &attributeValue);
    static void insertParamater (ProxyI::ParameterNames& parameterNames, const string &parameterName);
    static void createCORBAArgList (list<string> &argList, const string &servername, const string &namingServiceHostname, const string &namingServicePort, const string &orbConfFileName, const string &orbDebugLevel, const string &orbAdditionalParamaters, const string &orbLogFile, bool secureConnectionToNaming);


private:

    static ProxyI::Proxy_var getProxyReference (const string &proxyInterfaceName, int timeOutInSeconds = 0);

};
#endif
