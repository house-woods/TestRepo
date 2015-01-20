/*******************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:28:42  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: TL1DbEventHandler
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1DbEventHandler.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* Process updates for Mo DataBase.
* 
* Revision History:
*
* File Dependencies: 
* <List of include files this file requires> 
*******************************************************************************/

#include "TL1DbEventHandler.h"
#include "BaseCmdInfo.h"
#include "TL1MoLiteDbMgr.h"
#include "TL1ProductDecisionMaker.h"
#include "sysErrLog.h"



using namespace ELogger;
using namespace EMS_FW;
extern const char* sourceName;

//******************************************************************************
TL1DbEventHandler::TL1DbEventHandler()
{}

TL1DbEventHandler::~TL1DbEventHandler()
{}

DbEventHandeler* TL1DbEventHandler::clone()
{
    return new TL1DbEventHandler();
}

//******************************************************************************
bool TL1DbEventHandler::handleEvent(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleEvent", __LINE__,
                                 TRACE4, "Method Begin");
    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    if (!pdMaker->doNotifyDb(_neId, info->getMoc()))
    {
        TL1MoLiteDbMgr* mgr = (TL1MoLiteDbMgr*)TL1MoLiteDbMgr::getInstance(_neId);
        mgr->notifyListeners(info);
    }
    else
        if (!EventHandeler::handleEvent(info))
        { // event was not processed by default handelers send it as it is to the listeners
            TL1MoLiteDbMgr* mgr = (TL1MoLiteDbMgr*)TL1MoLiteDbMgr::getInstance(_neId);
            mgr->notifyListeners(info);
        }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleEvent", __LINE__,
                                 TRACE4, "Method End");
    return true;
}

bool TL1DbEventHandler::handleEvent(const EMS_FW::BaseCmdInfoList* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleEvent LIST", __LINE__,
                                 TRACE4, "Method Begin");
    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    // EventHandeler::handleEvent(info);
    TL1MoLiteDbMgr* mgr = (TL1MoLiteDbMgr*)TL1MoLiteDbMgr::getInstance(_neId);
    mgr->notifyListeners(info);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleEvent LIST", __LINE__,
                                 TRACE4, "Method End");
    return true;
}


//******************************************************************************
void TL1DbEventHandler::handleCreateEvent(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleCreateEvent", __LINE__,
                                 TRACE4, "Method Begin");

    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    EMS_FW::BaseCmdInfo* eventInfo = (EMS_FW::BaseCmdInfo*)info;

    pdMaker->createDbObject(eventInfo);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleCreateEvent", __LINE__,
                                 TRACE4, "Method End");
}


//******************************************************************************
void TL1DbEventHandler::handleDeleteEvent(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleDeleteEvent", __LINE__,
                                 TRACE4, "Method Begin");

    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    EMS_FW::BaseCmdInfo* eventInfo = (EMS_FW::BaseCmdInfo*)info;
    pdMaker->deleteDbObject(eventInfo);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleDeleteEvent", __LINE__,
                                 TRACE4, "Method End");
}


//******************************************************************************
void TL1DbEventHandler::setAttributes(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "setAttributes", __LINE__,
                                 TRACE4, "Method Begin");

    TL1MoLiteDbMgr* mgr = (TL1MoLiteDbMgr*)TL1MoLiteDbMgr::getInstance(_neId);
    TL1ProductDecisionMaker* pdMaker = (TL1ProductDecisionMaker*)
                                       ProtocolProductDecisionMaker::getProductDecisionMaker();

    EMS_FW::BaseCmdInfo* eventInfo = (EMS_FW::BaseCmdInfo*) info;
    pdMaker -> updateDbObject (eventInfo);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "setAttributes", __LINE__,
                                 TRACE4, "Method End");
}

//******************************************************************************
void TL1DbEventHandler::handleAVCEvent(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleAVCEvent", __LINE__,
                                 TRACE4, "Method Begin");
    setAttributes(info);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleAVCEvent", __LINE__,
                                 TRACE4, "Method End");
}

//******************************************************************************
void TL1DbEventHandler::handleSCEvent(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleSCEvent", __LINE__,
                                 TRACE4, "Method Begin");
    setAttributes(info);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "handleSCEvent", __LINE__,
                                 TRACE4, "Method End");
}



DbMgr* TL1DbEventHandler::getDbMgr()
{

    return (DbMgr*)TL1MoLiteDbMgr::getInstance(_neId);

}



void TL1DbEventHandler::abortUpdates(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "abortUpdates", __LINE__,
                                 TRACE4, "Method Begin");

    TL1MoLiteDbMgr::getInstance(_neId)->abortUpdates(info);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "abortUpdates", __LINE__,
                                 TRACE4, "Method End");
}


void TL1DbEventHandler::commitUpdates(const EMS_FW::BaseCmdInfo* info)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "commitUpdates", __LINE__,
                                 TRACE4, "Method Begin");

    TL1MoLiteDbMgr::getInstance(_neId)->commitUpdates(info);

    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1DbEventHandler", "commitUpdates", __LINE__,
                                 TRACE4, "Method End");

}
