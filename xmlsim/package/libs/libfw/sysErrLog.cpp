/***********************************************************************************************
* COPYRIGHT $Date:   Oct 28 1999 17:34:48  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: sysErrLog
* Type: C++ Source
* Originator: $Author:   pstork  $
* File: $Workfile:   sysErrLog.cpp  $
* Version: $Revision:   1.7  $
* Description:
* This class will handle tracing and error message logging.
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/sysErrLog.cpv  $

Rev 1.7   Oct 28 1999 17:34:48   pstork
Update include lines for ospace headers so that client
can compile. (iostream stuff)

Rev 1.6   Oct 27 1999 14:48:08   dtchalak
Query Interface updates.

Rev 1.5.1.0   Sep 20 1999 16:50:34   dtchalak
Just to create a branch.

Rev 1.5   08 Sep 1999 14:34:50   dtchalak
USE_ALTERNATE_STD added.

Rev 1.4   03 Sep 1999 11:23:58   aloo
Upgraded for Framework 1.1

Rev 1.3   21 Jul 1999 10:39:24   dtchalak
Multiple Ne support included.

Rev 1.2   18 Jun 1999 10:45:38   jfrasset
Reset the OS_USE_ALTERNATE_STD parameter for including header files so the expected string template is compiled.

Rev 1.1   04 May 1999 19:16:58   dtchalak
Comments added.
//
//    Rev 1.2   03 Mar 1999 09:35:10   aloo
//
//
//    Rev 1.1   24 Feb 1999 09:47:58   aloo
// Updated for handle various trace level.
**********************************************************************************************/

/*
#ifdef OS_USE_ALTERNATE_STD
#  undef OS_USE_ALTERNATE_STD
#  include <ospace/thread.h>
#  include <string>
#  define OS_USE_ALTERNATE_STD
#else
#  include <ospace/thread.h>
#  include <string>
#endif
*/
#include <stdlib.h>
#include "sysErrLog.h"
#include <iostream>
#include <ospace/time/time.h>

#include "BaseException.h"

#include "ConfigNames.h"
#include "EMSDefines.h"
#include <ospace/file.h>


#include "EmsUnixDefines.h"
#include <ospace/file/dir.h>

#ifdef EMSUNIX
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include "tutil.h"

using namespace ELogger;
using namespace EMS_FW;
using namespace std;
extern const char* sourceName;

string pgm;
string NameOfServer;

//////////////////////////////////////////////////////////////////
// Code added to handle signals.
//////////////////////////////////////////////////////////////////

#include <signal.h>
#include <ucontext.h>
#include <sys/frame.h>
#include <dlfcn.h>
#include <setjmp.h>
#include <sys/procfs_isa.h>
#include <stdio.h>


#if defined(sparc) || defined(__sparc)
#define FRAME_PTR_REGISTER REG_SP
#endif

#if defined(i386) || defined(__i386)
#define FRAME_PTR_REGISTER EBP
#endif

#include "version.h"

// Code to walk the stack based on an context
//Comment out all the user signal handler codes, since they're not called anyway.
/*
struct frame *
            csgetframeptr(ucontext_t *u)
{
    return (((struct frame *)
             u->uc_mcontext.gregs[FRAME_PTR_REGISTER])->fr_savfp);
}


void
cswalkstack(struct frame * fp, int (*operate_func)(void *))
{
    void * savpc;
    struct frame * savfp;

    // Starting to dump the stack
    ErrorLogger::logError("STACK", CRITICAL,
                          "Starting to dump the stack.");

    while (fp &&
           (savpc = (void*)fp->fr_savpc) &&
           (*operate_func)(savpc) == 0)
    {
        fp = fp->fr_savfp;
    }

    // Stack dump finished
    ErrorLogger::logError("STACK", CRITICAL,
                          "Finished dumping stack.");
}

static int
csprintaddress(void *pc)
{
    Dl_info info;
    char * func;
    char * lib;
    char line[512];

    if (dladdr(pc, & info) == 0)
    {
        func = (char *)"??";
        lib = (char *)"??";
    }
    else
    {
        lib = (char *) info.dli_fname;
        func = (char *) info.dli_sname;
    }

    sprintf(line,
            "%s:%s+0x%x",
            lib,
            func,
            (unsigned int)pc - (unsigned int)info.dli_saddr);

    ErrorLogger::logError("STACK", CRITICAL, line);

    return (0);
}

void
csprintstack( ucontext_t *u)
{
    cswalkstack(csgetframeptr(u), csprintaddress);
}

extern "C"
    static void sig_act_handler(int signo, siginfo_t *sip, void *uap)
{
    ErrorLogger::logError("SIGNAL CAUGHT", CRITICAL, "*************ABOUT TO CORE DUMP*********");
    switch (signo)
    {
    case SIGSEGV:
        ErrorLogger::logError("SIGNAL CAUGHT", CRITICAL, "Segmentation Violation");
        break;
    case SIGILL:
        ErrorLogger::logError("SIGNAL CAUGHT", CRITICAL, "Illigeal Instruction");
        break;
    case SIGBUS:
        ErrorLogger::logError("SIGNAL CAUGHT", CRITICAL, "Bus Error");
        break;
    case SIGFPE:
        ErrorLogger::logError("SIGNAL CAUGHT", CRITICAL, "Floating Point Exception");
        break;
    default:
        ErrorLogger::logError("SIGNAL CAUGHT", CRITICAL, "Unknown Signal");
        return ;
    }
    if (uap != 0)
    {
        csprintstack((ucontext_t *)uap);
    }
    char s[512];
    // Attempting to invoke dbx to get a prettier stack dump
    // When in production this would fail, as dbx wouldn't be installed, but the symbols
    // would be stripped out anyway...
#if 0

    if (pgm != "")
    {
        char logFileName[256];
        ErrorLogger::logVarError( "STACK", "STACK", "sig_act_handler", __LINE__, CRITICAL,
                                  "Dumping the stack trace to file %s_%d_stack.log",
                                  NameOfServer.c_str(), getpid());
        sprintf(logFileName, "%s_%d_stack.log", NameOfServer.c_str(), getpid());

        sprintf(s, "/bin/echo 'Dumping Trace' >> %s", logFileName);
        system(s);
        sprintf(s, "/bin/echo `date` >> %s", logFileName);
        system(s);

        sprintf(s, "/bin/echo 'where\ndetach' | dbx %s %d >> %s", pgm.c_str(), getpid(), logFileName);
        system(s);
    }
#endif
    abort();

}

static void install_signal_handler()
{
    // We are interested in the software signals only
    struct sigaction act;
    act.sa_handler = 0;
    act.sa_sigaction = sig_act_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    // Initialize the ErrorLogger so that if an exception occurs before we start the Error Logging in the servers
    // the stack trace goes to this file
    // When this method executes, main() hasn't been invoked
    ErrorLogger *pErr = ErrorLogger::instance();
    if (sigaction(SIGSEGV, &act, NULL) < 0)
    {
        ErrorLogger::logError("SIGNAL HANDLER", CRITICAL, "Failed to install handler for SIGSEGV");
    }
    if (sigaction(SIGBUS, &act, NULL) < 0)
    {
        ErrorLogger::logError("SIGNAL HANDLER", CRITICAL, "Failed to install handler for SIGBUS");
    }
    if (sigaction(SIGILL, &act, NULL) < 0)
    {
        ErrorLogger::logError("SIGNAL HANDLER", CRITICAL, "Failed to install handler for SIGILL");
    }
    if (sigaction(SIGFPE, &act, NULL) < 0)
    {
        ErrorLogger::logError("SIGNAL HANDLER", CRITICAL, "Failed to install handler for SIGFPE");
    }

}
*/
// We have gone thru' the initializations, let's install the signal handlers
// The idea is to to catch SIGSEGV, SIGILL, SIGBUS, SIGFPE, all the s/w signals
// The choice of adding it to Server_i is to allow all servers to get the benefit
// w/o going into each server and adding this code.
// This pragma will ensure that this handler is the first to be called.

// Commenting the pragma, since the stack trace needs to be disabled

//#pragma init(install_signal_handler)

//////////////////////////////////////////////////////////////////
// Signal code ended
//////////////////////////////////////////////////////////////////


const size_t MAX_LEN = 99999;
const err_type MAX_LEVEL = ELogger::CRITICAL;
const err_type _errMask = 0;

char* errorObjectName = (char *)"SysErrorLogger";

ErrorLogger* ErrorLogger::m_instance = 0;

int ErrorLogger::_logSize = 500;
osi_mutex ErrorLogger::_timeMutex;
osi_mutex ErrorLogger::_traceMutex;

//#ifndef OS_USE_ALTERNATE_STD
osi_file* ErrorLogger::_logFile = 0;
osi_file* ErrorLogger::_excFile = 0;
osi_file* ErrorLogger::_agentFile = 0;

bool ErrorLogger::_logToConsoleOnly = false; // if this is enabled (i.e. for emsAdmin) then we ignore the _consoleOutputEnabled flag below
bool LocalErrorLogger::_consoleOutputEnabled = false;
//#else
//ofstream* ErrorLogger::m_errFile = 0;
//#endif

//osi_otstream* ErrorLogger::_logStream=0;
err_type ErrorLogger::m_lowestSeverityLevel = ELogger::CRITICAL;
err_type ErrorLogger::_traceMask = ELogger::TRACE1 | TRACE2;
bool ErrorLogger::_traceListOn = false;
bool ErrorLogger::_checkHeap = false;
bool ErrorLogger::_excWriteOn = false;
bool ErrorLogger::_agentLogOn = false;
//added by lzou for tliaa67457
bool ErrorLogger::_errorLogOn = true; //by default, turn on the error log
//end for 67457

list<string> ErrorLogger::_traceList;
//SysError_var SysErrorLogger::m_seVar = 0;
//ComManager* SysErrorLogger::m_pCM = 0;

ErrorLogger::~ErrorLogger()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = 0;
    }
}

ErrorLogger*
ErrorLogger::instance(const char* errorLoggerLocation, err_type severityLevel)
{
    // lowest severity level to start calling logSevereError
    if (severityLevel < MINOR)
        m_lowestSeverityLevel = MINOR;
    else
        m_lowestSeverityLevel = severityLevel;

    if (!m_instance)
    {
        try
        {
            if (strcmp(errorLoggerLocation, "local") == 0)
            {
                m_instance = new LocalErrorLogger;
            }
            else
            {
                m_instance = new SysErrorLogger;
            }
        }
        catch (LoggerException)
        {
            m_instance = 0;
        }
    }

    return m_instance;
}



#define MAX_FILE_SIZE 100000000
#define AGENT_LOG_RECORD_SIZE 500


void ErrorLogger::dumpTraceList()
{}

string ErrorLogger::getCurrentThreadId()
{
    char buffer[20];
    osi_thread_t id = osi_this_thread::tid();
#if defined(WIN32)

    unsigned long ul = id.tid_;
#else

    unsigned long ul = (unsigned long) id;
#endif

    _ultoa( ul, buffer, 10 );
    return string(buffer);
}

void
ErrorLogger::logError(const char* source, err_type type, const char *data)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        m_instance->logErrorMsg(source, type, data);
    }
}

void
ErrorLogger::logError(const char* source, const char* className, err_type type, const char *data)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        string errMsg(className);
        errMsg += ": ";
        errMsg += data;
        m_instance->logErrorMsg(source, type, errMsg.c_str());
    }
}

void
ErrorLogger::logError(const char* source, const char* className, const char* methodName,
                      err_type type, const char *data)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        string errMsg(className);
        errMsg += " - ";
        errMsg += methodName;
        errMsg += ": ";
        errMsg += data;
        m_instance->logErrorMsg(source, type, errMsg.c_str());
    }
}

void
ErrorLogger::logError(const char* source, const char* className, const char* methodName,
                      int lineNumber, err_type type, const char *data)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        string errMsg(className);
        errMsg += " - ";
        errMsg += methodName;
        errMsg += " line ";
        char buf[64];
        itoa(lineNumber, buf, 10);
        errMsg += buf;
        errMsg += ": ";
        errMsg += data;
        m_instance->logErrorMsg(source, type, errMsg.c_str());
    }
}

void
ErrorLogger::logVarError(const char* source, err_type type, const char *desc, ...)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        char res[MAX_LEN];

        va_list ap;
        va_start(ap, desc);

        _vsnprintf(res, MAX_LEN - 1, desc, ap);

        va_end(ap);
        //added by lzou for MR: 79194
        res[MAX_LEN - 1] = '\0';
        //end of 79194

        m_instance->logErrorMsg(source, type, res);
    }
}

void
ErrorLogger::logVarError(const char* source, const char* className, err_type type, const char *desc, ...)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        char res[MAX_LEN];
        string errMsg(className);
        errMsg += ": ";

        va_list ap;
        va_start(ap, desc);

        _vsnprintf(res, MAX_LEN - 1, desc, ap);

        va_end(ap);
        //added by lzou for MR: 79194
        res[MAX_LEN - 1] = '\0';
        //end of 79194
        errMsg += res;
        m_instance->logErrorMsg(source, type, errMsg.c_str());
    }
}

void
ErrorLogger::logVarError(const char* source, const char* className, const char* methodName,
                         err_type type, const char *desc, ...)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        char res[MAX_LEN];
        string errMsg(className);
        errMsg += " - ";
        errMsg += methodName;
        errMsg += ": ";

        va_list ap;
        va_start(ap, desc);

        int rLen = _vsnprintf(res, MAX_LEN - 1, desc, ap);

        va_end(ap);
        //added by lzou for MR: 79194
        res[MAX_LEN - 1] = '\0';
        //end of 79194

        errMsg += res;
        m_instance->logErrorMsg(source, type, errMsg.c_str());
    }
}

void
ErrorLogger::logVarError(const char* source, const char* className, const char* methodName,
                         int lineNumber, err_type type, const char *desc, ...)
{
    if ((type & _traceMask) || type >= MINOR)
    {
        char res[MAX_LEN];

        string errMsg(className);
        errMsg += " - ";
        errMsg += methodName;
        errMsg += " line ";
        char buf[64];
        itoa(lineNumber, buf, 10);
        errMsg += buf;
        errMsg += ": ";

        va_list ap;
        va_start(ap, desc);

        _vsnprintf(res, MAX_LEN - 1, desc, ap);

        va_end(ap);
        //added by lzou for MR: 79194
        res[MAX_LEN - 1] = '\0';
        //end of 79194

        errMsg += res;
        m_instance->logErrorMsg(source, type, errMsg.c_str());
    }
}

void ErrorLogger::writeToLogFile(string header, string data)
{
    static char trailer[128];
    static int trailer_len = 0;

    if (0 == trailer_len)
    {
        sprintf(trailer, "\n*WRAP* (%s)\n\n", Version);
        trailer_len = strlen(trailer);
    }

    if (_logToConsoleOnly)
    {
        cout << header << data << endl;
        return ;
    }

    if (_logFile != 0 && _logFile->good() )
    {
        int size = _logFile->tell();
        if ((size / 1024) >= _logSize)
        {
            _logFile->resize_to(size - trailer_len + 1);
            _logFile->rewind();
        }
        else
        {
            if (size >= trailer_len - 1)
                _logFile->seek(size - trailer_len + 1, SEEK_SET);
        }

        _logFile->write(header.c_str(), header.size());
        _logFile->write(data.c_str(), data.size());
        _logFile->write(trailer, trailer_len);
    }
    //#else
    // if (m_errFile)
    //  *m_errFile << header << data << endl;
    //#endif

}
LocalErrorLogger::LocalErrorLogger() throw (LoggerException)
{
    // create ofstream for local logging
    /* try
     {
    //  string logName = Server_i::instance()->getConfigValue(ConfigNames::LocalErrorLogFileNameKey);
     
      string logName = "local_err.log";
      string excName = "exceptions.log";
      string agentName = "agent.log";
    //#ifndef OS_USE_ALTERNATE_STD
      try {
       
       string logName = "local_err.log";
       string oldFile = logName +".old";
       if (osi_file_system::exists( oldFile ) ) {
        osi_file_system::remove( oldFile );
       }
       if (osi_file_system::exists( logName ) ) {
        osi_file_system::rename( logName, oldFile );
       }
       _logFile = new osi_file(logName,osi_open_control::open_always  , O_RDWR);
    #ifdef EMSUNIX
                // Get the file descriptor and make it not inheritable on a spawn() or exec()
                int iRet = fcntl(_logFile->descriptor(), F_SETFD, FD_CLOEXEC);
                if (iRet == -1)
                {
                    // Could not prevent duplication, since the log file is not yet open, using cout
                    cout << "F_SETFD to setup FD_CLOEXEC for log file: " << logName << endl;
                }
    #endif
       if(!_logFile->good())
       {
        printf("Cannot open file %s",logName.c_str());
        delete _logFile;
        _logFile=0;
       }
       _excFile = new osi_file(excName,osi_open_control::open_always  , O_RDWR);
       if(!_excFile->good())
       {
        printf("Cannot open file %s",excName.c_str());
        delete _excFile;
        _excFile=0;
       }
       else
       { // position to the end of the file
        _excFile->seek(0,SEEK_END);
        printf("The size of the exception file is->%d\n",_excFile->tell());
     
       }
       _agentFile = new osi_file(agentName,osi_open_control::create_always  , O_RDWR);
       if(!_agentFile->good())
       {
        printf("Cannot open file %s",agentName.c_str());
        delete _agentFile;
        _agentFile=0;
       }
      }
      catch(...)
      {
       printf("Cannot open file %s or %s or %s",logName.c_str(),excName.c_str(),agentName.c_str());
       if(_logFile)
       {
        delete _logFile;
        _logFile=0;
       }
       if(_excFile)
       {
        delete _excFile;
        _excFile=0;
       }
       if(_agentFile)
       {
        delete _agentFile;
        _agentFile=0;
       }
      }
    //#else
    //  m_errFile = new ofstream(logName.c_str(), ios::app);
    //  if (m_errFile->bad()) {
    //   m_errFile->close();
    //   delete m_errFile;
    //   m_errFile=0;
    //  }
    //#endif
     }
     catch (UnresolvedNameEx& ex)
     {
      printf("Unresolved Name: %s",ex.getName().c_str());
      throw LoggerException();
     }*/
}

void LocalErrorLogger::startLogging(string logFileName, err_type mask, int size)
{
    _consoleOutputEnabled = ( (Tutil::getEnvVariableValue ("ConsoleOutputEnabled")) == "true");

    if (logFileName.empty())
    {
        _logToConsoleOnly = true;
        return ;
    }

    _logToConsoleOnly = false;
    stopLogging();
    try
    {
        if (_consoleOutputEnabled)
            printf("logFileName: %s \n", logFileName.c_str());
        ErrorLogger::instance()->enableTrace(mask);
        //added by lzou for tliaa67457
        ErrorLogger::instance()->setErrorLogFlag(true);
        //end for tliaa67457
        _logSize = size;

        //  Make sure the log file parent directory exists.
        //  If it doesn't, create it.
#if defined(WIN32)

        char logDir[MAX_PATH];
#else

        char logDir[PATH_MAX];
#endif

        strcpy( logDir, logFileName.c_str() );
        char *q = strrchr( logDir, EMSDEF::CSLASH );

        if ( q != NULL )
        {
            *q = NULL;
            try
            {
                //  Check if the directory exists.
                if (!osi_directory::exists(logDir))
                {
                    //  Parent directory does not exist.  Create it.


                    osi_directory::create(logDir);

                    //  Double check if the directory exists.
                    if (!osi_directory::exists(logDir))
                    {
                        cerr << "WARNING: Could not create directory " << logDir << endl;
                    }
                }
            }
            catch (osi_file_toolkit_error& ex)
            {
                cerr << "WARNING: Could not access directory " << logDir << " Error->" << ex.description(ex.code()) << endl;
            }
        }

        //#ifndef OS_USE_ALTERNATE_STD
        try
        {
            string oldFile = logFileName + ".old";
            try
            {
                if (osi_file_system::exists( oldFile ) )
                {
                    (osi_file_system::remove
                     ( oldFile ));
                }
                if (osi_file_system::exists( logFileName ) )
                {
                    osi_file_system::rename( logFileName, oldFile );
                }
            }
            catch (...)
            {
                // cerr << "osi_file_system::remove( oldFile ) is failed because the .log.old file is read only." << endl;

            }

            _logFile = new osi_file(logFileName, osi_open_control::open_always , O_RDWR);

            if (!_logFile->good())
            {
                cerr << "Cannot open file " << logFileName.c_str() << endl;
                if (_logFile != NULL)
                    delete _logFile;
                _logFile = NULL;

            }
        }

        catch (...)
        {

            cerr << "Cannot open file " << logFileName.c_str() << endl;
            if (_logFile != NULL)
                delete _logFile;
            _logFile = NULL;
        }

        //#else
        //  m_errFile = new ofstream(logFileName.c_str(), ios::app);
        //  if (m_errFile->bad()) {
        //   m_errFile->close();
        //   delete m_errFile;
        //   m_errFile=0;
        //  }
        //#endif
        ErrorLogger::instance()->enableTrace(mask);
        tzset();
    }
    catch (UnresolvedNameEx& ex)
    {
        if (_consoleOutputEnabled)
            printf("Unresolved Name: %s", ex.getName().c_str());
        //throw LoggerException();
    }
}

void LocalErrorLogger::stopLogging()
{
    if (_logToConsoleOnly)
        return ;

    try
    {
        //#ifndef OS_USE_ALTERNATE_STD
        if (_logFile)
        {
            _logFile->close();
            delete _logFile;
            _logFile = 0;

        }
        //#else
        //  if (m_errFile) {
        //   m_errFile->close();
        //   delete m_errFile;
        //   m_errFile = 0;
        //
        //  }
        //#endif
    }
    catch (UnresolvedNameEx& ex)
    {
        if (_consoleOutputEnabled)
            printf("Unresolved Name: %s", ex.getName().c_str());
    }
}



LocalErrorLogger::~LocalErrorLogger()
{}

#include <malloc.h>

#define BADHEAPSTART "ERROR - bad start of heap\n"
#define BADHEAPNODE  "ERROR - bad node in heap\n"

bool ErrorLogger::checkHeap()
{

#ifdef WIN32

    int heapstatus = _heapchk();
    switch ( heapstatus )
    {
    case _HEAPOK:
        if (_consoleOutputEnabled)
            printf(" OK - heap is fine\n" );
        break;
    case _HEAPEMPTY:
        if (_consoleOutputEnabled)
            printf(" OK - heap is empty\n" );
        break;
    case _HEAPBADBEGIN:
        writeToLogFile("HEAPERROR", BADHEAPSTART);
        if (_consoleOutputEnabled)
            printf( BADHEAPSTART );
        break;
    case _HEAPBADNODE:
        writeToLogFile("HEAPERROR", BADHEAPNODE);
        if (_consoleOutputEnabled)
            printf(BADHEAPNODE);
        break;
    }
    if (heapstatus == _HEAPOK)
        return true;
    return false;
#else

    return true;
#endif
}

void ErrorLogger::getTime(char* pTimeString)
{
    TimeLock lock (_timeMutex)
    ;
    struct tm datetime;
    time_t current_time;
    time(&current_time);
    localtime_r(&current_time, &datetime);
    sprintf(pTimeString, "%d/%d/%02d %d:%02d:%02d", datetime.tm_mon + 1, datetime.tm_mday,
            datetime.tm_year - 100, datetime.tm_hour, datetime.tm_min, datetime.tm_sec);
}

//==============================================================================
// logErrorMsg is one of the methods every log file message travels through on its way
// to the log file.  Since it is often called many times per second and is essentially
// overhead, this method needs to be as fast as possible.
//
void
LocalErrorLogger::logErrorMsg(const char* source, err_type type, const char *data)
{
    TimeLock lock (_guardMutex)
    ;
    char header[192];
    char* level = NULL;

    // Get message level (All same length makes file easier to read.)
    if (type < MINOR)
        level = (char *)"  trace ";
    else if (type == MINOR)
        level = (char *)"Minor   ";
    else if (type == MAJOR)
        level = (char *)"Major   ";
    else if (type == CRITICAL)
        level = (char *)"Critical";
    else
    {
        type = MINOR;
        level = (char *)"Minor   ";
    }

    // get current Time (I save some instructions by doing work here instead of getTime).
    struct tm datetime;
    time_t current_time;
    time(&current_time);
    localtime_r(&current_time, &datetime);

    // get thread Id ( again, I save instructions by doing it here.)
    osi_thread_t id = osi_this_thread::tid();
#if defined(WIN32)

    unsigned long ul = id.tid_;
#else

    unsigned long ul = (unsigned long) id;
#endif

    sprintf(header, "%s %2d/%2d/%02d %2d:%02d:%02d T=%4lu %s ", level,
            datetime.tm_mon + 1, datetime.tm_mday, datetime.tm_year - 100,
            datetime.tm_hour, datetime.tm_min, datetime.tm_sec, ul, source);

    // added for tliaa67457
    // Use the varible to turn on or off the error log. By default, it should be true.
    if (_errorLogOn)
    {
        if (type < MINOR)  // All Trace messages
        {
            // log error to file and to error console

            writeToLogFile(header, data);
            if (_consoleOutputEnabled)
                printf("%s %s\n", header, data);
        }
        else if (type < m_lowestSeverityLevel)
        {
            // print to the screen and file the error.
            writeToLogFile(header, data);
            if (_consoleOutputEnabled)
                printf("%s %s\n", header, data);
        }
        else  // Error needs special attention
            logSevereErrorMsg(source, type, header, data);

    } //end of check error log on
}

void
LocalErrorLogger::logSevereErrorMsg(const char* source, err_type type, const char* header, const char *data)
{
    // for now we will just put it into the local error file
    writeToLogFile(header, data);
    if (_consoleOutputEnabled)
        printf("%s %s\n", header, data);
}

SysErrorLogger::SysErrorLogger() throw (LoggerException)
{

}

SysErrorLogger::~SysErrorLogger()
{
    // m_pCM->terminate();
}

void
SysErrorLogger::logErrorMsg(const char* source, err_type type, const char *data)
{
    string header = source;

    // error checking on type
    if (type > MAX_LEVEL)
        type = MINOR; // ???

    if (type < MINOR)
        header += "(Trace) ";
    else if (type == MINOR)
        header += "(Minor) ";
    else if (type == MAJOR)
        header += "(Major) ";
    else
        header += "(Critical) ";

    string message;
    if (_errorLogOn)
    {

        if (type < m_lowestSeverityLevel)
        {
            message += header;
            message += data;

            //  try
            {
                //   m_seVar->logError(message.c_str());
            }
            //  catch (CORBA::SystemException& se)
            {
                //   cerr << "CORBA System Exception: " << se << endl;
            }
        }
        else // Error needs special attention
            logSevereErrorMsg(source, type, header.c_str(), data);
    }
}

void
SysErrorLogger::logSevereErrorMsg(const char* source, err_type type, const char* header, const char *data)
{
    string message = header;
    message += data;

    // try
    {
        //  m_seVar->logSevereError(message.c_str());
    }
    // catch (CORBA::SystemException& se)
    {
        //  cerr << "CORBA System Exception: " << se << endl;
    }
}


void
ErrorLogger::enforceInstance(ErrorLogger* pInstance)
{
    if (!m_instance)
        m_instance = pInstance;

    return ;
}




/************************************************************************************
*
* Name        : maskPassword
*
* Description : This method masks the password which is displayed in the log files. The password will be
*  masked as "*". The possible cases for masking are in the following TL1 commands data recvd at the
*  socket:
*  ENT-USER-SECU:TELLABS7100:TEST:AM0121::*******:UAP=A4,TMOUT=30,PAGE=60,PUWP=50,DBCHGMSG=INH;
*  ED-USER-SECU:TELLABS7100:Smith22:AM0105::******:UAP=A4,TMOUT=30,PAGE=60,PUWP=50,DBCHGMSG=INH,EVTMSG=INH,PMMSG=INH,STATUS=ENABLED;
*  EID-PID:TELLABS7100:ADMIN:CP0100::MYPSWD$1,MYPSWD$2;
*
* Arguments   : A constant string reference which holds the data.
*  A non constant string reference which has the password masked.
*
* Returns     : void
*
/************************************************************************************/
void ErrorLogger::maskPassword(const string& msg, string& duplicateMessage)
{
    int doubleColon = msg.find("::", 0);

    if (msg.find("/**/") != string::npos) //This will be found only in RTRV-LOG response and pwd is already masked by NE
    {
        duplicateMessage = msg;
    }
    else if (string::npos != doubleColon)
    {
        if (msg.find("act-user") != string::npos || msg.find("ACT-USER") != string::npos)
        {
            int commaPosition = msg.find (",", doubleColon + 2);
            if (commaPosition != string::npos)
            {
                if (commaPosition > (doubleColon - 2))
                {
                    string maskPswd ((commaPosition - (doubleColon + 2)), '*');
                    string encryptionFlag;
                    if ( (commaPosition + 1) < msg.size())
                        encryptionFlag = msg.substr (commaPosition + 1, 1);
                    duplicateMessage = msg.substr(0, doubleColon) + "::" + maskPswd + "," + encryptionFlag + ";";
                }
            }

            int semiColon = msg.find(";", doubleColon + 2) ;

            if (string::npos == semiColon)
            {
                duplicateMessage = msg;
            }

            string maskPswd ((semiColon - (doubleColon + 2)), '*');
            duplicateMessage = msg.substr(0, doubleColon) + "::" + maskPswd + ";";
        }
        else if (msg.find("ENT-USER-SECU") != string::npos)
        {
            int singleColon = msg.find(":", doubleColon + 2) ;

            if (string::npos == singleColon)
            {
                duplicateMessage = msg;
            }

            string maskPswd ((singleColon - (doubleColon + 2)), '*');
            duplicateMessage = msg.substr(0, doubleColon + 2) + maskPswd + msg.substr(singleColon, msg.length());
        }
        else if (msg.find("ED-USER-SECU") != string::npos)
        {
            int semiColon = (msg.length() - (doubleColon + 2)) - 1;

            if (string::npos == semiColon)
            {
                duplicateMessage = msg;
            }

            string maskPswd(semiColon, '*');
            duplicateMessage = msg.substr(0, doubleColon + 2) + maskPswd + ";";
        }
        else if (msg.find("ED-PID") != string::npos)
        {
            int comma = msg.find(",", doubleColon);

            if (string::npos == comma)
            {
                duplicateMessage = msg;
            }

            string firstMask (((comma - doubleColon) - 2), '*');
            string secondMask (((msg.length() - comma) - 2), '*');

            duplicateMessage = msg.substr(0, doubleColon + 2) + firstMask + "," + secondMask + ";";
        }
    }

    // Can't just look for 'ftp://' because TL1 preprocessing removes the colons
    // We need to match 'ftp.//' (where . is any character)
    //
    int ftp_pos = msg.find("ftp");
    while ((ftp_pos != string::npos) && (msg.substr(ftp_pos + 4, 2) != "//"))
    {
        ftp_pos = msg.find("ftp", ftp_pos + 3);
    }

    if (msg.find("copy-rfile") != string::npos ||
        msg.find("COPY-RFILE") != string::npos ||
        ftp_pos != string::npos)    // if yes, hide the password
    {
        int begin = ftp_pos;
        if (begin != string::npos)
        {
            begin = msg.find_first_of(" :", begin + 6);
            if (begin != string::npos)
            {
                begin += 1;

                int end = msg.find("@", begin);
                if (end != string::npos)
                {
                    duplicateMessage = msg;
                    duplicateMessage.replace (begin, end - begin, "*");
                }
            }
        }
    }
    else
    {
        duplicateMessage = msg;
        return ;
    }
}
