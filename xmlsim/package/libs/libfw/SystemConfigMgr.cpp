/***********************************************************************************************
* COPYRIGHT $Date:   Dec 14, 2004  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name:             ResourceMgr.cpp
* Type:             C++ Source
* Originator:       Altaf Aali
* File: $Workfile:  SystemConfigMgr.cpp
* Description:      Class responsible for management and access of System Configuration data
* 
**********************************************************************************************/

#include <list>
#include <string>
#include <map>

using namespace std;

#include "SystemConfigMgr.h"
#include "RDbMgr.h"
#include "OracleDbMgr.h"
#include "BaseException.h"
#include "sysErrLog.h"
#include "tutil.h"


using namespace EMS_FW;
using namespace RDb;
using namespace ELogger;

extern const char *sourceName;
static const char* className = "SystemConfigMgr";

SystemConfigMgr* SystemConfigMgr::_instance = NULL;

SystemConfigMgr::SystemConfigMgr()
{}

SystemConfigMgr::~SystemConfigMgr()
{}

SystemConfigMgr* SystemConfigMgr::instance ()
{
    if (_instance == NULL)
    {
        _instance = new SystemConfigMgr();
    }
    return _instance;
}

void SystemConfigMgr::getSystemConfigVersions (list<string>& versionList, const string &configName, const string &configValue, const string &predicate)
{
    const char *methodName = "getSystemConfigVersions";

    RDbMgr* dbMgr = NULL;
    ResultSet *rs = NULL;


    try
    {
        dbMgr = new OracleDbMgr(sourceName);
        /* string sql = "select DISTINCT CONFIG_TYPE from SYSTEM_CONFIG where CONFIG_NAME = '"+configName+"'";
           string andClause;
           if (predicate == "=")
             andClause = " and CONFIG_VALUE = '"+configValue+"'";
           else
             if (predicate == "substring")
             andClause = " and CONFIG_VALUE like '%"+configValue+"%'";
           sql.append (andClause);
           dbMgr->setSQL(sql);*/
        if (predicate == "substring")
        {
            string sql ("select DISTINCT CONFIG_VERSION from SYSTEM_CONFIG where CONFIG_NAME = :n and CONFIG_VALUE like '%'||:v||'%'");
            dbMgr->setSQL(sql);
            dbMgr->bindStringParam (1, configName);
            dbMgr->bindStringParam (2, configValue);
        }
        else
            if (predicate == "=")
            {
                string sql ("select DISTINCT CONFIG_VERSION from SYSTEM_CONFIG where CONFIG_NAME = :1 and CONFIG_VALUE = :2");
                dbMgr->setSQL (sql);
                dbMgr->bindStringParam (1, configName);
                dbMgr->bindStringParam (2, configValue);
            }
        rs = dbMgr->executeQuery();
        while (rs->moveNext())
        {
            const string &id = rs->getString(1);
            versionList.push_back (id);
        }
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error getting system config ids for configName %s  configValue %s  predicate %s; %s ", configName.c_str(), configValue.c_str(), predicate.c_str(), ex.getDescription().c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;

    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error getting system config ids for configName %s  configValue %s  predicate %s", configName.c_str(), configValue.c_str(), predicate.c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }

    delete rs;
    rs = NULL;
    delete dbMgr;
    dbMgr = NULL;

}

void SystemConfigMgr::getSystemConfigTypes (list<string> &typeList, const string &configVersion, const string &sortByConfigName, bool ascendingSort)
{
    const char *methodName = "getSystemConfigTypes";

    RDbMgr* dbMgr = NULL;
    ResultSet *rs = NULL;

    try
    {
        dbMgr = new OracleDbMgr(sourceName);
        int n_para = 0;
        string sql ("select DISTINCT CONFIG_TYPE, CONFIG_VALUE from SYSTEM_CONFIG ");
        if (!configVersion.empty())
        {
            appendClause (sql, " CONFIG_VERSION = :v ");
            n_para++;
        }
        if (!sortByConfigName.empty())
        {
            appendClause (sql, " CONFIG_NAME = :n ");
            sql.append (" order by CONFIG_VALUE ");
            if (ascendingSort)
                sql.append (" ASC ");
            else
                sql.append (" DESC ");
            n_para++;
        }
        dbMgr->setSQL(sql);
        if(n_para>0)
        {
            n_para = 1;
            if (!configVersion.empty())
            {
                dbMgr->bindStringParam(n_para++, configVersion );
            }
            if (!sortByConfigName.empty())
            {
                dbMgr->bindStringParam(n_para++, sortByConfigName );
            }
        }
        rs = dbMgr->executeQuery();
        while (rs->moveNext())
        {
            const string &type = rs->getString(1);
            const string &configValue = rs->getString(2);
            if (!configValue.empty())
                typeList.push_back (type);
        }
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error getting system config types; %s", ex.getDescription().c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error getting system config types");
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    delete rs;
    rs = NULL;
    delete dbMgr;
    dbMgr = NULL;
}

void SystemConfigMgr::getSystemConfigTypes (list<string> &typeList, const string &configVersion)
{
    const char *methodName = "getSystemConfigTypes";

    RDbMgr* dbMgr = NULL;
    ResultSet *rs = NULL;

    try
    {
        dbMgr = new OracleDbMgr(sourceName);
        string sql ("select DISTINCT CONFIG_TYPE from SYSTEM_CONFIG ");
        int n_para = 0;
        if (!configVersion.empty())
        {
            appendClause (sql, " CONFIG_VERSION = :v ");
            n_para++;
        }
        dbMgr->setSQL(sql);
        if(n_para>0)
        {
            n_para = 1;
            if (!configVersion.empty())
            {
                dbMgr->bindStringParam(n_para++, configVersion );
            }
        }
        rs = dbMgr->executeQuery();
        while (rs->moveNext())
        {
            const string &type = rs->getString(1);
            typeList.push_back (type);
        }
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error getting system config types; %s", ex.getDescription().c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error getting system config types");
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    delete rs;
    rs = NULL;
    delete dbMgr;
    dbMgr = NULL;
}

void SystemConfigMgr::getSystemConfigVersions (list <string> &versionList)
{
    const char *methodName = "getSystemConfigVersions";

    RDbMgr* dbMgr = NULL;
    ResultSet *rs = NULL;

    try
    {
        dbMgr = new OracleDbMgr(sourceName);
        string sql ("select DISTINCT CONFIG_VERSION from SYSTEM_CONFIG");
        dbMgr->setSQL(sql);
        rs = dbMgr->executeQuery();
        while (rs->moveNext())
        {
            const string &id = rs->getString(1);
            versionList.push_back (id);
        }
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error getting system config ids; %s", ex.getDescription().c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error getting system config ids");
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    delete rs;
    rs = NULL;
    delete dbMgr;
    dbMgr = NULL;
}


bool SystemConfigMgr::loadSystemConfig (const string &configType, const string &configVersion, const string &configName, string &configValue)
{
    const char *methodName = "loadSystemConfig";

    if ( (configType.empty()) && (configVersion.empty()) )
        return false;
    if ( configName.empty() )
        return false;

    if (!configType.compare("DB"))
    {
        map<string, string>::iterator iter = _dbConfiguration.find (configName);
        if (iter != _dbConfiguration.end())
            configValue = (*iter).second;
        else
            configValue = "";
        return true;
    }

    RDbMgr* dbMgr = NULL;
    ResultSet *rs = NULL;

    bool result;
    try
    {
        int n_para = 0;
        dbMgr = new OracleDbMgr(sourceName);
        string sql ("select CONFIG_VALUE from SYSTEM_CONFIG ");
        if (!configType.empty())
        {
            appendClause (sql, " CONFIG_TYPE = :t ");
            n_para++;
        }
        if (!configVersion.empty())
        {
            appendClause (sql, " CONFIG_VERSION = :v ");
            n_para++;
        }
        if (!configName.empty())
        {
            appendClause (sql, " CONFIG_NAME = :n ");
            n_para++;
        }
        dbMgr->setSQL(sql);
        if( n_para>0 )
        {
            n_para = 1;
            if (!configType.empty())
            {
                dbMgr->bindStringParam(n_para++, configType );
            }
            if (!configVersion.empty())
            {
                dbMgr->bindStringParam(n_para++, configVersion );
            }
            if (!configName.empty())
            {
                dbMgr->bindStringParam(n_para++, configName );
            }
        }

        rs = dbMgr->executeQuery();
        if (rs->moveNext())
        {
            configValue = rs->getString(1);
            result = true;
        }
        else
            result = false;
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error loading system config id for type %s and name %s; %s", configType.c_str(), configName.c_str(), ex.getDescription().c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error loading system config id for type %s and name %s", configType.c_str(), configName.c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    delete rs;
    rs = NULL;
    delete dbMgr;
    dbMgr = NULL;
    return result;
}

void SystemConfigMgr::appendClause (string &sql, const string &clause)
{
    string sqlCopy (sql);
    Tutil::toLowerCase (sqlCopy);

    if ( sqlCopy.find (" where ") == string::npos)
        sql = sql + " where " + clause;
    else
        sql = sql + " and " + clause;
}

string SystemConfigMgr::getConfig(const string &configType, const string &configVersion, const string &name)
{
    string value;
    bool result = loadSystemConfig (configType, configVersion, name, value);
    if ( (result == false) )
    {
        string message = string ("Config value for ") + name + string (" not found in configuration for ") + configType;
        throw BaseException (message.c_str());
    }
    return value;
}

string SystemConfigMgr::getDefaultConfig (const string &configType, const string &configVersion, const string &name, const string &defaultValue)
{
    string value;

    bool result = loadSystemConfig (configType, configVersion, name, value);
    if ( (result == false) || (value.empty()) )
        value = defaultValue;

    return value;
}

bool SystemConfigMgr::loadSystemConfig (const string &configType, const string &configVersion, map<string, string> &configuration)
{
    const char *methodName = "loadSystemConfig";

    if ( (configType.empty()) && (configVersion.empty()) )
        return false;

    if (!configType.compare("DB"))
    {
        configuration = _dbConfiguration;
        return true;
    }

    RDbMgr* dbMgr = NULL;
    ResultSet *rs = NULL;
    bool result;
    try
    {
        dbMgr = new OracleDbMgr(sourceName);
        /*    string sql = "select CONFIG_NAME,CONFIG_VALUE from SYSTEM_CONFIG where CONFIG_TYPE='"+configType+"'";*/

        int n_para = 0;
        string sql ("select CONFIG_NAME,CONFIG_VALUE from SYSTEM_CONFIG ");
        if (!configType.empty())
        {
            appendClause (sql, " CONFIG_TYPE = :t ");
            n_para++;
        }
        if (!configVersion.empty())
        {
            appendClause (sql, " CONFIG_VERSION = :v ");
            n_para++;
        }

        dbMgr->setSQL(sql);
        if( n_para>0 )
        {
            n_para = 1;
            if (!configType.empty())
            {
                dbMgr->bindStringParam(n_para++, configType );
            }
            if (!configVersion.empty())
            {
                dbMgr->bindStringParam(n_para++, configVersion );
            }
        }
        rs = dbMgr->executeQuery();
        while (rs->moveNext())
        {
            const string configName = rs->getString (1);
            const string configValue = rs->getString (2);
            configuration.insert (map<string, string>::value_type (configName, configValue));
        }
        if (configuration.empty())
            result = false;
        else
            result = true;
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error loading system config for type %s; %s", configType.c_str(), ex.getDescription().c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;

    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error loading system config for type %s", configType.c_str());
        delete rs;
        rs = NULL;
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }

    delete rs;
    rs = NULL;
    delete dbMgr;
    dbMgr = NULL;
    return result;
}


bool SystemConfigMgr::saveSystemConfig (const string &configType, const string &configVersion, map<string, string> &configuration)
{
    const char *methodName = "saveSystemConfig";

    if ( (configuration.empty()) || (configType.empty()) )
        return false;

    if (!configType.compare ("DB"))
    {
        _dbConfiguration = configuration;
        return true;
    }

    RDbMgr* dbMgr = NULL;
    bool result;
    try
    {
        dbMgr = new OracleDbMgr(sourceName);
        map <string, string> configToUpdate, configToInsert;
        for (map<string, string>::iterator iter = configuration.begin();iter != configuration.end();iter++)
        {
            const string &configName = (*iter).first;
            const string &configValue = (*iter).second;
            // check if config name,value pair exists
            string oldConfigValue;
            result = loadSystemConfig (configType, configVersion, configName, oldConfigValue);
            if (result) // means the configuration exists
            {
                // only update if it has changed
                if (oldConfigValue != configValue)
                    configToUpdate.insert (pair<string, string> (configName, configValue));
                else
                    continue;
            }
            else
            {
                configToInsert.insert (pair<string, string> (configName, configValue));
            }
        }

        if (configToInsert.size())
        {
            string sql ("insert into SYSTEM_CONFIG (CONFIG_TYPE, CONFIG_VERSION, CONFIG_NAME,CONFIG_VALUE) values  (:1,:2,:3,:4)");
            dbMgr->setSQL(sql);
            dbMgr->setMaxIterationsForBatch(50);
            dbMgr->setMaxParamSize (1, 12);
            dbMgr->setMaxParamSize (2, 255);
            dbMgr->setMaxParamSize (3, 50);
            dbMgr->setMaxParamSize (4, 1000);
            map<string, string>::iterator iter;
            for (iter = configToInsert.begin();iter != configToInsert.end();iter++)
            {
                if (iter != configToInsert.begin())
                    dbMgr->addIterForBatchExecute();
                dbMgr->bindStringParam (1, configType);
                dbMgr->bindStringParam (2, configVersion);
                dbMgr->bindStringParam (3, (*iter).first);
                dbMgr->bindStringParam (4, (*iter).second);
            }
            dbMgr->executeUpdate();
        }

        if (configToUpdate.size())
        {
            string sql ("update SYSTEM_CONFIG set CONFIG_VALUE=:1 where CONFIG_NAME=:2");
            if (!configType.empty())
                appendClause (sql, " CONFIG_TYPE = '" + configType + "' ");
            if (!configVersion.empty())
                appendClause (sql, " CONFIG_VERSION='" + configVersion + "' ");
            dbMgr->setSQL(sql);
            dbMgr->setMaxIterationsForBatch(50);
            dbMgr->setMaxParamSize (1, 1000);
            dbMgr->setMaxParamSize (2, 50);
            map<string, string>::iterator iter;
            for (iter = configToUpdate.begin();iter != configToUpdate.end();iter++)
            {
                if (iter != configToUpdate.begin())
                    dbMgr->addIterForBatchExecute();
                dbMgr->bindStringParam (1, (*iter).second);
                dbMgr->bindStringParam (2, (*iter).first);
            }
            dbMgr->executeUpdate();
        }

        if ( (configToInsert.size()) || (configToUpdate.size()) )
            dbMgr->commitTransaction();
        result = true;
    }
    catch (BaseException &ex)
    {
        dbMgr->rollbackTransaction();
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Error saving system config for type %s; %s", configType.c_str(), ex.getDescription().c_str());
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    catch (...)
    {
        dbMgr->rollbackTransaction();
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Unknown error saving system config for type %s", configType.c_str());
        delete dbMgr;
        dbMgr = NULL;
        throw;
    }
    delete dbMgr;
    dbMgr = NULL;
    return result;
}


bool SystemConfigMgr::saveSystemConfig (const string &configType, const string &configVersion, const string &configName, const string &configValue)
{
    if ( (configType.empty()) && (configVersion.empty()) )
        return false;
    if ( configName.empty() )
        return false;

    map<string, string> configuration;
    configuration.insert (map<string, string>::value_type (configName, configValue));

    if (!configType.empty())
        return (saveSystemConfig (configType, configVersion, configuration));
    else
    {
        bool returnValue = true;
        list <string> allTypes;
        getSystemConfigTypes (allTypes, configVersion);
        for (list<string>::iterator iter = allTypes.begin();iter != allTypes.end();iter++)
            returnValue = returnValue && (saveSystemConfig ((*iter), configVersion, configuration));
        return returnValue;
    }
}

void SystemConfigMgr::setCachedConfig (const string &type, const string &version, const string &name, const string &value)
{
    _cachedConfig.insert (map<string, string>::value_type (type + version + name, value));
}

void SystemConfigMgr::setCachedSystemConfigTypes (const string &version, const string &name, const list<string>& types)
{
    _cachedConfigTypes.insert (map<string, list<string> >::value_type (version + name, types));
}

string SystemConfigMgr::getCachedDefaultConfig (const string &configType, const string &configVersion, const string &name, const string &defaultValue)
{
    map<string, string>::iterator iter = _cachedConfig.find (configType + configVersion + name);
    if (iter != _cachedConfig.end())
        return (*iter).second;
    else
        return defaultValue;
}

void SystemConfigMgr::getCachedSystemConfigTypes (list<string> &typeList, const string &configVersion, const string &name)
{
    map<string, list<string> >::iterator iter = _cachedConfigTypes.find (configVersion + name);
    if (iter != _cachedConfigTypes.end())
        typeList = (*iter).second;
}


