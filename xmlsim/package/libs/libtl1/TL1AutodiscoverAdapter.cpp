#include "SocketHandler.h"
#include "TL1AutodiscoverAdapter.h"
#include "AgentCmdInfo.h"
#include "sysErrLog.h"
#include "BaseException.h"
#include "BaseCmd.h"
#include "BaseCmdFactory.h"
#include "AgentCmd.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include "NetworkElement.h"
#include "TL1CmdFactory.h"
#include "TL1GenericCmd.h"
#include "TL1CmdInfo.h"
#include "TL1Utility.h"
#include "RdbUtil.h"


extern const char* sourceName;
extern const char* ServerIdName;


using namespace EMS_FW;
using namespace ELogger;


TL1AutodiscoverAdapter::TL1AutodiscoverAdapter()
{
    _cmd = NULL;
}

TL1AutodiscoverAdapter::~TL1AutodiscoverAdapter()
{
    if (_cmd)
    {
        delete _cmd;
        _cmd = NULL;
    }
}

//==============================================================================
void TL1AutodiscoverAdapter::getElements(const EMS_FW::BaseCmdInfo* discInfo)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "getElements", TRACE4,
                                 "Method start for moi=%s", discInfo->getMoi().c_str());

    AgentCmdInfo* inf = (AgentCmdInfo*) discInfo;
    _fl = inf->getFilter();
    string filter = _fl.getCondition();
    _neId = discInfo->getNeId();

    BaseCmdInfo* cmdInfo = NULL;
    cmdInfo = TL1Utility::createTL1CmdInfoFromFilter(_neId, filter);
    // clean the memory before reassign another value
    if (_cmd)
    {
        delete _cmd;
        _cmd = NULL;
    }
    _cmd = (TL1GenericCmd*) BaseCmdFactory::instance()->createCommand(cmdInfo, TL1CmdFactory::TL1);
    if (!_cmd)
    {
        string message = "Failed to create the TL1 command for " + filter +
                         " in autodiscovery of " + _neId;
        ErrorLogger::logError(sourceName, "TL1AutodiscoverAdapter", "getElements",
                              MAJOR, message.c_str());
        if (cmdInfo)
        {
            delete cmdInfo;
            cmdInfo = NULL;
        }
        throw BaseException(message.c_str(), "TL1AutodiscoverAdapter", "getElements");
    }
    ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "getElements", TRACE4,
                             "Created the TL1 command for %s to do autodiscovery of %s", filter.c_str(), _neId.c_str());

    CmdResult res = _cmd->execute();

    string neName = Server_i::instance()->getDefaultConfigValue (EMS_FW::ConfigNames::NeNameKey, _neId, _neId);
    if (!res.getStatus()) // If unable to execute the TL1 command
    {
        string errorMessage = "Autodiscovery failed for " + neName + " on command '" + filter +
                              "': " + res.getResultInfo();
        ErrorLogger::logError(sourceName, "TL1AutodiscoverAdapter", "getElements", MAJOR, errorMessage.c_str());
        throw BaseException(errorMessage, "TL1AutodiscoverAdapter", "getElements");
    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "getElements", TRACE4,
                                 "Method finished for moi=%s, filter=%s", discInfo->getMoi().c_str(), filter.c_str());
}


//==============================================================================
bool TL1AutodiscoverAdapter::getNextElement(EMS_FW::BaseCmdInfo*& info)
{
    info = new TL1CmdInfo();
    return _cmd->getNextElement(info);
}


bool TL1AutodiscoverAdapter::hasMoreElements()
{
    bool result = _cmd->hasMoreElements();
    return result;
}
void TL1AutodiscoverAdapter::editWarningMessage(const string& neId)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "editWarningMessage", TRACE1,
                                 "Method start for neId=%s", neId.c_str());
    try
    {
        string enableSetWarn("NO");
        RdbUtil::retrieveDefaultProvisionAttributes("Enable", "WarningMessage", enableSetWarn);
        if (0 == enableSetWarn.compare("YES"))
        {
            if (ErrorLogger::isTraceListOn())
                ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "editWarningMessage", TRACE1,
                                         "Set warning message is enabled for neId=%s", neId.c_str());
            vector <string> commonBlk;
            string warnMsg("");
            RdbUtil::retrieveDefaultProvisionAttributes("Warning", "WarningMessage", warnMsg);
            if (0 == warnMsg.compare("-"))
            {
                //we insert - instead of empty.
                warnMsg = "\"\"";
            }
            else
            {
                warnMsg.insert(0,"\"");
                warnMsg.append("\"");
            }
            commonBlk.push_back(warnMsg);
    
            EMS_FW::BaseCmdInfo* cmdInfo = NULL;
            cmdInfo = TL1Utility::createTL1CmdInfo(neId, "ED-WARNING", "", "", commonBlk);
    
            TL1InfoList res;
            const CmdResult& result = TL1Utility::sendCommand(cmdInfo, res);
            if (!result.getStatus())
            {
                ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "editWarningMessage", __LINE__, MAJOR, "ED-WARNING error");
            }
        }
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "editWarningMessage", __LINE__, MAJOR, "%s", ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "editWarningMessage", __LINE__, MAJOR, "Unknown Exception");
    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "TL1AutodiscoverAdapter", "editWarningMessage", TRACE1,
                                 "Method finished for neId=%s", neId.c_str());
}
