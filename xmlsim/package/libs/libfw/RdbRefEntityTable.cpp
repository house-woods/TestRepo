
#include "RdbRefEntityTable.h"
#include "ResultSet.h"
#include "RDbFactory.h"
#include "sysErrLog.h"

using namespace ELogger;
using namespace EMS_FW;
using namespace RDb;

extern const char* sourceName;
static const char* objectName = "RdbRefEntityTable";

RdbRefEntityTable* RdbRefEntityTable::_instance = NULL;

RdbRefEntityTable* RdbRefEntityTable::instance()
{
    if (!_instance)
    {
        _instance = new RdbRefEntityTable();
    }

    return _instance;
}

RdbRefEntityTable::RdbRefEntityTable()
{
    initialize();
}

RdbRefEntityTable::~RdbRefEntityTable()
{}

//******************************************************************************
void RdbRefEntityTable::initialize()
{
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());

    string sql = "SELECT ENTITY_AIDTYPE, ENTITY_ID, ENTITY_I_TYPE FROM REF_ENTITY";
    auto_ptr<ResultSet> rs(dbMgr->executeQuery(sql));
    while (rs->moveNext())
    {
        const string& aidType = rs->getString(1);
        const string& id = rs->getString(2);
        const string& type = rs->getString(3);
        ErrorLogger::logVarError(sourceName, objectName, "initialize", TRACE7,
                                 "Record from table %s %s %s.", aidType.c_str(), id.c_str(), type.c_str());
        _insertRecord (aidType, id, type);
    }
}


//******************************************************************************
// The AID type references from the database are stored in two map to make it
// easy to convert in both direction, from AID type to the numeric id type and
// also back to the original AID type.
//
void RdbRefEntityTable::_insertRecord(const string& aidType, const string& id,
                                      const string& entity_i_type)
{
    struct ref_entity* record = new struct ref_entity (id, entity_i_type);
    _tableRecords.insert(pair<string, struct ref_entity*>(aidType, record));

    struct ref_entity* revRecord = new struct ref_entity (aidType, entity_i_type);
    _reverseRecords.insert(pair<string, struct ref_entity*>(id, revRecord));
}


//******************************************************************************
// This method uses the given AID type (part of AID before shelf),
// and returns the numeric identifier for it. It also returns the internal table type.
//
bool RdbRefEntityTable::getEntityIdandType (const string& aidType, string& id,
        string& entity_i_type)
{
    map <string, struct ref_entity*>::const_iterator iter = _tableRecords.find(aidType);
    struct ref_entity* record;
    if (iter != _tableRecords.end())
    {
        record = (*iter).second;
        id = record->entity_id;
        entity_i_type = record->entity_i_type;
        return true;
    }
    else
    {
        id.erase();
        entity_i_type.erase();
        return false;
    }
}

//******************************************************************************
// This method uses the given AID type (part of AID before shelf),
// and returns the numeric identifier for it.
//
bool RdbRefEntityTable::getEntityIdCode (const string& aidType, string& id)
{
    map <string, struct ref_entity*>::const_iterator iter = _tableRecords.find(aidType);
    if (iter != _tableRecords.end())
    {
        struct ref_entity* record = (*iter).second;
        id = record->entity_id;
        return true;
    }
    else
    {
        id.erase();
        return false;
    }
}

//******************************************************************************
// This method uses the given numeric id type, and returns the original AID type
// it represents, like FCMM or OCH-P. It also returns the internal table type.
//
bool RdbRefEntityTable::getAidTypeandType (const string& id, string& aidType,
        string& entity_i_type)
{
    map <string, struct ref_entity*>::const_iterator iter = _reverseRecords.find(id);
    struct ref_entity* record;
    if (iter != _reverseRecords.end())
    {
        record = (*iter).second;
        aidType = record->entity_id;
        entity_i_type = record->entity_i_type;
        return true;
    }
    else
    {
        aidType.erase();
        entity_i_type.erase();
        return false;
    }
}

//******************************************************************************
// This method uses the given numeric id type, and returns the original AID type
// it represents, like FCMM or OCH-P. It also returns the internal table type.
//
string RdbRefEntityTable::getAidType (const string& id)
{
    map <string, struct ref_entity*>::const_iterator iter = _reverseRecords.find(id);
    if (iter != _reverseRecords.end())
    {
        struct ref_entity* record = (*iter).second;
        return record->entity_id;
    }
    else
    {
        return "";
    }
}

//******************************************************************************
//
//
//
bool RdbRefEntityTable::getPossibleAidTypes (list<string>& aidTypes)

{
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());

    string sql = "SELECT distinct TABLE_NAME from user_tab_columns where COLUMN_NAME like '%PARENT%' AND TABLE_NAME like '%FM_VW'";
    auto_ptr<ResultSet> rs(dbMgr->executeQuery(sql));
    list<string> types;
    while (rs->moveNext())
    {
        types.push_back(rs->getString(1));
    }

    map <string, struct ref_entity*>::const_iterator iter;
    list<string>::iterator typeIter;
    struct ref_entity* record;
    string entity_i_type;
    for (iter = _reverseRecords.begin();iter != _reverseRecords.end();iter++)
    {
        record = (*iter).second;
        entity_i_type = record->entity_i_type;
        entity_i_type.append("_FM_VW");
        for (typeIter = types.begin(); typeIter != types.end(); typeIter++)
        {
            if ((*typeIter) == entity_i_type)
            {
                aidTypes.push_back(record->entity_id);
                break;
            }
        }
    }
    return true;
}

//******************************************************************************

string RdbRefEntityTable::getAidTypesForAlarmFilter ()

{
    string returnStr;
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());

    string sql = "SELECT DISTINCT ENTITY_TYPE FROM REF_CONDTYPE ORDER BY ENTITY_TYPE ";
    auto_ptr<ResultSet> rs(dbMgr->executeQuery(sql));
    bool firstTime = true;
    while (rs->moveNext())
    {
        if (firstTime)
        {
            returnStr += rs->getString(1);
            firstTime = false;
        }
        else
        {
            returnStr += "&" + rs->getString(1);
        }
    }
    return returnStr;
}

//******************************************************************************

string RdbRefEntityTable::getConditionTypesForAlarmFilter ()
{
    string returnStr;

    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());

    string sql = "SELECT CONDITION_TYPE, ENTITY_TYPE FROM REF_CONDTYPE ORDER BY CONDITION_TYPE";
    auto_ptr<ResultSet> rs(dbMgr->executeQuery(sql));
    bool firstTime = true;
    while (rs->moveNext())
    {
        if (firstTime)
        {
            // $ is being used to distinguish between condition type and the entity type.
            // it is needed for the client to parsed and determine SDH and SONET based
            // condition types.
            returnStr += rs->getString(1);
            returnStr += "@";
            returnStr += rs->getString(2);
            firstTime = false;
        }
        else
        {
            returnStr += "&" + rs->getString(1);
            returnStr += "@" + rs->getString(2);
        }
    }
    return returnStr;
}

