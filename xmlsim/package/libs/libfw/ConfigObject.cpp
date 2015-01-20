#include "ConfigObject.h"

/*****************************************************************
 * Default constructor 
 * 
 *****************************************************************/
ConfigObject::ConfigObject()
{}

/*****************************************************************
 * Copy constructor
 * 
 *****************************************************************/
ConfigObject::ConfigObject(const ConfigObject &right)
{}

/*****************************************************************
 * Destructor
 * 
 *****************************************************************/
ConfigObject::~ConfigObject()
{}


/*****************************************************************
 * This method returns value of the specified config parameter
 * from the config map.
 * 
 *****************************************************************/
string ConfigObject::getConfigValue (string name)
{
    //_configuration.find(name); //This call doesn't need to be called twice
    map<string, string>::iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        return "";
    }
    else
        return (*iter).second;
}
/*****************************************************************
 * This method sets value of the specified config parameter
 * in the config map.
 * 
 *****************************************************************/
void ConfigObject::setConfigValue (string name, string value)
{
    map<string, string>::iterator iter = _configuration.find(name);
    if (iter == _configuration.end())
    {
        _configuration.insert(pair<string, string> (name, value));
    }
    else
        (*iter).second = value;
}

/*****************************************************************
 * This method returns configuration from the config map in the specified
 * corba structure.
 * 
 *****************************************************************/
void ConfigObject::getConfiguration (SysMon::Configuration& config)
{
    int length = _configuration.size();
    config.length(length);
    map<string, string>::iterator iter;
    int i = 0;
    for (iter = _configuration.begin(); iter != _configuration.end(); iter++)
    {
        config[i].name = CORBA::string_dup((*iter).first.c_str());
        config[i].value = CORBA::string_dup((*iter).second.c_str());
        i++;
    }
}
/*****************************************************************
 * This method updates configuration in the config map from the specified
 * corba structure.
 * 
 *****************************************************************/
void ConfigObject::setConfiguration (const SysMon::Configuration& config)
{
    _configuration.clear();
    int length = config.length();
    for (int i = 0;i < length;i++)
    {
        string name = config[i].name.in();
        string value = config[i].value.in();
        setConfigValue(name, value);
    }
}
