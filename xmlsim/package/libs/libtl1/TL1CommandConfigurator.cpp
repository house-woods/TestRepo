/***********************************************************************************************
* COPYRIGHT $Date:   Dec 14, 2004  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name:             TL1CommandConfigurator.cpp
* Type:             C++ source
* Originator:       Altaf Aali
* File: $Workfile:  TL1CommandConfigurator.cpp
* Description:      Class responsible for initializing and providing access to configuration
*                   paramaters for TL1 commands such as timesouts, syntax etc
* 
*                   !!foo-bar are not mythical!!
**********************************************************************************************/

#include <string>
#include <map>

using namespace std;

#include "TL1CommandConfigurator.h"
#include "sysErrLog.h"
#include "FileReader.h"
#include "tutil.h"
#include "SystemConfigMgr.h"

using namespace EMS_FW;
using namespace ELogger;

extern const char *sourceName;
static const char* className = "TL1CommandConfigurator";

TL1CommandConfigurator* TL1CommandConfigurator::_instance = NULL;

TL1CommandConfigurator::TL1CommandConfigurator()
{}

TL1CommandConfigurator::~TL1CommandConfigurator()
{}

TL1CommandConfigurator* TL1CommandConfigurator::instance ()
{
    if (_instance == NULL)
        _instance = new TL1CommandConfigurator();
    return _instance;
}

void TL1CommandConfigurator::initializeMSECTimeouts (unsigned long defaultMSECTimeout)
{
    ErrorLogger::logVarError(sourceName, "TL1CommandConfigurator", "initializeMSECTimeouts", TRACE1, "Started");
    _defaultMSECTimeout = defaultMSECTimeout;
    map<string, string> timesoutsInConfig;
    SystemConfigMgr::instance()->loadSystemConfig ("TL1Timeout", "", timesoutsInConfig);
    for (map<string, string>::iterator i = timesoutsInConfig.begin();i != timesoutsInConfig.end();i++)
    {
        string key = (*i).first;
        Tutil::toLowerCase (key);
        ErrorLogger::logVarError(sourceName, "TL1CommandConfigurator", "initializeMSECTimeouts", TRACE1, "Configuring command [%s] to have a timeout of [%s] milliseconds", key.c_str(), (*i).second.c_str());
        _commandToTimeoutMap [key] = (*i).second;
    }
    ErrorLogger::logVarError(sourceName, "TL1CommandConfigurator", "initializeMSECTimeouts", TRACE1, "Finished");
}

unsigned long TL1CommandConfigurator::getMSECTimeout (string command, const string &version)
{
    unsigned long timeout;

    Tutil::toLowerCase (command);
    map<string, string>::iterator i = _commandToTimeoutMap.find (command);
    if (i != _commandToTimeoutMap.end())
    {
        const string &timeoutString = (*i).second;
        try
        {
            timeout = atoi (timeoutString.c_str());
        }
        catch (...)
        {
            timeout = _defaultMSECTimeout;
        }
    }
    else
    {
        timeout = _defaultMSECTimeout;
    }
    return timeout;
}
