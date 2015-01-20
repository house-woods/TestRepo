/*******************************************************************************
* COPYRIGHT 2002 - 2007 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
*
* Name: TL1ProductDecisionMaker
* Type: C++
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description:
* TL1 class implementing product behavior.
*
* Revision History:
* Rev 1.2  06/11/2003  lzou
*  add getReptEventSyntax
* Rev 1.1  12/2002   lzou
add getCommonBlockSyntaxStr
*  Rev 1.0    init version    lzou taken from AmsProductDecisionMaker
*
* $Log$
*******************************************************************************/


#include "TL1ProductDecisionMaker.h"
#include "AgentCmd.h"
#include "BaseCmdInfo.h"
#include "EMSDefines.h"
#include "EmsTypes.h"
#include "BaseException.h"
#include "TL1EventTypeResolver.h"
#include "CmNameResolver.h"
#include "TL1Utility.h"
#include "sysErrLog.h"
#include "NetworkElement.h"
#include "StringParser.h"
#include "TL1Defines.h"
#include "TL1MoLiteDbMgr.h"

using namespace std;

using namespace EMS_FW;
using namespace ELogger;
const string TL1ProductDecisionMakerName = "TL1ProductDecisionMaker"; // Class name
extern const char* sourceName;


//******************************************************************************
string TL1ProductDecisionMaker::determineEventType(const string& cmdName)
{
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineEventType",
                             TRACE3, "Resolve the event type from command name: %s", cmdName.c_str());
    BaseNameResolver* resolver = TL1EventTypeResolver::instance();
    const string& eventType = resolver->resolveName(cmdName);

    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineEventType",
                             TRACE3, "The event type resolved is %s", eventType.c_str());
    return eventType;
}


//******************************************************************************
string TL1ProductDecisionMaker::determineMoi(const string& neId,
        const list<string>& aidList,
        const string& tlMsgName)
{
    string aid;
    //Note: some responses have no aid field, so the aidList is empty.
    //In this case, we assume the moi and moc is for ManagedElement
    if (!aidList.empty())
        aid = aidList.front();
    /*
     if (aidList.empty()||aid.empty())
     {
          NetworkElement* ne = Server_i::instance()->locateNetworkElement(neId);
          string tid = ne->getDefaultConfigValue(ConfigNames::TID, "");
          aid = tid;
     }
        
     ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineMoi",
          TRACE3,"Determine the moi for aid=%s", aid.c_str());
     return TL1Utility::determineMoi(aid, neId);
    */
    return aid;
}

//******************************************************************************
string TL1ProductDecisionMaker::determineMoc(const string& neId,
        const list<string>& aidList,
        const string& tlMsgName)
{
    string moc;

    if (tlMsgName.find("SECU") != osi_npos)
    {
        moc = TL1DEF::MOC_TL1USER;
    }
    else
    {
        if (aidList.empty())
        {
            moc = TL1DEF::MOC_TL1MANAGEDELEMENT;
        }
        else
        {
            const string& aid = aidList.front();

            ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineMoc",
                                     TRACE4, "Determine the moc for aid: %s", aid.c_str());

            const string& tid = Server_i::instance()->getDefaultConfigValue(ConfigNames::TID, neId, "");
            if ( tid == aid || aid.empty())
            {
                moc = TL1DEF::MOC_TL1MANAGEDELEMENT;
            }
            else
            {
                int pos = aid.find(TL1DEF::DELIMITER_HYPHEN);
                const string& type = aid.substr(0, pos);

                try
                {
                    CmNameResolver* nameResolver = CmNameResolver::instance();
                    moc = nameResolver->resolveName(type);
                }
                catch (UnresolvedNameEx& ex)
                {
                    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineMoc", __LINE__, TRACE1,
                                             "Couldn't find corresponding MOC for aid %s. Set moc to aid", aid.c_str());

                    moc = type;
                }
            }
        }
    }
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineMoc",
                             TRACE4, "Method End, returning moc = %s", moc.c_str());

    return moc;
}



//This method is not used. Need to implement as it is pure virtual in base
//class
bool TL1ProductDecisionMaker::isAidSupported(const string& aid)
{
    return true;
}

AgentCmdInfo *TL1ProductDecisionMaker::onFilterDiscovery (const string &neId, const string &filterName)
{
    return NULL;
}

bool TL1ProductDecisionMaker::isTL1CmdSupported(const string& cmdType)
{
    BaseNameResolver* resolver = TL1EventTypeResolver::instance();
    try
    {
        string eventType = resolver->resolveName(cmdType);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

//*********************************************************************************
// The isAttributeAID operation helps choose the MOI for an incoming notification.
// Most are identified by AID.  Protection groups are identified by the
// protected facility Id.  Cross Connections are identified by the
// first termination Id.
//
bool TL1ProductDecisionMaker::isAttributeAID(const string& attrName)
{
    return false;
}


//******************************************************************************
void TL1ProductDecisionMaker::createDbObject(BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createDbObject",
                             TRACE4, "Method Begin for MOI = %s", info->getMoi().c_str());

    TL1MoLiteDbMgr* mgr = TL1MoLiteDbMgr::getInstance(info->getNeId());
    info->setType(BaseCmdInfo::CM_CREATE_EVENT);
    if (!doNotifyDb(info->getNeId(), info->getMoc()))
    {
        mgr->notifyListeners(info);

        return ;
    }

	if (info->getMoc() == "RAMAN")
	{
		mgr->createRamanEntity(info);
	}
	else
	{
		mgr->createEntity(info);
	}
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createDbObject",
                             TRACE4, "Method End for MOI = %s", info->getMoi().c_str());
}

//******************************************************************************
void TL1ProductDecisionMaker::deleteDbObject(BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "deleteDbObject",
                             TRACE4, "Method Begin for MOI = %s", info->getMoi().c_str());

    TL1MoLiteDbMgr* mgr = TL1MoLiteDbMgr::getInstance(info->getNeId());
    if (!doNotifyDb(info->getNeId(), info->getMoc()))
    {
        mgr->notifyListeners(info);
        return ;
    }


	if (info->getMoc() == "RAMAN")
	{
		mgr->deleteRamanEntity(info);
	}
	else
	{
		mgr->deleteEntity(info);
	}

    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "deleteDbObject",
                             TRACE4, "Method End for MOI = %s", info->getMoi().c_str());
}

//******************************************************************************
void TL1ProductDecisionMaker::updateDbObject(BaseCmdInfo* info)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "updateDbObject",
                             TRACE4, "Method Begin for MOI = %s", info->getMoi().c_str());
    TL1MoLiteDbMgr* mgr = TL1MoLiteDbMgr::getInstance(info->getNeId());
    if (!doNotifyDb(info->getNeId(), info->getMoc()))
    {
        mgr->notifyListeners(info);
        return ;
    }

    mgr->createEntity(info);

    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "updateDbObject",
                             TRACE4, "Method End for MOI = %s", info->getMoi().c_str());
}


//*****************************************************************************
// doNotifyDb returns true if the TL1MoLiteDbMgr handles the object.
//
bool TL1ProductDecisionMaker::doNotifyDb(const string& neId, const string& moc)
{
    return true;
}


//*****************************************************************************
void TL1ProductDecisionMaker::getCommonBlockSyntax(const string& cmd, const string& block, vector<string>& varNames)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getCommonBlockSyntax", TRACE4,
                             "Method begins for %s ", cmd.c_str());
    if (cmd == TL1DEF::EVT_SESSION)
    {
        TL1Utility::loadVarNameVector(varNames, "uid", "port", "date", "time", "msg", 0);
    }
    else
    {
        TL1Utility::loadVarNameVector(varNames, "condtype", "ntfcncde", "ocrdat", "ocrtm", "locn", "dirn", "monval", "thlev", "tmper", 0);
    }
    ErrorLogger::logError(sourceName, getClassName().c_str(), "getCommonBlockSyntax", TRACE4,
                          "Method ends ...... ");
}

//*****************************************************************************
string TL1ProductDecisionMaker::getReptEventSyntax(const string& neId, const string& code2, const string& code3)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getReptEventSyntax", TRACE7,
                             "This method should be implemented by application. ");
    string syntax;
    //    NetworkElement* ne = Server_i::instance()->locateNetworkElement(neId);
    if (code2 == TL1DEF::EVT_DBCHG)
    {
        syntax = Server_i::instance()->getDefaultConfigValue(ConfigNames::DefaultDbChgSyntax, neId,
                 "dbchg_header:cmdcode:aid:common_block:specific_block:state_block");
    }
    else if (code2 == TL1DEF::EVT_EVT)
    {
        if (code3 == TL1DEF::EVT_SESSION)
        {
            syntax = Server_i::instance()->getDefaultConfigValue("DefaultReptEvtSessionSyntax", neId,
                     "aid:common_block");
        }
        else
        {
            syntax = Server_i::instance()->getDefaultConfigValue(ConfigNames::DefaultReptEvtSyntax, neId,
                     "aid:common_block:conddescr");
        }

    }
    return syntax;
}

const string& TL1ProductDecisionMaker::getClassName()
{
    return TL1ProductDecisionMakerName;
}

//*****************************************************************************
string TL1ProductDecisionMaker::getTL1FilterFileName (const string& neId)
{
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "getTL1FilterFileName",
                             TRACE4, "Framework generic get filter for %s ", neId.c_str());

    return Server_i::instance()->getConfigValue(ConfigNames::FilterKey, neId);
}


//*****************************************************************************
void TL1ProductDecisionMaker::resetNEInfoBeforeDiscovery(const string& neId)
{
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "resetNEInfoBeforeDiscovery",
                             __LINE__, MINOR, "Unimplemented base method called for %s ", neId.c_str());

}

//*****************************************************************************
string TL1ProductDecisionMaker::determineTableName(const string& aid)
{
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "determineTableName",
                             __LINE__, MINOR, "Unimplemented base method called for %s ", aid.c_str());
    return "";
}

//*****************************************************************************
bool TL1ProductDecisionMaker::getTableandIdColName(const string& aid, const string& TL1cmd, string& tableName, string& idColName)
{
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "getTableandIdColName",
                             __LINE__, MINOR, "Unimplemented base method called for %s [%s]",
                             aid.c_str(), TL1cmd.c_str());
    return false;
}

//*****************************************************************************
bool TL1ProductDecisionMaker::getParentMoiMoc(const string& moi, const string& neId, const string& moc, string& parentMoi, string& parentMoc)
{
    ErrorLogger::logVarError(sourceName, "TL1ProductDecisionMaker", "getParentMoiMoc",
                             __LINE__, MINOR, "Unimplemented base method called for %s [%s]",
                             moi.c_str(), moc.c_str());
    return false;
}



