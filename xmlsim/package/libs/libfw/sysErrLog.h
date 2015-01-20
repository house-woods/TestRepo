//
// System error logger classes
//
// Usage: The error logger is implemented as a singleton. The user is required to
// obtain a reference to the error logger by invoking ErrorLogger::instance(). The
// instance calls takes two parameters, location and severity level. Location can
// be "local" or "system", with "system" been the default. Local logging will log
// to a file name "local_errors.log". System logging will require the system error
// logging running on Corba. The system logging will log errors to a file name
// "btm_errors.log". The second parameter takes a severity level, which gives the
// lowest severity level before a special action is taken. Currently, the lowest
// severity level is defaulted to Critical. To log error, invoke
//
//  ErrorLogger::logError(source, type, data)
//
// where 'source' is the source of the error message, ie FMR, CMW, SM, and etc.
// 'type' is the error type, ie Trace, Minor, Major, Critical. Trace will only send
// the message to the console.
// 'data' is the error message
//
// To log message with variable parameters, invoke
//
//  ErrorLogger::logVarError(source, type, desc, ...)
//
// where 'desc' is the format, which works similar to printf format.
//

#ifndef _SYSERRLOG_H_
#define _SYSERRLOG_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <list>
#include <ospace/file.h>
//#include <iostream>

//#include <ospace/stream.h>
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif


//#include "errLog.hh"

#include "NsgErrorDef.h"
//#include "ComManager.h"
#include <ospace/thread.h>
#include <iostream>
#include <fstream>
//#include <ospace/file.h>


namespace ELogger
{
class LoggerException
    {}
;

class ErrorLogger
{
public:

    static ErrorLogger* instance(const char* errorLoggerLocation = "local", err_type severityLevel = CRITICAL);

    static bool isErrTypeEnabled(err_type type)
    {
        return (type & _traceMask) || (type >= MINOR);
    }

    static void logError(const char* source, err_type type, const char* data);
    static void logError(const char* source, const char* className, err_type type, const char* data);
    static void logError(const char* source, const char* className, const char* methodName, err_type type, const char* data);
    static void logError(const char* source, const char* className, const char* methodName, int lineNumber, err_type type, const char* data);
    static void logVarError(const char* source, err_type type, const char* desc, ...);
    static void logVarError(const char* source, const char* className, err_type type, const char* desc, ...);
    static void logVarError(const char* source, const char* className, const char* methodName, err_type type, const char* desc, ...);
    static void logVarError(const char* source, const char* className, const char* methodName, int lineNumber, err_type type, const char* desc, ...);
    static void dumpTraceList();
    static list<string>* getTraceList()
    {
        return &_traceList;
    }

    void setErrorLogFlag(bool st)
    {
        _errorLogOn = st;
    }
    void enableTrace(err_type mask)
    {
        _traceMask |= mask;
    }
    void enableAllTraces()
    {
        _traceMask |= 0x1FFF;
    }
    void disableTrace(err_type mask)
    {
        _traceMask &= ~mask;
    }
    static bool isTraceListOn()
    {
        return true;
    }
    static bool isErrorLogOn()
    {
        return _errorLogOn;
    }
    static void writeToLogFile(string header, string data);
    void setCheckHeap(bool st = true)
    {
        _checkHeap = st;
    }
    static bool checkHeap();
    static string getCurrentThreadId();
    void enforceInstance(ErrorLogger* pInstance);
    static void setExceptionLogging(bool state)
    {
        _excWriteOn = state;
    }
    static void setAgentLogging(bool state)
    {
        _agentLogOn = state;
    }
    // if exception logging is enabled/default state/ everytim when
    // dumpTraceList is called the content of the list will be logged
    // into the additional file that is never wrapped.
    static void getTime(char* pTimeStr);
    static void maskPassword(const string& msg, string& duplicateMessage);

protected:

    typedef osi_sem_lock<osi_mutex_semaphore> TimeLock;

    static osi_mutex _timeMutex;
    static osi_mutex _traceMutex;
    osi_mutex _guardMutex;

    static bool _excWriteOn;
    static bool _agentLogOn;


    static bool _traceListOn;
    static bool _checkHeap;
    static list<string> _traceList;
    static err_type _traceMask;

    static ofstream* m_errFile;
    static int _logSize;

    //used for turn on or off the error log
    static bool _errorLogOn;

    //#ifndef OS_USE_ALTERNATE_STD
    static osi_file* _logFile;
    static osi_file* _excFile;
    static osi_file* _agentFile;


    static bool _logToConsoleOnly;
    //#endif

    //static osi_otstream* _logStream;
    static err_type m_lowestSeverityLevel;
    static ErrorLogger* m_instance;

    ErrorLogger()
    {}
    ;
    virtual ~ErrorLogger();
    virtual void logErrorMsg(const char* source, err_type type, const char* data) = 0;
    virtual void logSevereErrorMsg(const char* source, err_type type, const char* header, const char* data) = 0;
};

class LocalErrorLogger : public ErrorLogger
{
public:
    static void startLogging(string fileName, err_type mask, int size = 100);
    static void stopLogging();
    void logErrorMsg(const char* source, err_type type, const char* data);

    friend ErrorLogger;

protected:

    LocalErrorLogger() throw (LoggerException);
    ~LocalErrorLogger();
    void logSevereErrorMsg(const char* source, err_type type, const char* header, const char *data);
    static bool _consoleOutputEnabled;

};

class SysErrorLogger : public ErrorLogger
{
public:

    void logErrorMsg(const char* source, err_type type, const char* data);

    friend ErrorLogger;

protected:

    //  static SysError_var m_seVar;
    //  static ComManager* m_pCM;

    SysErrorLogger() throw (LoggerException);
    ~SysErrorLogger();
    void logSevereErrorMsg(const char* source, err_type type, const char* header, const char *data);
};
};

#endif // _SYSERRLOG_H

