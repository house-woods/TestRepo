
#include "TL1NESecurityObject.h"

#include "BaseCmdInfo.h"
#include "ConfigNames.h"
#include "EmsGUIHandeler.h"
#include "GuiCmdInfo.h"
#include "MoAttributePo.h"
#include "TL1MoLiteDbMgr.h"
#include "TL1Utility.h"
#include "sysErrLog.h"
#include "EmsTypes.h"
#include "CMSessionManager.h"
#include "Node.hh"

extern const char* sourceName;

using namespace ELogger;
using namespace EMS_FW;

const string TL1NESecurityObject::_className = "TL1NESecurityObject";
const string TL1NESecurityObject::AS_KEY_TL1USERS = "TL1UsersList";

osi_mutex TL1NESecurityObject::_timeMutex;
TL1NESecurityObject::InterestedClientsMap TL1NESecurityObject::_clientsMap;

TL1NESecurityObject::TL1NESecurityObject()
{}

TL1NESecurityObject::~TL1NESecurityObject()
{}

void TL1NESecurityObject::handleCreateEvent (const BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "handleCreateEvent", __LINE__,
                             TRACE4, "Method Begin");
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "handleCreateEvent", __LINE__,
                             TRACE4, "Method End");
}

void TL1NESecurityObject::handleDeleteEvent (const BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "handleDeleteEvent", __LINE__,
                             TRACE4, "Method Begin");

    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "handleDeleteEvent", __LINE__,
                             TRACE4, "Method End");
}

void TL1NESecurityObject::getAttributes (const GuiCmdInfo* info, list<MoAttributePo*>& attrList)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getAttributes", __LINE__,
                             TRACE4, "Method Begin");

    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getAttributes", __LINE__,
                             TRACE4, "Method End");
}

void TL1NESecurityObject::action(const GuiCmdInfo* info, list<MoAttributePo*>& resList)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "action", __LINE__,
                             TRACE4, "Method Begin");

    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "action", __LINE__,
                             TRACE4, "Method End");
}

bool TL1NESecurityObject::interestedEvent (const BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "interestedEvent", __LINE__,
                             TRACE4, "Method Begin");

    const string& eventType = info->getType();
    const string& moc = info->getMoc();

    if (eventType == GuiCmdInfo::CM_ACTION_REQUEST &&
        (((GuiCmdInfo*)(info))->getActionType() == TL1DEF::AT_GETALLTL1USERS ||
         ((GuiCmdInfo*)(info))->getActionType() == TL1DEF::AT_UNREGISTERSMNOTIFICATIONS))
    {
        return true;
    }

    if ((moc == getTL1UserMoc()) &&
        (eventType == BaseCmdInfo::CM_CREATE_EVENT ||
         eventType == BaseCmdInfo::CM_DELETE_EVENT ||
         eventType == BaseCmdInfo::CM_AVC_EVENT ||
         eventType == GuiCmdInfo::CM_GET_REQUEST ||
         eventType == GuiCmdInfo::CM_SET_REQUEST ||
         eventType == GuiCmdInfo::CM_ACTION_REQUEST ||
         eventType == "Properties" ) )
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "interestedEvent", __LINE__,
                                 TRACE4, "Method End");
        return true;
    }

    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "interestedEvent", __LINE__,
                             TRACE4, "Method End");

    return false;
}

const string& TL1NESecurityObject::getClassName() const
{
    return _className;
}

string TL1NESecurityObject::getTL1UserMoc()
{
    return TL1DEF::MOC_TL1USER;
}

void TL1NESecurityObject::_insertInterestedClient(const string& NeId, const EMS_FW::SESSIONIDTYPE& sessionId)
{
    TimeLock lock (_timeMutex)
        ;

    int maxClients = _clientsMap.count(NeId);
    InterestedClientsIter iter = _clientsMap.find(NeId);

    while ((iter != _clientsMap.end()) && (maxClients > 0) )
    {
        EMS_FW::SESSIONIDTYPE storedSesId = (*iter).second;
        if (sessionId == storedSesId)
            break;
        iter++;
        maxClients--;
    }

    if (maxClients == 0)
        _clientsMap.insert(InterestedClientsMap::value_type(NeId, sessionId));
}

void TL1NESecurityObject::_removeNotInterestedClient(const EMS_FW::SESSIONIDTYPE& sessionId)
{
    TimeLock lock (_timeMutex)
        ;

    InterestedClientsIter iter = _clientsMap.begin();
    while (iter != _clientsMap.end())
    {
        InterestedClientsIter nextIter = iter++;
        if (sessionId == (EMS_FW::SESSIONIDTYPE)(*nextIter).second)
        {
            _clientsMap.erase(nextIter);
        }
    }
}

void TL1NESecurityObject::_sendIndicationToClients(Node::Indication& nodeIndication)
{
    TimeLock lock (_timeMutex)
        ;

    string neId = getNeId();
    int maxClients = _clientsMap.count(neId);
    InterestedClientsIter iter = _clientsMap.find(neId);

    while ((iter != _clientsMap.end()) && (maxClients > 0))
    {
        EMS_FW::SESSIONIDTYPE storedSessId = (*iter).second;

        CMSessionManager::Instance()->publish(nodeIndication, storedSessId);

        iter++;
        maxClients--;
    }
}

