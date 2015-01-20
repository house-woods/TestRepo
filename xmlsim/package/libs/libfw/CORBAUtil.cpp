#include <string>

using namespace std;

#include "sysmon.hh"
#include "HostManager.hh"
#include "sm.hh"
#include "BaseException.h"
#include "CORBAUtil.h"
#include "ComManager.h"
#include "tutil.h"
#include "ProxyS.hh"
#include "Proxy.hh"
#include "ConfigNames.h"
#include "EMSDefines.h"
#include "sysErrLog.h"
#include <ospace/helper.h>


using namespace EMS_FW;
using namespace ProxyI;
using namespace ELogger;

extern const char *sourceName;
static const char* className = "CORBAUtil";

HM::HostManager_var CORBAUtil::getHMServerReference (const string &hostname, int timeOutInSeconds)
{
    ComManager* comManager = ComManager::instance();

    string hostmanagerName = "HostManager_" + hostname;
    CORBA::Object_var tempObject = comManager->resolveObjectName(hostmanagerName.c_str());
    if (CORBA::is_nil (tempObject))
    {
        string message = "Null reference returned while trying to resolve the object::" + hostmanagerName;
        throw BaseException (message.c_str());
    }
    if (timeOutInSeconds)
        tempObject = comManager->SetTimeOut (tempObject, timeOutInSeconds * 1000);
    HM::HostManager_var serverReference = HM::HostManager::_narrow(tempObject);
    if (CORBA::is_nil (serverReference))
    {
        string message = "Null reference returned while setting ping time out for the object::" + hostmanagerName;
        throw BaseException (message.c_str());
    }
    return serverReference;
}

SysMon::EMSServer_var CORBAUtil::getServerReference (const string &serverName, int timeOutInSeconds)
{
    ComManager* comManager = ComManager::instance();

    CORBA::Object_var tempObject = comManager->resolveObjectName(serverName.c_str());
    if (CORBA::is_nil (tempObject))
    {
        string message = "Null reference returned while trying to resolve the object::" + serverName;
        throw BaseException (message.c_str());
    }
    if (timeOutInSeconds)
        tempObject = comManager->SetTimeOut (tempObject, timeOutInSeconds * 1000);
    SysMon::EMSServer_var serverReference = SysMon::EMSServer::_narrow(tempObject);
    if (CORBA::is_nil (serverReference))
    {
        string message = "Null reference returned while trying to resolve the object::" + serverName;
        throw BaseException (message.c_str());
    }
    return serverReference;
}

SM::ServerService_var CORBAUtil::getSMServerReference (const string &interfaceName, int timeOutInSeconds)
{
    ComManager* comManager = ComManager::instance();

    CORBA::Object_var tempObject = comManager->resolveObjectName(interfaceName.c_str());
    if (CORBA::is_nil (tempObject))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    if (timeOutInSeconds)
        tempObject = comManager->SetTimeOut (tempObject, timeOutInSeconds * 1000);
    tempObject = comManager->SetSyncScopPolicy (tempObject);
    SM::ServerService_var serverReference = SM::ServerService::_narrow(tempObject);
    if (CORBA::is_nil (serverReference))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    return serverReference;
}

SM::AdminService_var CORBAUtil::getSMAdminReference (const string &interfaceName, int timeOutInSeconds)
{
    ComManager* comManager = ComManager::instance();

    CORBA::Object_var tempObject = comManager->resolveObjectName(interfaceName.c_str());
    if (CORBA::is_nil (tempObject))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    if (timeOutInSeconds)
        tempObject = comManager->SetTimeOut (tempObject, timeOutInSeconds * 1000);
    tempObject = comManager->SetSyncScopPolicy (tempObject);
    SM::AdminService_var adminReference = SM::AdminService::_narrow(tempObject);
    if (CORBA::is_nil (adminReference))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    return adminReference;
}

SM::ClientService_var CORBAUtil::getSMClientReference (const string &interfaceName, int timeOutInSeconds)
{
    ComManager* comManager = ComManager::instance();

    CORBA::Object_var tempObject = comManager->resolveObjectName(interfaceName.c_str());
    if (CORBA::is_nil (tempObject))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    if (timeOutInSeconds)
        tempObject = comManager->SetTimeOut (tempObject, timeOutInSeconds * 1000);
    tempObject = comManager->SetSyncScopPolicy (tempObject);
    SM::ClientService_var serverReference = SM::ClientService::_narrow(tempObject);
    if (CORBA::is_nil (serverReference))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    return serverReference;
}

void CORBAUtil::getCORBAAttributeValue (const EMS::Attributes& attributes, const string &attributeName, string &attributeValue)
{
    string attributeNameInList, extractedAttributeName, extractionErrors;
    for (int index = 0;index < attributes.length();index++)
    {
        attributeNameInList.assign (attributes[index].attName.in());
        if (!attributeNameInList.compare (attributeName))
        {
            extractCORBAAttribute (attributes, index, extractedAttributeName, attributeValue, extractionErrors);
            break;
        }
    }
}

void CORBAUtil::updateAttribute (EMS::Attributes& attributes, const string &attributeName, string &attributeValue)
{
    string attributeNameInList, extractedAttributeName, extractionErrors;
    for (int index = 0;index < attributes.length();index++)
    {
        attributeNameInList.assign (attributes[index].attName.in());
        if (!attributeNameInList.compare (attributeName))
        {
            attributes[index].value <<= attributeValue.c_str();
            break;
        }
    }
}

void CORBAUtil::insertAttribute (EMS::Attributes_out &results, const string &name, const string &value)
{
    int currrentLength = results->length();
    results->length (currrentLength + 1);
    (*results)[currrentLength].attName = CORBA::string_dup(name.c_str());
    (*results)[currrentLength].value <<= value.c_str();
}

void CORBAUtil::insertAttribute (EMS::Attributes &results, const string &name, const string &value)
{
    int currrentLength = results.length();
    results.length (currrentLength + 1);
    (results)[currrentLength].attName = CORBA::string_dup(name.c_str());
    (results)[currrentLength].value <<= value.c_str();
}

bool CORBAUtil::extractCORBAAttribute (const EMS::Attributes& attributes, int index, string &attributeName, string &attributeValue, string &extractionErrors)
{
    attributeName = attributes[index].attName.in();
    if (attributeName.empty())
    {
        string error = string (" Attribute at position ") + Tutil::numberToString(index) + string (" is invalid");
        extractionErrors.append (error);
        return false;
    }
    else
    {
        const char *charArray;
        if (0 != (attributes[index].value >>= charArray))
        {
            attributeValue = string (charArray);
            return true;
        }
        int number;
        if (0 != (attributes[index].value >>= number))
        {
            attributeValue = Tutil::numberToString (number);
            return true;
        }
        SysMon::AdminState state;
        if (0 != (attributes[index].value >>= state))
        {
            if (state == SysMon::locked)
                attributeValue = "locked";
            else
                attributeValue = "unlocked";
            return true;
        }
        string error = string (" Invalid Attribute value for ") + attributeName;
        extractionErrors.append (error);
        return false;
    }
}

void CORBAUtil::insertParamater (ParameterNames& parameterNames, const string &parameterName)
{
    int currrentLength = parameterNames.length();
    parameterNames.length (currrentLength + 1);
    parameterNames[currrentLength] = CORBA::string_dup(parameterName.c_str());
}

void CORBAUtil::throwNodeException(const string &errorMessage)
{
    throw NodeI::NodeControlException(EMS_INTERNAL_ERROR, CORBA::string_dup (errorMessage.c_str()));
}

void CORBAUtil::throwNodeNAVException(const string &errorMessage)
{
    throw NodeI::NodeNavigationException (EMS_INTERNAL_ERROR, CORBA::string_dup (errorMessage.c_str()));
}

void CORBAUtil::throwSysmonException(const string &errorMessage)
{
    throw SysMon::SysMonException(EMS_INTERNAL_ERROR, CORBA::string_dup (errorMessage.c_str()));
}

string CORBAUtil::getNeVersion (const EMS::Attributes& arguments, int timeOutInSeconds)
{
    string tid;
    getCORBAAttributeValue (arguments, ConfigNames::TID, tid);

    ProxyI::Proxy_var defaultProxy = getProxyReference (EMSDEF::DEFAULTPROXYINTERFACENAME, timeOutInSeconds);
    EMS::Attributes* proxyResults;
    ParameterNames parameterNames;
    try
    {
        insertParamater (parameterNames, "NeVersion");
        defaultProxy->retrieveParameters (arguments, proxyResults, parameterNames);
    }
    catch (ProxyException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not get version for NE [%s]: %s", tid.c_str(), ex.errorDescription.in());
        string errorMessage = "Could not get version for NE " + tid + " due to the following errors: " + string (ex.errorDescription.in());
        throw BaseException (errorMessage);
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not get version for NE [%s]; %s", tid.c_str(), ex.getDescription().c_str());
        string errorMessage = "Could not get version for NE " + tid + " due to the following errors: " + string (ex.getDescription());
        throw BaseException (errorMessage);
    }
    catch (CORBA::TIMEOUT &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not version for NE [%s]; %s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA timeout";
        throw BaseException (errorMessage);
    }
    catch (CORBA::COMM_FAILURE)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not get version for NE [%s]; CORBA comm failure", tid.c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA comm failure";
        throw BaseException (errorMessage);
    }
    catch (CORBA::SystemException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not get version for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA system exception";
        throw BaseException (errorMessage);
    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not get version for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA exception";
        throw BaseException (errorMessage);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersion", MINOR, "Could not get version for NE [%s]", tid.c_str());
        string errorMessage = "Could not get version for NE " + tid;
        throw BaseException (errorMessage);
    }
    string neVersion;
    if (proxyResults->length() > 0)
    {
        const char* value;
        (*proxyResults)[0].value >>= value;
        neVersion = value;
    }

    return neVersion;
}


bool CORBAUtil::getSNMPNEDetails(const EMS::Attributes& arguments, string& snmp_auth_eng_Id, string& snmp_sysdesc, string& snmp_sysname, string & snmp_neversion, int timeOutInSeconds)
{
    const char* methodName = "getSNMPNEDetails";
    string tid;
    getCORBAAttributeValue (arguments, ConfigNames::TID, tid);

    ProxyI::Proxy_var defaultProxy = getProxyReference(EMSDEF::DEFAULTPROXYINTERFACENAME, timeOutInSeconds);
    EMS::Attributes* proxyResults;
    try
    {
        defaultProxy->retrieveSNMPNEDetails(arguments, proxyResults);
    }
    catch (ProxyException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not get SNMP info for NE [%s]: %s", tid.c_str(), ex.errorDescription.in());
        string errorMessage = "Could not get SNMP info for NE " + tid + " due to the following errors: " + string (ex.errorDescription.in());
        throw BaseException (errorMessage);
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not get SNMP info for NE [%s]; %s", tid.c_str(), ex.getDescription().c_str());
        string errorMessage = "Could not get SNMP info for NE " + tid + " due to the following errors: " + string (ex.getDescription());
        throw BaseException (errorMessage);
    }
    catch (CORBA::TIMEOUT &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not version for NE [%s]; %s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get SNMP info for NE  " + tid + " due to a CORBA timeout";
        throw BaseException (errorMessage);
    }
    catch (CORBA::COMM_FAILURE)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not get SNMP info for NE [%s]; CORBA comm failure", tid.c_str());
        string errorMessage = "Could not get SNMP info for NE  " + tid + " due to a CORBA comm failure";
        throw BaseException (errorMessage);
    }
    catch (CORBA::SystemException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not get SNMP info for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get SNMP info for NE  " + tid + " due to a CORBA system exception";
        throw BaseException (errorMessage);
    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not get SNMP info for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get SNMP info for NE  " + tid + " due to a CORBA exception";
        throw BaseException (errorMessage);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Could not get SNMP info for NE [%s]", tid.c_str());
        string errorMessage = "Could not get SNMP info for NE " + tid;
        throw BaseException (errorMessage);
    }

    for (int i = 0; i < proxyResults->length(); i++)
    {
        const char* name = (*proxyResults)[i].attName.in();
        const char* value = 0;
        (*proxyResults)[i].value >>= value;
        
        if (0 == strcmp(name, "SNMPengID"))
        {
            snmp_auth_eng_Id = value;
        }
        else if (0 == strcmp(name, "SNMPsysDescr"))
        {
            snmp_sysdesc = value;
        }
        else if (0 == strcmp(name, "SNMPsysName"))
        {
            snmp_sysname = value;
        }
        else if (0 == strcmp(name, "SNMPneversion"))
        {
            snmp_neversion = value;
        }
    }

    return true;
}

string CORBAUtil::getNeCLLI (const EMS::Attributes& arguments)
{
    string tid;
    getCORBAAttributeValue (arguments, ConfigNames::TID, tid);

    ProxyI::Proxy_var defaultProxy = getProxyReference (EMSDEF::DEFAULTPROXYINTERFACENAME);
    EMS::Attributes* proxyResults;
    ParameterNames parameterNames;
    try
    {
        insertParamater (parameterNames, "SITE");
        defaultProxy->retrieveParameters (arguments, proxyResults, parameterNames);
    }
    catch (ProxyException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeCLLI", MINOR, "Could not get CLLI for NE [%s]: [%s}", tid.c_str(), ex.errorDescription.in());
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeCLLI", MINOR, "Could not get CLLI for NE [%s] due to an unknown exception", tid.c_str());
    }

    string neCLLI;
    if (proxyResults->length() > 0)
    {
        const char* value;
        (*proxyResults)[0].value >>= value;
        neCLLI = value;
    }
    return neCLLI;
}

string CORBAUtil::getNeSTSVCMode (const EMS::Attributes& arguments, int timeOutInSeconds)
{
    string tid;
    getCORBAAttributeValue (arguments, ConfigNames::TID, tid);

    ProxyI::Proxy_var defaultProxy = getProxyReference (EMSDEF::DEFAULTPROXYINTERFACENAME);
    EMS::Attributes* proxyResults = NULL;
    ParameterNames parameterNames;
    try
    {
        insertParamater (parameterNames, "STSVCMDE");
        defaultProxy->retrieveParameters (arguments, proxyResults, parameterNames);
    }
    catch (ProxyException &ex)
    {
        throw;
    }
    catch (BaseException &ex)
    {
        throw;
    }
    catch (...)
    {
        string errorMessage = "Could not get version for NE " + tid;
        ErrorLogger::logVarError(sourceName, className, "getNeSTSVCMode", MINOR, errorMessage.c_str());
        throw BaseException (errorMessage);
    }

    // pre FP4.2 NEs double not support this parameter therefore the proxy returns a blank string.
    // All pre FP4.2 are actually in STS mode so therefore to keep things simple let's just set it
    // to STS
    string stsVCMode = "STS";
    if ((proxyResults != NULL) && (proxyResults->length() > 0))
    {
        const char* value;
        (*proxyResults)[0].value >>= value;

        string temp = value;
        if (temp.length() > 0)
            stsVCMode = value;
    }

    return stsVCMode;
}

void CORBAUtil::getEONType (const EMS::Attributes& arguments, string& eonType, string& dcnif, string& eonState, int timeOutInSeconds)
{
    string tid;
    getCORBAAttributeValue (arguments, ConfigNames::TID, tid);

    ProxyI::Proxy_var defaultProxy = getProxyReference (EMSDEF::DEFAULTPROXYINTERFACENAME, timeOutInSeconds);
    EMS::Attributes* proxyResults;
    ParameterNames parameterNames;
    try
    {
        insertParamater (parameterNames, "EONType");
        insertParamater (parameterNames, "DCNIF");
        insertParamater (parameterNames, "EONSTATE");
        defaultProxy->retrieveParameters (arguments, proxyResults, parameterNames);
    }
    catch (ProxyException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get EON type and DCN IP for NE [%s]: %s", tid.c_str(), ex.errorDescription.in());
        string errorMessage = "Could not get EON type and DCN IP for NE " + tid + " due to the following errors: " + string (ex.errorDescription.in());
        throw BaseException (errorMessage);
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get EON type and DCN IP for NE [%s]; %s", tid.c_str(), ex.getDescription().c_str());
        string errorMessage = "Could not get EON type and DCN IP for NE  " + tid + " due to the following errors: " + string (ex.getDescription());
        throw BaseException (errorMessage);
    }
    catch (CORBA::TIMEOUT &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get EON type and DCN IP for NE [%s]; %s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get EON type and DCN IP for NE  " + tid + " due to a CORBA timeout";
        throw BaseException (errorMessage);
    }
    catch (CORBA::COMM_FAILURE)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get EON type and DCN IP for NE [%s]; CORBA comm failure", tid.c_str());
        string errorMessage = "Could not get EON type and DCN IP for NE  " + tid + " due to a CORBA comm failure";
        throw BaseException (errorMessage);
    }
    catch (CORBA::SystemException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get EON type and DCN IP for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get EON type and DCN IP for NE  " + tid + " due to a CORBA system exception";
        throw BaseException (errorMessage);
    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get EON type and DCN IP for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get EON type and DCN IP for NE  " + tid + " due to a CORBA exception";
        throw BaseException (errorMessage);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, "getEONType", MINOR, "Could not get version for NE [%s]", tid.c_str());
        string errorMessage = "Could not get EON type and DCN IP for NE " + tid;
        throw BaseException (errorMessage);
    }


    for (int index = 0;index < proxyResults->length();index++)
    {
        string name = (*proxyResults)[index].attName.in();

        if (name == "EONType")
        {
            const char* eonValue;
            (*proxyResults)[index].value >>= eonValue;
            eonType = eonValue;
        }
        else if (name == "DCNIF")
        {
            const char* dcnifValue;
            (*proxyResults)[index].value >>= dcnifValue;
            dcnif = dcnifValue;
        }
        else if (name == "EONSTATE")
        {
            const char* eonstateValue;
            (*proxyResults)[index].value >>= eonstateValue;
            eonState = eonstateValue;
        }
    }
    if (eonType.compare(EMSDEF::EONTYPEDIROSC) != 0 && eonState.empty())
    {
        //default EONSTATE is PRIVATE for PGNE/PRNE in case of old NE version before FP7.1.
        eonState = "PRIVATE";
    }
}

Proxy_var CORBAUtil::getProxyReference (const string &interfaceName, int timeOutInSeconds)
{
    ComManager* comManager = ComManager::instance();

    CORBA::Object_var tempObject = comManager->resolveObjectName(interfaceName.c_str());
    if (CORBA::is_nil (tempObject))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    if (timeOutInSeconds)
        tempObject = comManager->SetTimeOut (tempObject, timeOutInSeconds * 1000);
    ProxyI::Proxy_var proxyReference = ProxyI::Proxy::_narrow(tempObject);
    if (CORBA::is_nil (proxyReference))
    {
        string message = "Null reference returned while trying to resolve the object::" + interfaceName;
        throw BaseException (message.c_str());
    }
    return proxyReference;
}

void CORBAUtil::insertProperty (Node::NodeSummary &summary, const string &name, const string &value)
{
    int currentlength = summary.properties.length();
    summary.properties.length (currentlength + 1);
    Node::Property property;
    property.name = CORBA::string_dup (name.c_str());
    property.value = CORBA::string_dup (value.c_str());
    summary.properties[currentlength] = property;
    //ErrorLogger::logVarError(sourceName, className, "AddCORBAProperty",  TRACE1,"Adding property: Name [%s]  Value [%s}",name.c_str(),value.c_str());
}

void CORBAUtil::setCORBAConfig(SysMon::Configuration &corbaConfiguration, const string &name, const string &value)
{
    int currentLength = corbaConfiguration.length();
    corbaConfiguration.length (currentLength + 1);
    corbaConfiguration[currentLength].name = CORBA::string_dup(name.c_str());
    corbaConfiguration[currentLength].value = CORBA::string_dup(value.c_str());
}

void CORBAUtil::setCORBAConfiguration(SysMon::Configuration &corbaConfiguration, const map<string, string> &localConfiguration)
{
    int currentLength = corbaConfiguration.length();
    int i = currentLength;
    int increment = localConfiguration.size();
    corbaConfiguration.length (currentLength + increment);
    for (map<string, string>::const_iterator iter = localConfiguration.begin();iter != localConfiguration.end();iter++)
    {
        corbaConfiguration[i].name = CORBA::string_dup((*iter).first.c_str());
        corbaConfiguration[i].value = CORBA::string_dup((*iter).second.c_str());
        i++;
    }
}


void CORBAUtil::launchServerProcess (const string &hostname, const string &commandLine, const string &registeredName, const string &parentServerName, bool waitForIsReadyCallback, int timeOutInSeconds)
{
    HM::HostManager_var hmServerReference = getHMServerReference(hostname, timeOutInSeconds);
    hmServerReference->launch (CORBA::string_dup(commandLine.c_str()), CORBA::string_dup(registeredName.c_str()),
                               CORBA::string_dup(parentServerName.c_str()), waitForIsReadyCallback, true);

}

void CORBAUtil::terminateServerProcess (const string &hostname, const string &registeredName, int timeOutInSeconds)
{
    HM::HostManager_var hmServerReference = getHMServerReference(hostname, timeOutInSeconds);
    hmServerReference->terminate(CORBA::string_dup(registeredName.c_str()));
}

void CORBAUtil::startServer (const string &registeredName, const SysMon::Configuration &serverConfig)
{
    SysMon::EMSServer_var serverReference = getServerReference (registeredName);
    serverReference->startServer (serverConfig);
}

void CORBAUtil::stopServer (const string &registerName)
{
    SysMon::EMSServer_var serverReference = CORBAUtil::getServerReference (registerName);
    serverReference->terminateServer();
}

void CORBAUtil::sendCORBAConfig (const string &registeredName, const string &configName, const string &configValue, int timeOutInSeconds)
{
    SysMon::Configuration config;
    config.length(1);
    config[0].name = CORBA::string_dup( configName.c_str() );
    config[0].value = CORBA::string_dup( configValue.c_str() );
    try
    {
        SysMon::EMSServer_var server = getServerReference (registeredName, timeOutInSeconds);
        ErrorLogger::logVarError(sourceName, className, "sendCORBAConfig", TRACE1, "Attempting to set config [%s]=[%s] for server [%s]", configName.c_str(), configValue.c_str(), registeredName.c_str());
        server->execCommand( CORBA::string_dup( "SetConfig" ), CORBA::string_dup( "" ), config );
    }
    catch (CORBA::TIMEOUT &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "sendCORBAConfig", MINOR, "Could not set config [%s]=[%s] for server [%s] due to a CORBA timeout.", configName.c_str(), configValue.c_str(), registeredName.c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, "sendCORBAConfig", MINOR, "Could not set config [%s]=[%s] for server [%s]", configName.c_str(), configValue.c_str(), registeredName.c_str());
    }
}

void CORBAUtil::createCORBAArgList (list<string> &argList, const string &servername, const string &namingServiceHostname, const string &namingServicePort, const string &orbConfFileName, const string &orbDebugLevel, const string &orbAdditionalParamaters, const string &orbLogFile, bool secureConnectionToNaming)
{
    if ( (!namingServicePort.empty()) && (!namingServicePort.empty()) )
    {
        // if both naming service host name and port number are specified create an ORBInitRef command-line parameter
        // if these are not specific on command line either, then multi-cast discovery is used
        string initrefString;
        if (secureConnectionToNaming)
            initrefString = "NameService=corbaloc:ssliop:" + namingServiceHostname + ":" + Tutil::numberToString (atoi (namingServicePort.c_str()) + 1) + "/NameService";
        else
            initrefString = "NameService=corbaloc:iiop:" + namingServiceHostname + ":" + namingServicePort + "/NameService";
        argList.push_back ("-ORBinitRef");
        argList.push_back (initrefString);
    }

    if (!orbConfFileName.empty())
    {
        argList.push_back ("-ORBSvcConf");
        argList.push_back (orbConfFileName);
    }

    if (!orbDebugLevel.empty())
    {
        if (atoi (orbDebugLevel.c_str()) > 0)
        {
            argList.push_back ("-ORBDebug");
            argList.push_back ("-ORBDebugLevel");
            argList.push_back (orbDebugLevel);
            argList.push_back ("-ORBVerboseLogging");
            argList.push_back ("2");
            if (!orbLogFile.empty())
            {
                argList.push_back ("-ORBLogFile");
                argList.push_back (orbLogFile);
            }
        }
    }

    if (!orbAdditionalParamaters.empty())
    {
        osi_tokenizer argTokenizer (" ", false, "", "\n", "", false);
        vector <string> arguments = argTokenizer.tokenize( orbAdditionalParamaters );
        for (vector< string >::const_iterator i = arguments.begin();i != arguments.end();i++)
        {
            string argument = *i;
            string defaultIPString ("defaultIP");
            string defaultHostnameString ("defaultHostname");
            string defaultInitialPortString ("defaultInitialPort");
            string defaultInitialSSLPortString ("defaultInitialSSLPort");

            int pos = argument.find (defaultIPString);
            if (pos != -1)
                argument.replace (pos, defaultIPString.length(), Tutil::getIPAddress());

            pos = argument.find (defaultHostnameString);
            if (pos != -1)
                argument.replace (pos, defaultHostnameString.length(), Tutil::getHostname());

            string defaultInitPort = Tutil::numberToString (atoi (namingServicePort.c_str()) + 5);
            pos = argument.find (defaultInitialPortString);
            if (pos != -1)
                argument.replace (pos, defaultInitialPortString.length(), defaultInitPort);

            string defaultInitSSLPort;
            if (servername.find ("CmServer") != -1)
                defaultInitSSLPort = Tutil::numberToString (atoi (namingServicePort.c_str()) + 300);
            else
                defaultInitSSLPort = Tutil::numberToString (atoi (namingServicePort.c_str()) + 6);
            pos = argument.find (defaultInitialSSLPortString);
            if (pos != -1)
                argument.replace (pos, defaultInitialSSLPortString.length(), defaultInitSSLPort);

            argList.push_back (argument);
        }
    }
}
void CORBAUtil::getNeVersionEonInfo (const EMS::Attributes& arguments, string& neVersion, string& eonType, int timeOutInSeconds)
{    
    string tid,neType;
    getCORBAAttributeValue (arguments, ConfigNames::TID, tid);
    CORBAUtil::getCORBAAttributeValue(arguments, ConfigNames::NeTypeKey, neType);

    ProxyI::Proxy_var defaultProxy = getProxyReference (EMSDEF::DEFAULTPROXYINTERFACENAME, timeOutInSeconds);
    EMS::Attributes* proxyResults;
    ParameterNames parameterNames;
    try
    {
        insertParamater (parameterNames, "NeVersion");
        if (0 == neType.compare("7100"))
        {
            insertParamater (parameterNames, "EONType");
        }
        defaultProxy->retrieveParameters (arguments, proxyResults, parameterNames);
    }
    catch (ProxyException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not get version for NE [%s]: %s", tid.c_str(), ex.errorDescription.in());
        string errorMessage = "Could not get version for NE " + tid + " due to the following errors: " + string (ex.errorDescription.in());
        throw BaseException (errorMessage);
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not get version for NE [%s]; %s", tid.c_str(), ex.getDescription().c_str());
        string errorMessage = "Could not get version for NE " + tid + " due to the following errors: " + string (ex.getDescription());
        throw BaseException (errorMessage);
    }
    catch (CORBA::TIMEOUT &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not version for NE [%s]; %s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA timeout";
        throw BaseException (errorMessage);
    }
    catch (CORBA::COMM_FAILURE)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not get version for NE [%s]; CORBA comm failure", tid.c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA comm failure";
        throw BaseException (errorMessage);
    }
    catch (CORBA::SystemException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not get version for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA system exception";
        throw BaseException (errorMessage);
    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not get version for NE [%s];%s", tid.c_str(), ex._info().c_str());
        string errorMessage = "Could not get version for NE  " + tid + " due to a CORBA exception";
        throw BaseException (errorMessage);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, "getNeVersionEonInfo", MINOR, "Could not get version for NE [%s]", tid.c_str());
        string errorMessage = "Could not get version for NE " + tid;
        throw BaseException (errorMessage);
    }
    for (int index = 0;index < proxyResults->length();index++)
    {
        string name = (*proxyResults)[index].attName.in();

        if (name == "EONType")
        {
            const char* eonValue;
            (*proxyResults)[index].value >>= eonValue;
            eonType = eonValue;
        }
        else if (name == "NeVersion")
        {
            const char* neVersionValue;
            (*proxyResults)[index].value >>= neVersionValue;
            neVersion = neVersionValue;
        }
    }
}
