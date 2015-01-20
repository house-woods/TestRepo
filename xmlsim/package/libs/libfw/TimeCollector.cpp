


/***********************************************************************************************
* COPYRIGHT $Date:   July 23 2000 14:39:38  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: TimeCollector
* Type: C++
* Originator: $Author:
* File: $Workfile:
* Version: $Revision:
* Description:
* Utility class used to collect time reports for debugging purposes.
*
* Revision History:
*
* $Log:
*
* File Dependencies:
*
************************************************************************************************/


#include "TimeCollector.h"

#include "sysErrLog.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include <ospace/time.h>
#include <ospace/file.h>
#include "EmsUnixDefines.h"


extern const char* sourceName;

using namespace ELogger;

using namespace EMS_FW;

TimeCollector::timeMapType TimeCollector::_timeMap;
map<string, bool> TimeCollector::_precisionProfilingEnabledMap;
map<string, osi_file*> TimeCollector::_profileLogMap;
bool TimeCollector::_precisionProfilingEnabled;
osi_file* TimeCollector::_profileLog;

osi_mutex TimeCollector::_messageMutex;

void TimeCollector::initialize (bool precisionProfilingEnabled, const string &profileLogFilename)
{
    if (profileLogFilename.empty())
        return ;
    map<string, bool>::iterator iter1;
    iter1 = _precisionProfilingEnabledMap.find(profileLogFilename);
    if (iter1 == _precisionProfilingEnabledMap.end())
    {
        _precisionProfilingEnabledMap.insert(pair<string, bool>(profileLogFilename, precisionProfilingEnabled));
    }

    if (!precisionProfilingEnabled)
        return ;
    map<string, osi_file*>::iterator iter;
    iter = _profileLogMap.find(profileLogFilename);
    if (iter == _profileLogMap.end())
    {
        string oldFilename = profileLogFilename + ".old";
        if (osi_file_system::exists (oldFilename))
            osi_file_system::remove
            (oldFilename);

        if (osi_file_system::exists(profileLogFilename))
            osi_file_system::rename(profileLogFilename, oldFilename);

        osi_file* profileLog = new osi_file(profileLogFilename.c_str(), osi_open_control::open_always , O_RDWR);
        if (!profileLog->good())
        {
            ErrorLogger::logVarError(sourceName, "TimeCollector", "suspend", __LINE__, MINOR, "Could not create profile log file %s", profileLogFilename.c_str());
            delete profileLog;
            profileLog = NULL;
            return ;
        }
        ErrorLogger::logVarError(sourceName, "TimeCollector", "suspend", __LINE__, MINOR, "Profile log file %s created successfully", profileLogFilename.c_str());
        profileLog->seek (SEEK_SET, 0);
        _profileLogMap.insert(pair<string, osi_file*>(profileLogFilename, profileLog));

    }

}

void TimeCollector::initializeDefaultProfile (bool precisionProfilingEnabled, const string &profileLogFilename)
{
    _precisionProfilingEnabled = precisionProfilingEnabled;
    _profileLog = NULL;
    if (precisionProfilingEnabled)
    {
        string oldFilename = profileLogFilename + ".old";
        if (osi_file_system::exists (oldFilename))
            osi_file_system::remove
            (oldFilename);

        if (osi_file_system::exists(profileLogFilename))
            osi_file_system::rename(profileLogFilename, oldFilename);

        _profileLog = new osi_file(profileLogFilename.c_str(), osi_open_control::open_always , O_RDWR);
        if (!_profileLog->good())
        {
            ErrorLogger::logVarError(sourceName, "TimeCollector", "suspend", __LINE__, MINOR, "Could not create profile log file %s", profileLogFilename.c_str());
            delete _profileLog;
            _profileLog = NULL;
            return ;
        }
        ErrorLogger::logVarError(sourceName, "TimeCollector", "suspend", __LINE__, MINOR, "Profile log file %s created successfully", profileLogFilename.c_str());
        _profileLog->seek (SEEK_SET, 0);

    }

}


void TimeCollector::finalize (void)
{
    map<string, osi_file*>::iterator iter;
    for (iter = _profileLogMap.begin();iter != _profileLogMap.end();iter++)
    {
        osi_file* profileLog = (*iter).second;
        profileLog->close();
        delete profileLog;
        profileLog = NULL;
    }
    _profileLogMap.clear();
    _precisionProfilingEnabledMap.clear();
    if (_profileLog)
    {
        _profileLog->close();
        delete _profileLog;
        _profileLog = NULL;
    }

}

void TimeCollector::finalize (const string& profileLogFilename)
{
    if (profileLogFilename.empty())
        return ;
    map<string, osi_file*>::iterator iter;
    iter = _profileLogMap.find(profileLogFilename);
    if (iter != _profileLogMap.end())
    {
        osi_file* profileLog = (*iter).second;
        _profileLogMap.erase(iter);
        profileLog->close();
        delete profileLog;
        profileLog = NULL;
    }
    map<string, bool>::iterator iter1;
    iter1 = _precisionProfilingEnabledMap.find(profileLogFilename);
    if (iter1 != _precisionProfilingEnabledMap.end())
    {
        _precisionProfilingEnabledMap.erase(iter1);
    }



}

void TimeCollector::start(const string& key, const string& profileLogFilename)
{
    bool precisionProfilingEnabled = false;
    if (!profileLogFilename.empty())
    {
        if (profileLogFilename == "DEFAULT")
        {
            precisionProfilingEnabled = _precisionProfilingEnabled;
        }
        else
        {
            map<string, bool>::iterator iter1;
            iter1 = _precisionProfilingEnabledMap.find(profileLogFilename);
            if (iter1 != _precisionProfilingEnabledMap.end())
            {
                precisionProfilingEnabled = (*iter1).second;
            }
        }

    }

    char buffer[20];
    osi_thread_t id = osi_this_thread::tid();
#if defined(WIN32)

    unsigned long ul = id.tid_;
#else

    unsigned long ul = (unsigned long) id;
#endif

    _ultoa( ul, buffer, 10 );
    string completeKey = key + "::" + string (buffer);

    MessageLock lock (_messageMutex)
    ;
    timeMapType::iterator p = _timeMap.find(completeKey);

    CollectTime ct;

    if (p != _timeMap.end()) // key is  in a map
    {
        ct = (*p).second;
    }

    if (precisionProfilingEnabled)
    {
        ct.stopWatch.reset ();
        ct.stopWatch.start ();
    }
    else
    {
        time_t currentTime;
        time(&currentTime);
        ct.startTime = currentTime;
        ct.suspendedTime = 0;
        ct.suspendTime = 0;
    }
    _timeMap[completeKey] = ct;
}

void TimeCollector::end(const string& key, const string& profileLogFilename)
{
    // get the thread id
    char buffer[20];
    osi_thread_t id = osi_this_thread::tid();
#if defined(WIN32)

    unsigned long ul = id.tid_;
#else

    unsigned long ul = (unsigned long) id;
#endif

    _ultoa( ul, buffer, 10 );

    // append it to the provided key to get a unique across threads
    string completeKey = key + "::" + string (buffer);

    MessageLock lock (_messageMutex)
    ;
    timeMapType::iterator p = _timeMap.find(completeKey);
    if (p != _timeMap.end()) // key is not in a map need to be created
    {
        printReport(completeKey, (*p).second, profileLogFilename);
        _timeMap.erase(p);
    }
}

void TimeCollector::collect(const string& key)
{
    MessageLock lock (_messageMutex)
    ;
    timeMapType::iterator p = _timeMap.find(key);
    if (p == _timeMap.end()) // key is  not in a map
    {
        ErrorLogger::logVarError(sourceName, "TimeCollector", "collect", __LINE__, TRACE1, "Key %s is not in a map", key.c_str());
        return ;
    }
    CollectTime& ct = (*p).second;
    time_t currentTime;
    time(&currentTime);
    if (ct.suspendTime != 0)
    {
        // add the suspended time
        unsigned int st = currentTime - ct.suspendTime;
        ct.suspendTime = 0;
        ct.suspendedTime += st;
    }
}


void TimeCollector::suspend(const string& key)
{
    MessageLock lock (_messageMutex)
    ;
    timeMapType::iterator p = _timeMap.find(key);
    if (p == _timeMap.end()) // key is  not in a map
    {
        ErrorLogger::logVarError(sourceName, "TimeCollector", "suspend", __LINE__, TRACE1, "Key %s is not in a map", key.c_str());
        return ;
    }
    CollectTime& ct = (*p).second;
    time_t currentTime;
    time(&currentTime);
    ct.suspendTime = currentTime;
}


void TimeCollector::printReport(const string& key, CollectTime ct, const string& profileLogFilename)
{
    bool precisionProfilingEnabled = false;
    osi_file* profileLog = NULL;

    if (!profileLogFilename.empty())
    {
        if (profileLogFilename == "DEFAULT")
        {
            precisionProfilingEnabled = _precisionProfilingEnabled;
            profileLog = _profileLog;
        }
        else
        {
            map<string, osi_file*>::iterator iter;
            iter = _profileLogMap.find(profileLogFilename);
            if (iter != _profileLogMap.end())
            {
                profileLog = (*iter).second;
            }
            map<string, bool>::iterator iter1;
            iter1 = _precisionProfilingEnabledMap.find(profileLogFilename);
            if (iter1 != _precisionProfilingEnabledMap.end())
            {
                precisionProfilingEnabled = (*iter1).second;
            }

        }

    }
    if (precisionProfilingEnabled)
    {
        ct.stopWatch.stop ();
        string lapTime = ct.stopWatch.lap().to_string();
        ErrorLogger::logVarError(sourceName, "TimeCollector", "printReport", __LINE__, TRACE5, "Collected result for key=%s totalTime=%s suspTime=%d", key.c_str(), lapTime.c_str(), ct.suspendedTime);

        struct tm *datetime;
        struct tm result;
        time_t current_time;
        time(&current_time);
        datetime = localtime_r(&current_time, &result);
        char buffer[128];
        strftime(buffer, sizeof(buffer), "%m-%d-%y %X", datetime);

        string message = string(buffer) + "  " + key + string ("::") + lapTime;
        profileLog->write (message.c_str(), message.size());
        string crlf = "\n";
        profileLog->write (crlf.c_str(), crlf.size());
    }
    else
    {
        time_t currentTime;
        time(&currentTime);
        unsigned int totalTime = currentTime - ct.startTime;
        ErrorLogger::logVarError(sourceName, "TimeCollector", "printReport", __LINE__, TRACE5, "Collected result for key=%s totalTime=%d suspTime=%d", key.c_str(), totalTime, ct.suspendedTime);
    }
}

