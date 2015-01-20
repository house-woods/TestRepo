/*******************************************************************************
* COPYRIGHT 2002 - 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: <TL1AutodiscoveryCmd> 
* Type: C++ 
* Originator: adesai
* File: TL1AutodiscoveryCmd.cpp
* Version: 1.5
* Description: 
* Autodiscover command.
* 
* Revision History:
*
*
* File Dependencies: 
* <List of include files this file requires> 
********************************************************************************/

#include "TL1AutodiscoveryCmd.h"
#include <memory>
#include <string>
#include "BaseCmdInfo.h"
#include "sysErrLog.h"
#include "TL1MoLiteDbMgr.h"
#include "CmEventQueue.h"
#include "CmNameResolver.h"

#include "Server_i.h"
#include "sysmon.hh"
#include "ConfigNames.h"
#include "FileReader.h"
#include "TimeCollector.h"
#include "TL1ProductDecisionMaker.h"
#include "ProductDecisionMaker.h"
#include "MoAttributePo.h"
#include "tutil.h"
#include "BaseCmdFactory.h"
#include "TL1CmdInfo.h"
#include "TL1Defines.h"
#include "TL1AutodiscoverAdapter.h"
#include "CmNetworkElement.h"
#include "TL1ConnectionManager.h"

extern const char* sourceName;

using namespace ELogger;
using namespace EMS_FW;

class EMS_FW::CmEventQueue;

#define MAX_OBJECTS_PER_TXN 100
#define DEFAULT_OBJECTS_PER_TXN 20

const string TL1AutodiscoveryCmdName = "TL1AutodiscoveryCmd";


TL1AutodiscoveryCmd::TL1AutodiscoveryCmd(EMS_FW::BaseCmdInfo* info) : CmAutodiscoveryCmd(info)
{}

const string& TL1AutodiscoveryCmd::getClassName() const
{
    return TL1AutodiscoveryCmdName;
}

//******************************************************************************
void TL1AutodiscoveryCmd::_processDiscoveredObjects(list<EMS_FW::BaseCmdInfo*>& objList,
        const string& tl1Cmd, const string& moc)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd",
                                 "_processDiscoveredObjects", __LINE__, TRACE3,
                                 "Method started for %s. MOC=%s", tl1Cmd.c_str(), moc.c_str());
    TL1MoLiteDbMgr* mgr = TL1MoLiteDbMgr::getInstance(info_->getNeId());
    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    const string& eventType = pdMaker->determineEventType(tl1Cmd);
    BaseCmdInfoList baseCmdInfoList;
    baseCmdInfoList.setType(eventType);
    baseCmdInfoList.setBaseCmdInfoList(objList);
    baseCmdInfoList.setMoc(moc);
    mgr->notifyListeners(&baseCmdInfoList);

    list<BaseCmdInfo*>::iterator lIter;
    for (lIter = objList.begin(); lIter != objList.end(); lIter++)
    { // delete the responses
        delete(*lIter);
    }
    objList.clear();

    if (_cancelRequest)
    { // cancel the current autodiscovery operation
        _canceled = true;
        string neName = Server_i::instance()->getConfigValue (EMS_FW::ConfigNames::NeNameKey, info_->getNeId());
        throw BaseException("Cancel request received",
                            getClassName(), "_processDiscoveredObjects");
    }

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_processDiscoveredObjects", __LINE__, TRACE3, "Method finished.");
}


//******************************************************************************
void TL1AutodiscoveryCmd::_discover(bool newDb)
{
    const char* methodName = "_discover";

    CmNetworkElement* ne = (CmNetworkElement*) Server_i::instance()->locateNetworkElement(info_->getNeId());
    /*if (ne)
    {
     ne->setReadOnly (true);
     ne->setReadOnly (false);
    }*/

    /*    if (ErrorLogger::isTraceListOn())
            ErrorLogger::logError(sourceName, getClassName().c_str(), methodName, __LINE__, TRACE1, "Autodiscovery started");*/

    // TimeCollector::start("Autodiscovery for "+info_->getNeId());

    string neName = Server_i::instance()->getDefaultConfigValue (EMS_FW::ConfigNames::NeNameKey, info_->getNeId(), info_->getNeId());

    TimeCollector::start ("Discovery set to in progress for " + neName);
    TL1MoLiteDbMgr* mgr = TL1MoLiteDbMgr::getInstance(info_->getNeId());
    ne->setAutodiscoverMode(EMSDEF::CMAUTODISCOVER_INPROGRESS);
    TimeCollector::end ("Discovery set to in progress for " + neName);

    mgr->invalidateDb();
    // Delete the elements created by an application
    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();
    string neAlwaysAliveMode = Server_i::instance()->getDefaultConfigValue("NEALWAYSALIVEMODE", "NO");
    bool connActive = true;
    if (0 == neAlwaysAliveMode.compare("YES"))
    {
        const string &tid = Server_i::instance()->getDefaultConfigValue (ConfigNames::TID, info_->getNeId(), info_->getNeId());
        TL1Session *tl1Session = TL1ConnectionManager::instance()->getSessionByTID (tid);
        string connectionName;
        TL1Connection *tl1Conn = TL1ConnectionManager::instance()->getConnectionInUse (tl1Session, connectionName);
        if (tl1Conn->isActive())
        {
            connActive = true;
        }
        else
        {
            connActive = false;
        }
    }
    if (connActive)
    {
        pdMaker->resetNEInfoBeforeDiscovery(info_->getNeId());  // Erase all NE data from DB, Close connections etc.
    
        int objsPerTxn = MAX_OBJECTS_PER_TXN;
    
        const string& discoverMethod = Server_i::instance()->getConfigValue(ConfigNames::CMAutodiscoveryMethodKey, info_->getNeId());
        const string& filterFileName = pdMaker->getTL1FilterFileName(info_->getNeId());
        EMS_FW::FileReader fr(filterFileName.c_str());
        if (fr.bad())
        {
            ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_discover",
                                     __LINE__, ELogger::CRITICAL,
                                     "Unable to open filter file [%s]; auto-discovery aborted",
                                     filterFileName.c_str());
            string message = "Unable to open filter file " + filterFileName + "; place filter filer and retry 'Invoke Auto-discovery' option from the NAV tree";
            throw BaseException(message, "TL1AutodiscoveryCmd", "_discover");
        }
    
        // For each line in the Filter file, discover its objects.
        string tmp;
        while (fr.getLine(tmp))
        {
            _discoverOneFilter(tmp, objsPerTxn, discoverMethod);
            pdMaker->onFilterDiscovery (info_->getNeId(), tmp);
        }
    }
    // Send Notification to the DbManager that autodiscovery is finished.
    BaseCmdInfo tmpInfo;
    tmpInfo.setType(BaseCmdInfo::CM_COMMIT_AUTODISCOVERY);
    tmpInfo.setMoc(info_->getMoc());
    tmpInfo.setMoi(info_->getMoi());
    tmpInfo.setNeId(info_->getNeId());
    mgr->notifyListeners(&tmpInfo);

    ne->setAutodiscoverMode(EMSDEF::CMAUTODISCOVER_COMPLETED);

    // notify all listeners to send all stored indications
    BaseCmdInfo cm;
    cm.setType(BaseCmdInfo::CM_COMMIT);
    cm.setMoc(info_->getMoc());
    cm.setMoi(info_->getMoi());
    cm.setNeId(info_->getNeId());
    mgr->notifyListeners(&cm);
    TL1AutodiscoverAdapter tl1Adp;
    tl1Adp.editWarningMessage(info_->getNeId());

    //  TimeCollector::end("Autodiscovery for "+info_->getNeId());

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "TL1AutodiscoveryCmd", "_discover", __LINE__, TRACE5, "Method finished");
}



//******************************************************************************
void TL1AutodiscoveryCmd::_execute()
{
    string netdiscoveryenabled = Server_i::instance()->getDefaultConfigValue("EnableNetworkAutodiscovery", "false");
    string neName = Server_i::instance()->getDefaultConfigValue (EMS_FW::ConfigNames::NeNameKey, info_->getNeId(), info_->getNeId());
    string tid = Server_i::instance()->getDefaultConfigValue (EMS_FW::ConfigNames::TID, info_->getNeId(), neName);

    map<string, string> properties;
    properties.insert (map<string, string>::value_type (ConfigNames::NeIdKey, info_->getNeId()));
    properties.insert (map<string, string>::value_type (ConfigNames::TID, tid));



    ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_execute", __LINE__, TRACE1,
                             "Autodiscovery started for [%s]", neName.c_str());


    if (netdiscoveryenabled == "true")
    {
        string state;
        state = EMSDEF::CMAUTODISCOVER_INPROGRESS;
        string description = "Autodiscovery in progress";
        Server_i::instance()->sendStateChangeEvent(state, properties, description);

    }
    string location = "NE: " + Server_i::instance()->getConfigValue(EMS_FW::ConfigNames::NeNameKey, info_->getNeId());

    TimeCollector::start ("Full Autodiscovery of " + neName);

    try
    {
        _discover(true);
    }
    catch (BaseException& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_execute", __LINE__, MINOR,
                                 "Autodiscovery of %s failed; '%s'",
                                 neName.c_str(), ex.getDescription().c_str());
        const string &errorDescription = ex.getDescription();
        string message;
        if (errorDescription.find ("Autodiscovery failed") == -1)
            message = "Autodiscovery failed; " + errorDescription;
        else
            message = errorDescription;

        // only generate a sw alarm if it is not related to STSVCMDE
        if (errorDescription.find("mode") == -1)
        {  // clear last software alarm before generating a new one
            Server_i::instance()->clearSwAlarm("SYNC", tid, "");
            Server_i::instance()->generateSwAlarm ("SYNC", tid, message);
        }

        if (netdiscoveryenabled == "true")
        {
            string state = EMSDEF::CMAUTODISCOVER_FAIL;
            Server_i::instance()->sendStateChangeEvent(state, properties, message);
        }

        if ((errorDescription.find ("Timed Out") != -1) || (errorDescription.find ("timed out") != -1))
        {
            ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_execute", __LINE__, TRACE1,
                                     "Re-sending auto-discovery request for %s", neName.c_str());
            // Re-send auto-discovery request to the Event Manager
            Server_i* srv = Server_i::instance();
            AgentCmdInfo* ainfo = new AgentCmdInfo(srv->getConfigValue(ConfigNames::RootMocKey, info_->getNeId()),
                                                   srv->getConfigValue(ConfigNames::RootMoiKey, info_->getNeId()),
                                                   BaseCmdInfo::CM_REQUESTED_AUTODISCOVERY);
            ainfo->setNeId(info_->getNeId());
            try
            {
                CmEventQueue::SendMessage(ainfo, info_->getNeId());
            }
            catch (...)
            {
                ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_execute", __LINE__, MINOR,
                                         "Failed to re-send auto-discovery request for %s", neName.c_str());
            }
        }
        throw;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_execute", __LINE__, MAJOR,
                                 "Autodiscovery of %s Terminated by Unknown Exception", neName.c_str());
        string message ("Autodiscovery failed");
        Server_i::instance()->clearSwAlarm("SYNC", tid, "");
        Server_i::instance()->generateSwAlarm ("SYNC", tid, message);
        throw;
    }
    ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_execute", __LINE__, TRACE1,
                             "Autodiscovery completed successfully for [%s]", neName.c_str());

    TimeCollector::end ("Full Autodiscovery of " + neName);

    Server_i::instance()->clearSwAlarm("SYNC", tid, "");

    if (netdiscoveryenabled == "true")
    {
        string state = EMSDEF::CMAUTODISCOVER_COMPLETED;
        string description = "Autodiscovery completed successfully";
        Server_i::instance()->sendStateChangeEvent(state, properties, description);
    }
    return ;
}

//******************************************************************************
void TL1AutodiscoveryCmd::_discoverOneFilter(const string& filter, int objsPerTxn, const string& discoverMethod)
{
    /*    ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_discoverOneFilter",
                                     __LINE__, TRACE3, "Method started with the filter=%s", filter.c_str());*/


    const string &neName = Server_i::instance()->getDefaultConfigValue (EMS_FW::ConfigNames::NeNameKey, info_->getNeId(), info_->getNeId());
    const string &tid = Server_i::instance()->getDefaultConfigValue (EMS_FW::ConfigNames::TID, info_->getNeId(), info_->getNeId());

    ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_discoverOneFilter", __LINE__, TRACE1,
                             "Autodiscovery of filter [%s:%s] started for [%s]", filter.c_str(), tid.c_str(), neName.c_str());

    TimeCollector::start ("Autodiscovery of filter " + filter + ":" + tid + " for " + neName);

    AgentCmdInfo* info = (AgentCmdInfo*) info_->clone();
    AgentFilter fl;
    fl.setCondition(filter);
    info->setFilter(fl);

    auto_ptr<TL1AutodiscoverAdapter> adp( new TL1AutodiscoverAdapter());
    adp->getElements(info);
    delete info;

    list<EMS_FW::BaseCmdInfo*> objList;
    while (adp->hasMoreElements()) // iterate through the responses
    {
        BaseCmdInfo* response = 0;
        if (!adp->getNextElement(response))
        { // never suppose to happen but it is error response!!!
            delete response;
            continue;
        }
        response->setNeId(info_->getNeId());
        if (!CmNameResolver::instance()->mocIsSupported(response->getMoc()))
        {  // unsupported class received just ignore it
            ErrorLogger::logVarError(sourceName, "CmAutodiscoveryCmd", "discover", __LINE__, TRACE1,
                                     "unsupported class -> %s", response->getMoc().c_str());
            delete response;
            continue;
        }
        _eliminateAttributes(response);
        objList.push_back(response);

        if (_cancelRequest) // cancel the current autodiscovery operation
        {
            _canceled = true;
            string neName = Server_i::instance()->getConfigValue (EMS_FW::ConfigNames::NeNameKey, info_->getNeId());
            throw BaseException("Cancel request received",
                                getClassName(), "_discover");
        }
    } // end of while

    if (objList.size() > 0)
    {
        // All MOCs should be same, so we just use the first to route handling.
        _processDiscoveredObjects(objList, filter, objList.front()->getMoc());
    }

    TimeCollector::end ("Autodiscovery of filter " + filter + ":" + tid + " for " + neName);

    ErrorLogger::logVarError(sourceName, "TL1AutodiscoveryCmd", "_discoverOneFilter", __LINE__, TRACE1,
                             "Autodiscovery of filter [%s:%s] completed successfully for [%s]", filter.c_str(), tid.c_str(), neName.c_str());

    /*    ErrorLogger::logVarError(sourceName, "CmAutodiscoveryCmd", "_discoverOneFilter", __LINE__, TRACE3, "Method finished for filter=%s", filter.c_str());*/
}
