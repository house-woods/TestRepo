#ifndef __CONFIG_MANAGER__
#define __CONFIG_MANAGER__

#include <iostream>
#include <fstream>
#include <ospace/thread.h>
#include <ospace/helper.h>
#include <map>
#include <list>
using namespace std;

struct schedule
{
    string neTid;
    string tid;
    string port;
    string header;
    string response;
    string type;
    int initialDelay;
    int schedulePeriod;
    string scheduleTime;
    double mseconds;
    bool launchSchedule;
};

typedef list<string> AttributeValueList;
typedef string AttributeName;
typedef map<AttributeName, AttributeValueList> PropertiesList;

typedef schedule ScheduleEntry;
typedef list<ScheduleEntry *> ScheduleQueue;

class ConfigManager
{
public:
    static ConfigManager* getInstance();
    ~ConfigManager();
    bool finialize();
    bool setCommandDelay(string &command, int &delay);
    bool getCommandDelay(string &command, int &delay);
    bool setAttributes(string key, map<AttributeName, AttributeValueList> &properties, string type);
    bool getAttributes(string key, map<AttributeName, AttributeValueList> &properties, string type);
    bool getAttribute(string key, string name, list<string> &value, string type);
    bool getNEConfigurationFiles(map<string, string> &list);
    bool getTcpConfigurations(map<string, PropertiesList> &neConfig);
    bool getAttribute(string &tid, string type, string attributeName, list<string> &attributeValueList);
    static void toUpper(string &name);
    static bool freeConfigurationMemory(map<string, PropertiesList>& configData);
    bool getAutonomousMessages(string &tid, string port, string &supportedType, ScheduleQueue &scheduleQueue, list<string> &atids);

protected:
    ConfigManager()
    { }
    ;

    bool cleanupConfiguartion(void);

private:
    map<string, PropertiesList> m_neList;
    map<string, PropertiesList> m_serverGroupList;
    map<string, PropertiesList> m_autonomousList;
    map<string, PropertiesList> m_configurationList;
    map<string, PropertiesList> m_neConfigurations;
    map<string, int> m_commandDelays;
    static ConfigManager* m_configManager;

};

#endif
