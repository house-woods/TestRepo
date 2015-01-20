/***********************************************************************************************
* COPYRIGHT $Date:   Dec 14, 2004  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name:             ResourceMgr.h
* Type:             C++ header
* Originator:       Altaf Aali
* File: $Workfile:  SystemConfigMgr.h
* Description:      Class responsible for management and access of System Config data
* 
**********************************************************************************************/

#ifndef _SYSTEMCFGMGR_H_
#define _SYSTEMCFGMGR_H_

#include <list>
#include <map>
#include <string>

using namespace std;

class SystemConfigMgr
{
public:
    static SystemConfigMgr *instance();
    void getSystemConfigVersions (list<string> &versionList);
    void getSystemConfigVersions (list<string> &versionList, const string &configName, const string &configValue, const string &predicate = "=");

    void setCachedConfig (const string &type, const string &version, const string &name, const string &value);
    void setCachedSystemConfigTypes (const string &version, const string &name,const list<string>& types);

    void getCachedSystemConfigTypes (list<string> &typeList, const string &configVersion, const string &name);
    string getCachedDefaultConfig (const string &configType, const string &configVersion, const string &name, const string &defaultValue);

    void getSystemConfigTypes (list<string> &typeList, const string &configVersion = "");
    void getSystemConfigTypes (list<string> &typeList, const string &configVersion, const string &sortByConfigName, bool ascendingSort = true);

    bool loadSystemConfig (const string &configType, const string &configVersion, const string &name, string &value);
    bool loadSystemConfig (const string &configType, const string &configVersion, map<string, string> &configuration);

    string getDefaultConfig (const string &configType, const string &configVersion, const string &name, const string &defaultValue);
    string getConfig (const string &configType, const string &configVersion, const string &name);

    bool saveSystemConfig (const string &configType, const string &configVersion, map<string, string> &configuration);
    bool saveSystemConfig (const string &configType, const string &configVersion, const string &configName, const string &configValue);

    void appendClause (string &sql, const string &clause);

private:

    SystemConfigMgr ();
    ~SystemConfigMgr ();
    map<string, string> _dbConfiguration;
    static SystemConfigMgr * _instance;

    map<string,string> _cachedConfig;
    map<string,list<string> > _cachedConfigTypes;


};

#endif

