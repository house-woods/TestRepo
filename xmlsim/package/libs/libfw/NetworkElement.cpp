
#include "NetworkElement.h"

#include "BaseException.h"

#include "sysErrLog.h"

using namespace std;
using namespace ELogger;
using namespace EMS_FW;

extern const char* sourceName;

/*****************************************************************
 * Default constructor
 *
 *****************************************************************/
NetworkElement::NetworkElement() :
    _state(SysMon::inactive),
    _usersCounter(0),
    _readOnly(false)
{
}

/*****************************************************************
 * Copy constructor
 *
 *****************************************************************/
/*
NetworkElement::NetworkElement(const NetworkElement &right)
{
    _usersCounter = right._usersCounter;
    _readOnly = right._readOnly;
}
*/

/*****************************************************************
 * Destructor
 *
 *****************************************************************/
NetworkElement::~NetworkElement()
{}

/*****************************************************************
 * This method is the default imlementation of initialize operation
 * which writes the log information indicating this operation was called.
 *
 *****************************************************************/
void NetworkElement::initialize ()
{

    //displayConfiguration();
    ErrorLogger::logVarError(sourceName, "NetworkElement", TRACE1,
                             "Network Element initialized :");

}
/*****************************************************************
 * This method is the default imlementation of synchronizeWithPeer operation
 * which writes the log information indicating this operation was called.
 *
 *****************************************************************/
void NetworkElement::synchronizeWithPeer()
{

    ErrorLogger::logVarError(sourceName, "NetworkElement", "synchronizeWithPeer", TRACE1,
                             "Network Element synchronizeWithPeer called");

}

/*****************************************************************
 * This method is the default imlementation of synchronizeWithFcp operation
 * which writes the log information indicating this operation was called.
 *
 *****************************************************************/
void NetworkElement::synchronizeWithFcp ()
{

    ErrorLogger::logVarError(sourceName, "NetworkElement", "synchronizeWithFcp", TRACE1,
                             "Network Element synchronizeWithFcp called");

}
/*****************************************************************
 * This method is the default imlementation of run operation
 * which writes the log information indicating this operation was called.
 *
 *****************************************************************/
void NetworkElement::run ()
{
    synchronizeWithPeer();
    synchronizeWithFcp();
    ErrorLogger::logVarError(sourceName, "NetworkElement", TRACE1,
                             "Network Element running :");
}
/*****************************************************************
 * This method is the default imlementation of finalize operation
 * which writes the log information indicating this operation was called.
 *
 *****************************************************************/
void NetworkElement::finalize ()
{
    ErrorLogger::logVarError(sourceName, "NetworkElement", TRACE1,
                             "Network Element removed :");
}

/*****************************************************************
 * This method is the default imlementation of configurationChanged operation
 * which writes the log information indicating this operation was called.
 *
 *****************************************************************/
void NetworkElement::configurationChanged(const SysMon::NetworkElementConfiguration& config)
{
}

/*****************************************************************
 * This method is responsible for
 * - copying the configuration information from corba structure to config map
 * - calling intialize to start the network element
 * - send state change event to sysmon, if the state is active.
 *
 *****************************************************************/
void NetworkElement::start (const SysMon::Configuration& config)
{
    if (true)
    {
        osi_sem_write_lock lock(_configurationSemaphore);
        _configuration.clear();
    }
    int length = config.length();
    for (int i = 0;i < length;i++)
    {
        string name = (const char*) config[i].name;
        string value = (const char*) config[i].value;
        if (name == "Password" || name == "password")
        {
            string maskPswd (value.length(), '*');
            ErrorLogger::logVarError(sourceName, "NetworkElement", "start", __LINE__, TRACE1, "Config Name=%s Value=%s", name.c_str(), maskPswd.c_str());
        }
        else
        {
            ErrorLogger::logVarError(sourceName, "NetworkElement", "start", __LINE__, TRACE1, "Config Name=%s Value=%s", name.c_str(), value.c_str());
        }
        setConfigValue(name, value);
    }
    _state = SysMon::active;
    initialize();
}
/*****************************************************************
 * This method is responsible for
 * - calling finalize to stop the network element
 * - clear the coniguration map.
 *
 *****************************************************************/
void NetworkElement::stop ()
{
    finalize();
    osi_sem_write_lock lock(_configurationSemaphore);
    _configuration.clear();
}

/*****************************************************************
 * This method is responsible for
 * - getting the value of the specified config parameter.
 *
 *****************************************************************/
string NetworkElement::getConfigValue (const string& name) const
{
    osi_sem_read_lock lock (_configurationSemaphore)
    ;
    map<string, string>::const_iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        string desc = "Unable to get config value->";
        desc += name;
        ErrorLogger::logVarError(sourceName, "NetworkElement", MAJOR,
                                 desc.c_str());
        throw UnresolvedNameEx(name, desc);
    }
    else
        return (*iter).second;
}

/*****************************************************************
 * This method is responsible for
 * - getting the value of the specified config parameter.
 * - returning the default value, if paramater does not exist.
 *
 *****************************************************************/
string NetworkElement::getDefaultConfigValue (const string& name, const string& defaultValue)
{
    osi_sem_read_lock lock (_configurationSemaphore)
    ;
    map<string, string>::const_iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
        return defaultValue;
    else
        return (*iter).second;
}

/*****************************************************************
 * This method is responsible for
 * - setting the value of the specified config parameter.
 *
 *****************************************************************/
void NetworkElement::setConfiguration(const SysMon::NetworkElementConfiguration& config)
{
    ErrorLogger::logError(sourceName, "NetworkElement", "setConfiguration", TRACE1, "Begin");

    int length = config.length();
    for (int i = 0;i < length;i++)
    {
        string name = (const char*) config[i].name;
        string value = (const char*) config[i].value;
        setConfigValue(name, value);
    }

    configurationChanged(config);

    ErrorLogger::logError(sourceName, "NetworkElement", "setConfiguration", TRACE1, "End");
}

/*****************************************************************
 * This method is responsible for
 * - getting the component for the specified name.
 *
 *****************************************************************/
EMS_FW::FWComponent* NetworkElement::getComponent(const string& name)
{
    cmpMap::iterator iter = _components.find(name);
    if (iter == _components.end())
    {
        return NULL; // could not found a resource throw exception?
    }
    else
        return (*iter).second;
}

/*****************************************************************
 * This method is responsible for
 * - registering the component.
 *
 *****************************************************************/
void NetworkElement::registerComponent(EMS_FW::FWComponent* component)
{
    _components[component->getName()] = component;
}

/*****************************************************************
 * This method is responsible for
 * - unregistering the component.
 *
 *****************************************************************/
void NetworkElement::unregisterComponent(EMS_FW::FWComponent* component)
{
    _components.erase(component->getName());
}

/*****************************************************************
 * This method is responsible for
 * - checking if all the specified configuration parameters has
 *   values in the configuration map.
 *
 *****************************************************************/
bool NetworkElement::checkMap(const list<string>& mapList, string& missingStrings)
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
        catch (UnresolvedNameEx& name)
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
 * - printing the names of all components
 *
 *****************************************************************/
void NetworkElement::printRegisteredResources()
{
    cmpMap::iterator p;
    for (p = _components.begin(); p != _components.end(); p++)
    {
        ErrorLogger::logError(sourceName, "NetworkElement", TRACE1, (*p).first.c_str());
    }

}
/*****************************************************************
 * This method is responsible for
 * - printing the contents of configuration map
 *
 *****************************************************************/
void NetworkElement::displayConfiguration()
{
    osi_sem_read_lock lock (_configurationSemaphore)
    ;
    map<string, string>::iterator iter;

    for (iter = _configuration.begin(); iter != _configuration.end(); iter++)
    {
        string msg = "" + (*iter).first + "=" + (*iter).second;
        ErrorLogger::logError(sourceName, "NetworkElement", TRACE1, msg.c_str());
    }

}
/*****************************************************************
 * This method is responsible for
 * - setting the value of the specified config parameter.
 *
 *****************************************************************/
void NetworkElement::setConfigValue(const string& name, const string& value)
{
    osi_sem_write_lock lock (_configurationSemaphore)
    ;
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
 * - setting the state of the network element.
 *
 *****************************************************************/
void NetworkElement::setState(SysMon::State state)
{
    _state = state;
}

/*****************************************************************
 * This method is responsible for
 * - registers an framework component in a specific group
 *
 *****************************************************************/
void NetworkElement::registerGroupComponent(EMS_FW::FWComponent* component, const string& groupName, const string& componentName)
{
    list<EMS_FW::FWComponent*>* groupEntry;
    grpMap::iterator iter = _groups.find(groupName);
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

/*****************************************************************
 * This method is responsible for
 * - unregistering the component.
 *
 *****************************************************************/
void NetworkElement::unregisterGroupComponent(EMS_FW::FWComponent* component, const string& groupName, const string& componentName)
{
    list<EMS_FW::FWComponent*>* groupEntry;
    grpMap::iterator p = _groups.find(groupName);
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


/*****************************************************************
 * This method is responsible for
 * - gets a list of components in a specified group.
 *
 *****************************************************************/
list<EMS_FW::FWComponent* > NetworkElement::getComponents(const string& groupName, const string& neId)
{
    grpMap::iterator iter = _groups.find(groupName);
    if (iter == _groups.end())
    {
        list<EMS_FW::FWComponent*> emptyList;
        return emptyList;
    }
    else
        return *((*iter).second);
}


void NetworkElement::accessNe()
{
    GuardLock guard(_guardMutex);
    if (_state != SysMon::active)
        throw BaseException("Network Element is not accessible.",
                            "NetworkElement", "accessNe");
    _usersCounter++;
}


void NetworkElement::releaseNe()
{
    _usersCounter--;
}

void NetworkElement::_disableNe()
{
    GuardLock guard(_guardMutex);

    setState(SysMon::stopping);
}

void NetworkElement::disableNe()
{
    _disableNe();
}

void NetworkElement::enableNe()
{
    ErrorLogger::logError(sourceName, "NetworkElement",
                          "enableNe", __LINE__, TRACE1, "Method started.");

    GuardLock guard(_guardMutex);
    setState(SysMon::active);

    ErrorLogger::logError(sourceName, "NetworkElement",
                          "enableNe", __LINE__, TRACE1, "Method finished.");
}

void NetworkElement::setAutodiscoverMode(const string& mode)
{
    _autodiscoverMode = mode;

}
