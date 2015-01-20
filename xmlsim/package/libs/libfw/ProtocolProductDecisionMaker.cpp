#include "ProtocolProductDecisionMaker.h"
#include "NetworkElement.h"
#include "RdbUtil.h"
#include "RdbRefEntityTable.h"
#include "StringParser.h"
#include "Server_i.h"
#include "ConfigNames.h"


using namespace EMS_FW;


//******************************************************************************
string ProtocolProductDecisionMaker::determineMoi(const string& neId,
        const list<string>& aidList,
        const string& tlMsgName)
{
    string aid;
    // Note: some responses have no aid field, so the aidList is empty.
    // In this case, we assume the moi and moc is for ManagedElement
    if (!aidList.empty())
        aid = aidList.front();
    if (aidList.empty() || aid.empty())
    {
        NetworkElement* ne = Server_i::instance()->locateNetworkElement(neId);
        aid = ne->getDefaultConfigValue(ConfigNames::TID, "");
    }
    string entityType;
    return RdbUtil::getEntityIdandType(neId, aid, entityType);
}

//******************************************************************************
string ProtocolProductDecisionMaker::determineMoc(const string& neId,
        const list<string>& aidList,
        const string& tlMsgName)
{
    string entityType;
    string aid;

    if (!aidList.empty())
        aid = aidList.front();
    if (aidList.empty() || aid.empty())
    {
        NetworkElement* ne = Server_i::instance()->locateNetworkElement(neId);
        aid = ne->getDefaultConfigValue(ConfigNames::TID, "");
    }
    RdbUtil::getEntityIdandType(neId, aid, entityType);
    return entityType;
}




bool ProtocolProductDecisionMaker::isAidSupported(const string& aid)
{
    return true;
}


bool ProtocolProductDecisionMaker::isTL1CmdSupported(const string& cmdType)
{
    return true;
}

