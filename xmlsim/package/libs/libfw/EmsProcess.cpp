
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:44  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: EmsProcess
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EmsProcess.cpp  $
* Version: $Revision:   1.2  $
* Description: 
* Encapsulates the process management API
* 
* Revision History:
*
*
**********************************************************************************************/



#include <procfs.h>

#include "EmsProcess.h"
#include "EmsUnixDefines.h"
#include "sysErrLog.h"
#include "BaseException.h"
#include "ConfigNames.h"
#include "EMSDefines.h" 
//#include "AmSrv.h"

#include <errno.h>

extern const char* sourceName;
static const char* className = (const char *)"EmsProcess";

using namespace EMS_FW;
using namespace ELogger;

#if defined(WIN32)


#define CREATE_ENV_WAIT    20000  // Wait time for the Proxy Env to be started.

EmsProcess::EmsProcess()
{
    m_proxyHandler = NULL;
    _pid = 0;
}

EmsProcess::~EmsProcess()
{
    if (m_proxyHandler)
    {  // release the handle to the process
        CloseHandle(m_proxyHandler);
        m_proxyHandler = NULL;
    }
}

unsigned int EmsProcess::getProcessId() const
{
    return _pid;
}


bool EmsProcess::createProcess(const string& execDir, const string& cmd, const string& args, bool visible, bool waitToFinish)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "EmsProcess", "createProcess", __LINE__, TRACE3, "Method start for cmd =%s args = %s", cmd.c_str(), args.c_str());

    STARTUPINFO sui = {0};
    PROCESS_INFORMATION procInfo;
    SECURITY_ATTRIBUTES saprocess, saThread;

    sui.cb = sizeof(sui);

    if (!visible)
    {
        sui.wShowWindow = SW_HIDE;
    }

    BOOL flag = FALSE;
    if (waitToFinish)
        flag = TRUE;
    saprocess.nLength = sizeof(saprocess);
    saprocess.lpSecurityDescriptor = NULL;
    saprocess.bInheritHandle = flag;

    saThread.nLength = sizeof(saThread);
    saThread.lpSecurityDescriptor = NULL;
    saThread.bInheritHandle = flag;

    string cmdExec = cmd + " " + args;

    const char* pExecDir = NULL;
    if (execDir.size() > 0)
        pExecDir = execDir.c_str();

    if (waitToFinish)
    {
        if (CreateProcess(NULL, &cmdExec[0], &saprocess, &saThread, TRUE, 0, NULL, pExecDir, &sui , &procInfo))
        {
            if (WaitForSingleObject(procInfo.hProcess, CREATE_ENV_WAIT) == WAIT_OBJECT_0) // To wait for the Proxy Env to be started.
            {
                CloseHandle(procInfo.hProcess);
                CloseHandle(procInfo.hThread);
                ErrorLogger::logError("AM", "EmsProcess", "CreateProcess", TRACE3, "Process executed.");
                return true;
            }
        }
    }
    else
    {
        if (CreateProcess(NULL, &cmdExec[0], &saprocess, &saThread, TRUE, CREATE_NEW_CONSOLE, NULL, pExecDir, &sui, &procInfo))
        {
            m_proxyHandler = procInfo.hProcess;
            _pid = procInfo.dwProcessId;
            ErrorLogger::logError("AM", "EmsProcess", "CreateProcess", TRACE3, "Process Created.");
            return true;
        }
    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "EmsProcess", "createProcess", __LINE__, TRACE3, "Fail to create the process.");
    return false;
}




bool EmsProcess::terminateProcess()
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "EmsProcess", "terminateProcess", __LINE__, TRACE3, "Method started.");
    if (m_proxyHandler != NULL)
    {
        TerminateProcess(m_proxyHandler, 1);
        CloseHandle(m_proxyHandler);
        m_proxyHandler = NULL;
    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logError(sourceName, "EmsProcess", "terminateProcess", __LINE__, TRACE3, "Method finished.");
    return true;
}

bool EmsProcess::setEnvorinmentVariable(const string& name, const string& value)
{
    if (SetEnvironmentVariable(name.c_str(), value.c_str()) == 0)
        return false;
    return true;
}

bool EmsProcess::getEnvironmentVariable(const string& name, string& value)
{
    char tmp[256];
    if (GetEnvironmentVariable(name.c_str(), tmp, sizeof(tmp)) == 0)
        return false;
    value = tmp;
    return true;
}


bool EmsProcess::getProcessTime(string& time) const
{
    if (m_proxyHandler == NULL)
        return false; // no open process exists
    char Time[128];
    FILETIME ftCreate, ftExit, ftKernel, ftUser;
    SYSTEMTIME sysTime;

    if (GetProcessTimes(m_proxyHandler, &ftCreate, &ftExit, &ftKernel, &ftUser))
    {
        if (FileTimeToSystemTime(&ftCreate, &sysTime))
        {
            _snprintf(&Time[0], 127, "%04d-%02d-%02d-%02d:%02d:%02d:%03d", sysTime.wYear, sysTime.wMonth,
                      sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
            return true;
        }
    }

    ErrorLogger::logVarError("AM", "ProxyMgr", "GetProcTime", CRITICAL,
                             "FAILED TO READ THE PROCESS TIME error = %d", GetLastError());
    return false;
}

bool EmsProcess::isProcessRunning(unsigned int pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE , FALSE, pid);
    if (hProcess == NULL)
        return false;
    CloseHandle(hProcess);
    return true;
}


bool EmsProcess::connectToProcess(unsigned int pid)
{
    m_proxyHandler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE , FALSE, pid);
    if (m_proxyHandler == NULL)
        return false;
    return true;
}

EmsProcess* EmsProcess::getProcess(unsigned int pid)
{
    EmsProcess* proc = new EmsProcess();
    if (proc->connectToProcess(pid))
        return proc;
    delete proc;
    return NULL;
}


#else

// UNIX implementation


#include <ospace/unix/thisproc.h>
#include <ospace/unix/unix_ex.h>
#include  <stdio.h>
#include  <fcntl.h>             /* file control option      */
#include  <unistd.h>            /* symbolic constants       */
#include  <sys/procfs.h>      /* process file system info   */


EmsProcess::EmsProcess( bool use_fork1 ) : _fork_alone( use_fork1 )
{
    _process = NULL;
}

EmsProcess::~EmsProcess()
{
    const char *methodName = (const char *)"~EmsProcess";
    if (_process)
    {
        try
        {
            _process->terminate();
            delete _process;
            _process = NULL;
        }
        catch (osi_unix_toolkit_error& err)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                     "Error terminating process=%s", err.description(err.code()));
        }
    }
}



// Create process and block waiting the process to finish if waitToFinish = true.
// If visible is true the process run in its own console otherwise is runing as a daemon
//bool EmsProcess::createProcess(const string& execDir,const string& cmd, const string& args, bool visible, bool waitToFinish,
//osi_unnamed_pipe* sendPipe, osi_unnamed_pipe* receivePipe)
bool EmsProcess::createProcess(const string& execDir, const string& cmd, const string& args, bool visible, bool waitToFinish,
                               const string& startConsoleCmd, int writeHandle)
{

#define MAX_ARGV_SIZE 50


    const char *methodName = (const char *)"createProcess";
    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                             "createProcess started!");
    osi_pid_t myPid;
    osi_pid_t parentPid;
    string execCmd = execDir + cmd;

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                             "Entering method; execDir:%s cmd:%s args:%s visible:%d waitToFinish:%d",
                             execDir.c_str(), cmd.c_str(), args.c_str(), visible, waitToFinish);

    if ( !startConsoleCmd.empty() )  // start the process in a separate console
    {
        string tempCmd = startConsoleCmd;
        tempCmd.append(" -e ");
        tempCmd.append(execCmd);
        tempCmd.append(args);
        execCmd = tempCmd;
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                                 "Real cmd; execCmd:%s", execCmd.c_str());
    }

    // Parse args into an array of pointers to the individual arguments (because
    // that is the format that the osi_process constructor wants).
    char *argv[MAX_ARGV_SIZE];
    char* argStr = 0;
    argv[0] = strdup(execCmd.c_str());
    int argc = 1;
    if ( args.size() == 0 )
    {
        // There is nothing in args, initialize argv and argc to only contain the exe name.
        argc = 1;
    }
    else
    {
        // Copy args to a C-Style character string.
        argStr = strdup( args.c_str() );

        // Get the first token (argument) using a space as a delimiter.
        char *psLast = NULL;
        char *tmpStr = strtok_r( argStr, " ", &psLast );
        while ( tmpStr != NULL )
        {
            // Save this argument into argv.
            argv[ argc ] = tmpStr;
            ++argc;
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE3,
                                     "set argv[%d] to '%s'.", argc - 1, argv[argc - 1]);

            // Get the next argument, picking up where we left off in argStr.
            tmpStr = strtok_r( NULL, " ", &psLast );
        }
    }
    argv [ argc ] = NULL;

    bool retValue = false;
    try
    {
        const char *cmdStr = execCmd.c_str();

        // Save our PID as the parent (we are the parent because we
        // are around before the new process is created).
        parentPid = osi_this_process::pid();
        myPid = parentPid;
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE3,
                                 "Before osi_process() call, parentPid:%lu, cmdStr:%s argv[0]:%s",
                                 parentPid, cmdStr, argv[0]);

        // Spawn the new process with args, if provided.
        pid_t childPid;
        //ErrorLogger::logError("Common", "EmsProcess", "CreateProcess", TRACE3, "Start Process in the same window");
        if ( _fork_alone )
        {
            // Create a copy of this executable (only the current
            // thread).
            childPid = osi_this_process::fork_alone();
        }
        else
        {
            // Create a copy of this executable (with all the
            // threads).
            childPid = osi_this_process::fork();
        }

        // Create an osi_process wrapper for this new thread.
        _process = new osi_process( childPid );

        // Both parent and child will get to here.  Use parentPid
        // to determine if we are the parent or the child.
        myPid = osi_this_process::pid();
        if ( myPid == parentPid )
        {
            // This is the parent copy of the fork.
            free(argStr);
            free(argv[0]);
            ErrorLogger::logVarError(sourceName, className,
                                     methodName, __LINE__, TRACE3,
                                     "Parent made it through the fork, childPid = %lu.",
                                     _process->pid() );
        }
        else
        {
            // This the the child copy, figure out which exec to do
            // based on whehter we have command-line arguments,
            // whether we have a pipe to the child, etc.

            if ( !startConsoleCmd.empty() )
            {
                char * shellEnv = getenv("SHELL");
                osi_this_process::execute( shellEnv, cmdStr );
            }
            else
            {
                if ( argc > 0 )
                {
                    if ( writeHandle != NULL )
                    {
                        osi_this_process::execute( osi_nil_env, osi_nil_desc,
                                                   writeHandle, osi_nil_desc, cmdStr,
                                                   (const char **)argv );
                        ErrorLogger::logError("Common", "EmsProcess", "CreateProcess", TRACE3, "End of osi_process");
                    }
                    else
                        osi_this_process::execute( cmdStr, (const char **)argv );
                }
                else
                {
                    // No arguments provided, spawn the command without arguments.
                    if ( writeHandle != NULL )
                    {
                        osi_this_process::execute( osi_nil_env, osi_nil_desc,
                                                   writeHandle, osi_nil_desc, cmdStr );
                    }
                    else
                        osi_this_process::execute( cmdStr );
                }
            }

            // It should never get to here.
            ErrorLogger::logVarError(sourceName, className,
                                     methodName, __LINE__, CRITICAL,
                                     "This process is the result of a failed exec. Exiting");
            exit(1);
        }


        myPid = osi_this_process::pid();
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE3,
                                 "After new osi_process(%s) call my pid=%lu, parent pid=%lu, _process->pid=%lu.",
                                 cmdStr, myPid, parentPid, _process->pid());

        // pause for two seconds, then check to see if the process is still
        // running.
        //   int sleepMilliSecs = 2000;
        //   osi_this_thread::milli_sleep(sleepMilliSecs);
        osi_process_status childStat = osi_this_process::wait_for_child( _process->pid(),
                                       WNOHANG);
        if ( childStat.defined() == true )
        {
            // If the childStat is defined, then the child has terminated.  We will consider
            // this as the child did not properly launch.
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__,
                                     MAJOR, "Process appears to have aborted; command:%s pid:%lu.",
                                     cmdStr, _process->pid() );
            retValue = false; // so that the caller knows the process failed.
        }
        else
        {
            if (waitToFinish)
                osi_this_process::wait_for_child(_process->pid());
            retValue = true;
        }
    }
    catch (osi_unix_toolkit_error& err)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Could not create process  Error code [%d]  Error description [%s]  Error symbol [%s]. this PID [%lu]  parent PID [%lu]  I am [%s]",
                                 err.native(), err.description(err.native()), err.symbol(err.native()), osi_this_process::pid(), parentPid,
                                 (myPid == parentPid ? "the parent" : "the child"));
        if ( myPid != parentPid )
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, CRITICAL,
                                     "This process is the result of a failed exec within osi_proces. Exiting");
            exit(1);
        }
    }
    return retValue;
}

bool EmsProcess::terminateProcess()
{
    const char *methodName = (const char *)"terminateProcess";
    bool result = false;
    if (_process)
    { // terminate the process
        try
        {
            _process->terminate();
            // Give OS 2 Sec to cleanup the process.
            osi_this_thread::sleep(2);
            // If Terminate does not terminate the process successfully,
            // then as a last resort , kill it.
            if (_process->valid())
            {

                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                                         "Could not Terminate Process, Had to KILL it");

                _process->kill();
            }
            delete _process;
            _process = NULL;
            result = true;
        }
        catch (osi_unix_toolkit_error& err)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                     "Error terminating process=%s", err.description(err.code()));
        }
    }
    else
        ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR, "Trying to terminate the process that does not exists");
    return true;
}

bool EmsProcess::setEnvorinmentVariable(const string& name, const string& value)
{
    osi_environment env = osi_this_process::environment();
    env[ name.c_str() ] = value.c_str();
    osi_this_process::environment( env ); // Change environment.
    return true;
}

bool EmsProcess::getEnvironmentVariable(const string& name, string& value)
{
    osi_environment env = osi_this_process::environment();
    osi_environment::iterator p = env.find(name);
    if (p == env.end()) // environment is not set
        return false;
    value = (*p).second;
    return true;
}


bool EmsProcess::connectToProcess(unsigned int pid)
{
    _process = new osi_process( pid ); // Associate object with existing PID.
    if (_process->valid())
        return true;
    delete _process;
    _process = NULL;
    return false;
}


// return false if process is not started yet
bool EmsProcess::getProcessTimeByPID(unsigned int pid, string& time)
{
    bool retValue = false;

    const char *methodName = (const char *)"getProcessTime(static)";

    char sFilename[24];
    int ifd;
    psinfo_t psInfo; /* ps(1) process information    */

    sprintf(sFilename, "/proc/%d/psinfo", pid);
    ifd = open(sFilename, O_RDONLY);
    if (ifd < 0)
    { // unable to open the process file
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__,
                                 MAJOR, "Unable to open %s, errno:%d", sFilename, errno);
    }
    else
    {
        if ( read(ifd, &psInfo, sizeof(psInfo)) < sizeof(psInfo) )
        { // unable to get the information
            ErrorLogger::logVarError(sourceName, className, methodName,
                                     __LINE__, MAJOR,
                                     "Unable to get the process structure from %s; errno:%d",
                                     sFilename, errno);
        }
        else
        {
            char buf[36];
            sprintf(buf, "%ld", psInfo.pr_start.tv_sec); // get the seconds till the process start
            time = buf;
            close(ifd);
            retValue = true;
        }
    }
    return retValue;
}


// return false if process is not started yet
bool EmsProcess::getProcessTime(string& time) const
{
    bool retValue = false;

    const char *methodName = (const char *)"getProcessTime";

    if (_process == NULL)
        return false;

    unsigned int pid = _process->pid();
    retValue = EmsProcess::getProcessTimeByPID(pid, time);
    return retValue;
}

bool EmsProcess::isProcessRunning(unsigned int pid)
{
    osi_process process(pid);
    if (process.valid())
        return true;
    return false;
}

EmsProcess* EmsProcess::getProcess(unsigned int pid)
{
    EmsProcess* proc = new EmsProcess( false );
    if (proc->connectToProcess(pid))
        return proc;
    delete proc;
    return NULL;
}

unsigned int EmsProcess::getProcessId() const
{
    if (_process)
        return (unsigned int)_process->pid();
    return 0; // invalid ID
}

/*static*/ void* EmsProcess::_systemThreadFunc(void* arg)
{
    _SystemThreadArgType* pThreadArg = (_SystemThreadArgType*) arg;

    *(pThreadArg->_procStatus) = osi_this_process::system(pThreadArg->_command);
    return 0;
}

/*static*/ bool EmsProcess::system(const string& cmd,     bool inSeparateThread)
{
    bool retVal = false;
    const char* methodName = "system(static)";
    osi_process_status procStatus;

    if (false == inSeparateThread)
    {
        procStatus = osi_this_process::system(cmd.c_str());
        retVal = (procStatus.exit_code() == 0) ? true : false;
    }
    else
    {

        osi_thread* pSystemThread = (osi_thread*)0;
        _SystemThreadArgType* pThreadArg = (_SystemThreadArgType*)0;

        try
        {

            ErrorLogger::logVarError(sourceName, className, methodName, TRACE1, "Launching system(\"%s\") in separate thread.",
                                     cmd.c_str());

            pThreadArg = new _SystemThreadArgType(cmd.c_str(), &procStatus);
            pSystemThread = new osi_thread(_systemThreadFunc, (void*)pThreadArg);

            osi_this_thread::milli_sleep(5*1000);

            int nAttemptsLeft = 5;
            while (nAttemptsLeft-- && pSystemThread->exists())
            {
                ErrorLogger::logVarError(sourceName, className, methodName, MINOR, "Waiting for system() to complete... "
                                         "Retries left %d", nAttemptsLeft);
                osi_this_thread::milli_sleep(30*1000);
            }

            if (nAttemptsLeft < 0)
            {
                // Thread seems to be hung. kill it
                ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR, "system() thread not responding. Terminating...");
                pSystemThread->terminate();
                retVal = false;
            }
            else
            {
                retVal = (procStatus.exit_code() == 0) ? true : false;
            }

            delete pSystemThread;
            pSystemThread = 0;
            delete pThreadArg;
            pThreadArg = 0;
        }
        catch ( osi_thread_toolkit_error& err )
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                     "Error detected by thread toolkit '%s'", err.description(err.code()));

            delete pSystemThread;
            pSystemThread = 0;
            delete pThreadArg;
            pThreadArg = 0;
            retVal = false;
        }
    }

    return retVal;
}

#endif
