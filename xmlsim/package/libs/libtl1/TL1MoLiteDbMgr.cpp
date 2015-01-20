/*******************************************************************************
* COPYRIGHT July 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* File: TL1MoLiteDbMgr.cpp
* Description: 
* Name: TL1MoLiteDbMgr
* Type: CPP
* Originator: adesai
* File: TL1MoLiteDbMgr.h
* Version:
* Description: 
*  Inherits from MoLiteDbMgr and take some functions from original
* MoDbMgr 
* 
* Revision History:
*
*  Rev 1.0   init version    lzou taken from AmsMoLiteDbMgr
*
*******************************************************************************/

#include "RDbFactory.h"
#include "RdbUtil.h"
#include <memory>
#include "TL1MoLiteDbMgr.h"
#include "BaseCmdInfo.h"
#include "sysErrLog.h"
#include "EventHandeler.h"
#include "Server_i.h"
#include "TL1ProductDecisionMaker.h"
#include "TL1CmdInfo.h"
#include "BaseException.h"
#include "StringParser.h"

extern const char* sourceName;

using namespace ELogger;
using namespace RDb;
using namespace EMS_FW;
using namespace std;

const string TL1MoLiteDbMgr::className = "TL1MoLiteDbMgr";

const string TL1MoLiteDbMgr::_TL1MoLiteRoot = "MoLiteRoot";


TL1MoLiteDbMgr* TL1MoLiteDbMgr::getInstance(const string& neId, const string& name)
{
    TL1MoLiteDbMgr* mgr;
    if (!name.empty())
        mgr = (TL1MoLiteDbMgr*) Server_i::instance()->getComponent(name, neId);
    else
        mgr = (TL1MoLiteDbMgr*) Server_i::instance()->getComponent(className, neId);

    if (mgr == NULL)
    {
        string desc = "Unable to get TL1MoLiteDbMgr for NeId->";
        desc += neId;
        desc += " Name->";
        desc += name;
        throw BaseException(desc, "TL1MoLiteDbMgr", "getInstance");
    }
    return mgr;
}



TL1MoLiteDbMgr::~TL1MoLiteDbMgr()
{}

TL1MoLiteDbMgr::TL1MoLiteDbMgr(const char* dbName, const string& neId, bool coldStart)
{
    setClassName(className);
    setName(className);
    _neId = neId;
}


void TL1MoLiteDbMgr::registerListener(EMS_FW::EventHandeler* handeler)
{
    ListenerLock lock1(_listenerMutex);
    _listeners.push_back(handeler);
}

void TL1MoLiteDbMgr::unregisterListener(EMS_FW::EventHandeler* handeler)
{
    ListenerLock lock1(_listenerMutex);
    _listeners.remove(handeler);
}

void TL1MoLiteDbMgr::_notifyListeners(const EMS_FW::BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "_notifyListeners", TRACE4,
                             "Method start for moi=%s", info->getMoi().c_str());
    ListenerLock lock1(_listenerMutex);
    list<EventHandeler*>::iterator p;
    for (p = _listeners.begin(); p != _listeners.end(); p++)
    { // notify all handelers that are interested in this event
        (*p)->handleEvent(info);
    }
    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "_notifyListeners", TRACE4,
                             "Method finished for moi=%s", info->getMoi().c_str());
}

void TL1MoLiteDbMgr::_notifyListeners(const EMS_FW::BaseCmdInfoList* infoList)
{
    ErrorLogger::logError(sourceName, "TL1MoLiteDbMgr", "_notifyListeners", TRACE4,
                          "Method start for _notifyListeners list");
    ListenerLock lock1(_listenerMutex);
    list<EventHandeler*>::iterator p;
    for (p = _listeners.begin(); p != _listeners.end(); p++)
    { // notify all handelers that are interested in this event
        (*p)->handleEvent(infoList);
    }
    ErrorLogger::logError(sourceName, "TL1MoLiteDbMgr", "_notifyListeners", TRACE4,
                          "Method finished for _notifyListeners list");
}

void TL1MoLiteDbMgr::abortUpdates(const EMS_FW::BaseCmdInfo* info)
{
    BaseCmdInfo ainfo;
    ainfo.setType(BaseCmdInfo::CM_ABORT);
    ainfo.setMoc(info->getMoc());
    ainfo.setMoi(info->getMoi());
    _notifyListeners(&ainfo);
}

void TL1MoLiteDbMgr::commitUpdates(const EMS_FW::BaseCmdInfo* info)
{
    BaseCmdInfo cinfo;
    cinfo.setType(BaseCmdInfo::CM_COMMIT);
    cinfo.setMoc(info->getMoc());
    cinfo.setMoi(info->getMoi());
    _notifyListeners(&cinfo);
}

void TL1MoLiteDbMgr::notifyListeners(const EMS_FW::BaseCmdInfo* info)
{
    _notifyListeners(info);
}

void TL1MoLiteDbMgr::notifyListeners(const EMS_FW::BaseCmdInfoList* infoList)
{
    _notifyListeners(infoList);
}




void TL1MoLiteDbMgr::initialize(bool coldStart)
{
    ErrorLogger::logError(sourceName, "TL1MoLiteDbMgr", "initialize", TRACE4, "Method start.");
    _autodiscoverMode = false;
    ErrorLogger::logError(sourceName, "TL1MoLiteDbMgr", "initialize", TRACE4, "Method finished OK");
    return ;
}




#define RECURSIVE_LEVEL 30


void TL1MoLiteDbMgr::invalidateDb()
{
    _setInterrupted(true);
}


void TL1MoLiteDbMgr::validateDb()
{
    _setInterrupted(false);
}


bool TL1MoLiteDbMgr::isValid()
{
    return !_interrupted;
}




bool TL1MoLiteDbMgr::getValidState()
{
    return _validState;
}


void TL1MoLiteDbMgr::setValidState(bool state)
{
    _validState = state;
}


void TL1MoLiteDbMgr::setAutodiscoverMode(bool mode)
{
    _autodiscoverMode = mode;
}


bool TL1MoLiteDbMgr::isAutodiscoverMode()
{
    return _autodiscoverMode;
}


void TL1MoLiteDbMgr::_setInterrupted(bool interrupted)
{
    _interrupted = interrupted;
}


//******************************************************************************
// This method tries to see if the Entity being created has any orphaned child
// creates that came in ahead of it.  It looks for orphans that had a specific
// Id for its parent, and also ones that had to leave the parent's type generic.
//
void TL1MoLiteDbMgr::_assignNotAssignedObjects(const EMS_FW::BaseCmdInfo* info)
{
    if (_orphanMap.empty())  // If orphaned object map is empty,
        return ;
    string id = info->getMoi();

    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "_assignNotAssignedObjects", TRACE4,
                             "Method start for moi=%s", id.c_str());

    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();
    string orphanId, table, idColName;

    int noItems = 0;

    // Need to `find' every time because the recursive call can invalidate the iterator.
    maptype::iterator mapIter;
    while ((mapIter = _orphanMap.find(id)) != _orphanMap.end())
    {
        noItems++;
        BaseCmdInfo* baseInfo = (*mapIter).second;
        _orphanMap.erase(mapIter);

        orphanId = baseInfo->getMoi();
        const string& aid = ((TL1CmdInfo*)(baseInfo))->getTL1Aid();
        const string& TL1cmd = ((TL1CmdInfo*)(baseInfo))->getTl1msgString();
        if (pdMaker->getTableandIdColName(aid, TL1cmd, table, idColName))
        {
            if (idColName.find("NE_ID,") == 0)     // If "NE_ID," at start of Id column name string,
            {
                orphanId.insert(0, ",");
                orphanId.insert(0, baseInfo->getNeId());
            }
            if (RdbUtil::checkIfEntityExists(table, idColName, orphanId))
                baseInfo->setType(BaseCmdInfo::CM_AVC_EVENT);
        }
        _notifyListeners(baseInfo);
        _assignNotAssignedObjects(baseInfo);
        delete baseInfo;
    }

    id.replace(5, 2, "00");    // Null the last two type characters to make it semi-generic.

    while ((mapIter = _orphanMap.find(id)) != _orphanMap.end())
    {
        noItems++;
        BaseCmdInfo* baseInfo = (*mapIter).second;
        ((TL1CmdInfo*)baseInfo)->setTL1ParentId(info->getMoi());
        _orphanMap.erase(mapIter);

        orphanId = baseInfo->getMoi();
        const string& aid = ((TL1CmdInfo*)(baseInfo))->getTL1Aid();
        const string& TL1cmd = ((TL1CmdInfo*)(baseInfo))->getTl1msgString();
        if (pdMaker->getTableandIdColName(aid, TL1cmd, table, idColName))
        {
            if (idColName.find("NE_ID,") == 0)     // If "NE_ID," at start of Id column name string,
            {
                orphanId.insert(0, ",");
                orphanId.insert(0, baseInfo->getNeId());
            }
            if (RdbUtil::checkIfEntityExists(table, idColName, orphanId))
                baseInfo->setType(BaseCmdInfo::CM_AVC_EVENT);
        }
        _notifyListeners(baseInfo);
        _assignNotAssignedObjects(baseInfo);
        delete baseInfo;
    }

    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "_assignNotAssignedObjects", TRACE3,
                             "Method finished for moi=%s, found %d children", id.c_str(), noItems);
    return ;
}


//******************************************************************************
bool TL1MoLiteDbMgr::_updateRelations(EMS_FW::BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "_updateRelations", TRACE3,
                             "Method start for moi=%s", info->getMoi().c_str());

    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    string parentMoc, parentMoi;
    const string& moc = info->getMoc();
    const string& aid = ((EMS_FW::TL1CmdInfo*)info)->getTL1Aid();

    //determine function determines parent moi and moc as well as checks if parent exists
    if (!pdMaker->getParentMoiMoc(aid, _neId, moc, parentMoi, parentMoc))
    {
        //another notification for the same enity is expected to be inserted at the end
        ((TL1CmdInfo*)info)->setTL1ParentId(parentMoi);
        EMS_FW::BaseCmdInfo* pNewInfo = info->clone();
        _orphanMap.insert(mapValueType(parentMoi, pNewInfo));

        return false;
    }
    ((TL1CmdInfo*)info)->setTL1ParentId(parentMoi);

    return true;
}

//******************************************************************************
bool TL1MoLiteDbMgr::createEntity(const EMS_FW::BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "createEntity", TRACE3,
                             "Method start for moi = %s", info->getMoi().c_str());

    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();
    string table, idColName;

    string id = info->getMoi();
    const string& aid = ((TL1CmdInfo*)(info))->getTL1Aid();
    const string& TL1cmd = ((TL1CmdInfo*)(info))->getTl1msgString();
    if (!pdMaker->getTableandIdColName(aid, TL1cmd, table, idColName))
        return false;
    if (idColName.find("NE_ID,") == 0)     // If "NE_ID," at start of Id column name string,
    {
        id.insert(0, ",");
        id.insert(0, info->getNeId());
    }
    if (RdbUtil::checkIfEntityExists(table, idColName, id))
    {
        // The object already exists! update it if the object MOC is the same
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createEntity",
                                 TRACE3, "Element, %s (%s), already exists in %s. Change to AVC",
                                 info->getMoi().c_str(), aid.c_str(), table.c_str());
        ((BaseCmdInfo*)info)->setType(BaseCmdInfo::CM_AVC_EVENT);
        _notifyListeners(info);

        ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "createEntity", TRACE3,
                                 "Method finished for existing moi=%s",
                                 info->getMoi().c_str());
        return true;
    }
    else
    {
        // Check if parent exists. If not than store it in the childMap
        if (_updateRelations((EMS_FW::BaseCmdInfo*)info))
        {
            _notifyListeners(info);     // notify listeners for newly created Object
            _assignNotAssignedObjects(info);

            return true;
        }
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createEntity", __LINE__,
                                 TRACE3, "Element, %s (%s), must be an orphan waiting for parent.",
                                 info->getMoi().c_str(), aid.c_str() );
    }

    return false;
}

bool TL1MoLiteDbMgr::createRamanEntity(const EMS_FW::BaseCmdInfo* info)
{
	ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "createRamanEntity", TRACE3,
                             "Method start for moi=%s", info->getMoi().c_str());
							 
	const list<MoAttributePo*>& attrList = info->getAttributes();
	string ip, engineId, aidType, link;

	for (list<MoAttributePo*>::const_iterator p = attrList.begin(); p != attrList.end(); p++)
	{
		string attrName = (*p)->getName();
		const string& attrValue = ((StringASN1*)((*p)->getValue()))->GetValue();
		// TL1Utility::deletePrefix(attrName);    // Tell client developer to stop sending prefix!
		if (attrName == "RNEIP")
		{
			ip = attrValue;
		}
		else if (attrName == "ENGINEID")
		{
			engineId = attrValue;
		}
		else if (attrName == "PRODTYPE")
		{
			aidType = attrValue;
		}
		else if (attrName == "LINK")
		{
			link = attrValue;
		}
	}
	
	string id = info->getMoi();
	
	string type;
	
	if (aidType == "NIR")
	{
		type = "581";
	}
	else if (aidType == "CRA")
	{
		type = "582";
	}
	else if (aidType == "BAMP")
	{
		type = "580";
	}
	else
	{
		return true;
	}
	
	StringParser prsr(link, "-");
	string shelf, slot, port;
	shelf = prsr.getTokenbyIndex(0);
	slot = prsr.getTokenbyIndex(1);
	port = prsr.getTokenbyIndex(2);
	
	if (shelf.length() < 2)
	{
		shelf = "0" + shelf;
	}
	
	if (slot.length() < 2)
	{
		slot = "0" + slot;
	}
	
	if (port.length() < 2)
	{
		port = "0" + port;
	}
	
	id = id + type + shelf + slot + port;
	
	((BaseCmdInfo*)info)->setMoi(id);
	
	string aid = aidType + "-" + link;


    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();
    string table, idColName;

    const string& TL1cmd = ((TL1CmdInfo*)(info))->getTl1msgString();
    if (!pdMaker->getTableandIdColName(aid, TL1cmd, table, idColName))
        return false;
    if (RdbUtil::checkIfEntityExists(table, idColName, id))
    {
        // The object already exists! update it if the object MOC is the same
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createEntity",
                                 TRACE3, "Element, %s (%s), already exists in %s. Change to AVC",
                                 info->getMoi().c_str(), aid.c_str(), table.c_str());
        ((BaseCmdInfo*)info)->setType(BaseCmdInfo::CM_AVC_EVENT);
        _notifyListeners(info);

        ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "createEntity", TRACE3,
                                 "Method finished for existing moi=%s",
                                 info->getMoi().c_str());
        return true;
    }
    else
    {
        // Check if parent exists. If not than store it in the childMap
        // if (_updateRelations((EMS_FW::BaseCmdInfo*)info))
        // {
            // _notifyListeners(info);     // notify listeners for newly created Object
            // _assignNotAssignedObjects(info);

            // return true;
        // }
		
		_notifyListeners(info);
		
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createRamanEntity", __LINE__,
                                 TRACE3, "Element, %s (%s), must be an orphan waiting for parent.",
                                 info->getMoi().c_str(), aid.c_str() );
    }

    return false;
}

//******************************************************************************
bool TL1MoLiteDbMgr::deleteEntity(const EMS_FW::BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "deleteEntity", TRACE3,
                             "Method start for moi=%s", info->getMoi().c_str());

    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();
    string table, idColName;

    string id = info->getMoi();
    const string& aid = ((TL1CmdInfo*)(info))->getTL1Aid();
    const string& TL1cmd = ((TL1CmdInfo*)(info))->getTl1msgString();
    if (!pdMaker->getTableandIdColName(aid, TL1cmd, table, idColName))
        return false;
    if (idColName.find("NE_ID,") == 0)     // If "NE_ID," at start of Id column name string,
    {
        id.insert(0, ",");
        id.insert(0, info->getNeId());
    }
    if (RdbUtil::checkIfEntityExists(table, idColName, id))
    {
        // The object exists! delete it.
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "deleteEntity",
                                 TRACE3, "Element %s exists in %s. Go ahead and delete it.",
                                 id.c_str(), table.c_str());
        // ATTENTION need to be duplicated
        ((BaseCmdInfo*)info)->setType(BaseCmdInfo::CM_DELETE_EVENT);
        _notifyListeners(info);
        ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "deleteEntity", TRACE3,
                                 "Method finished Object exists for moi=%s",
                                 info->getMoi().c_str());
        return true;
    }
    else
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "deleteEntity", __LINE__,
                                 TRACE3, "Element %s does not exist in %s. Ignore Event.",
                                 id.c_str(), table.c_str());
    }
    return false;
}

bool TL1MoLiteDbMgr::deleteRamanEntity(const EMS_FW::BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "deleteRamanEntity", TRACE3,
                             "Method start for moi=%s", info->getMoi().c_str());
							 
	const list<MoAttributePo*>& attrList = info->getAttributes();
	string ip, engineId, aidType, link;

	for (list<MoAttributePo*>::const_iterator p = attrList.begin(); p != attrList.end(); p++)
	{
		string attrName = (*p)->getName();
		const string& attrValue = ((StringASN1*)((*p)->getValue()))->GetValue();
		// TL1Utility::deletePrefix(attrName);    // Tell client developer to stop sending prefix!
		if (attrName == "RNEIP")
		{
			ip = attrValue;
		}
		else if (attrName == "ENGINEID")
		{
			engineId = attrValue;
		}
		else if (attrName == "PRODTYPE")
		{
			aidType = attrValue;
		}
		else if (attrName == "LINK")
		{
			link = attrValue;
		}
	}
	
	string id = info->getMoi();
	
	// string type;
	
	// if (aidType == "NIR")
	// {
		// type = "581";
	// }
	// else if (aidType == "CRA")
	// {
		// type = "582";
	// }
	// else if (aidType == "BAMP")
	// {
		// type = "580";
	// }
	// else
	// {
		// return true;
	// }
	
	// StringParser prsr(link, "-");
	// string shelf, slot, port;
	// shelf = prsr.getTokenbyIndex(0);
	// slot = prsr.getTokenbyIndex(1);
	// port = prsr.getTokenbyIndex(2);
	
	// if (shelf.length() < 2)
	// {
		// shelf = "0" + shelf;
	// }
	
	// if (slot.length() < 2)
	// {
		// slot = "0" + slot;
	// }
	
	// if (port.length() < 2)
	// {
		// port = "0" + port;
	// }
	
	// id = id + type + shelf + slot + port;
	
	// ((BaseCmdInfo*)info)->setMoi(id);
	
	string aid = aidType + "-" + link;
	
	
    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();
    string table, idColName;

    const string& TL1cmd = ((TL1CmdInfo*)(info))->getTl1msgString();
    if (!pdMaker->getTableandIdColName(aid, TL1cmd, table, idColName))
        return false;
		
	auto_ptr<RDbMgr> rdbMgr (RDbFactory::getDbMgr());
	string sql("SELECT RAMAN_ID FROM CM_RAMAN WHERE NE_ID = :1 AND ");
	sql += " RAMAN_AID = :2 AND RAMAN_INTERNAL_IP = :3";
	rdbMgr->setSQL(sql);
	rdbMgr->bindStringParam(1, info->getNeId());
	rdbMgr->bindStringParam(2, aid);
	rdbMgr->bindStringParam(3, ip);
	bool haveEntity;
	string theId;
	auto_ptr<ResultSet> rs ( rdbMgr->executeQuery() );
	if (rs->moveNext())
	{
		theId = rs->getString(1);
		((BaseCmdInfo*)info)->setMoi(theId);
		haveEntity = true;
	}
	else
	{
		haveEntity = false;
	}
	
	
    if (haveEntity)
    {
        // The object exists! delete it.
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "deleteEntity",
                                 TRACE3, "Element %s exists in %s. Go ahead and delete it.",
                                 id.c_str(), table.c_str());
        // ATTENTION need to be duplicated
        ((BaseCmdInfo*)info)->setType(BaseCmdInfo::CM_DELETE_EVENT);
        _notifyListeners(info);
        ErrorLogger::logVarError(sourceName, "TL1MoLiteDbMgr", "deleteEntity", TRACE3,
                                 "Method finished Object exists for moi=%s",
                                 info->getMoi().c_str());
        return true;
    }
    else
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "deleteRamanEntity", __LINE__,
                                 TRACE3, "Element %s does not exist in %s. Ignore Event.",
                                 id.c_str(), table.c_str());
    }
    return false;
}


