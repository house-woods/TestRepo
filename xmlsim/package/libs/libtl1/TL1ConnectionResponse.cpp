
#include <ospace/thread.h>
#include <ospace/helper.h>
#include <ospace/time.h>
#include <ospace/time/timedate.h>
#include <ospace/time/tperiod.h>

#include "TL1ConnectionResponse.h"

#include "BaseException.h"
#include "CmNameResolver.h"
#include "CmServer.h"
#include "ComManager.h"
#include "ComManager.h"
#include "ConfigNames.h"
#include "Cryptor.h"
#include "EMSDefines.h"
#include "NEAlarmControl.hh"
#include "NodeDefines.h"
#include "Node.hh"
#include "NodeI.hh"
#include "NsgErrorDef.h"
#include "Server_i.h"
#include "SocketHandler.h"
#include "sysErrLog.h"
#include "TL1AccountManager.h"
#include "TL1CmdInfo.h"
#include "TL1ConnectionCountSemaphore.h"
#include "TL1ConnectionManager.h"
#include "TL1Defines.h"
#include "TL1EdPidCmd.h"
#include "TL1ErrCodes.h"
#include "TL1EventReceiver.h"
#include "TL1GenericCmd.h"
#include "TL1Lexer.h"
#include "TL1Logger.h"
#include "TL1LoginCmd.h"
#include "TL1LogoutCmd.h"
#include "TL1Parser.h"
#include "TL1RtrvHdrCmd.h"
#include "tutil.h"
extern const char* sourceName;

using namespace TL1_MA;
using namespace EMS_FW;
using namespace ELogger;
using namespace SysMon;
using namespace EMS;


osi_mutex TL1Session::_neIdToSessionMapMutex;
osi_mutex TL1Session::_tidToSessionMapMutex;
osi_mutex TL1Session::_ctagToSessionMapMutex;

map <string, TL1Session *> TL1Session::_neIdToSessionMap;
map <string, TL1Session *> TL1Session::_tidToSessionMap;
map <string, TL1Session *> TL1Session::_ctagToSessionMap;

static const int EVTRATE_SECONDS_PER_SLOT = 10;
static const int EVTRATE_TOTAL_OBSERVED_SECONDS = 600;
static const int EVTRATE_NUM_SLOTS = EVTRATE_TOTAL_OBSERVED_SECONDS / EVTRATE_SECONDS_PER_SLOT;

static const int EVTRATE_MIN_RATE_FOR_UNBLOCK = 1; // Per-second

TL1Session::TL1Session()
{
    // Should never be used
}

TL1Session::TL1Session (const string& neId, const string& tid,
                        const string& rtrvCmd, const string& retry,
                        const string& retrieveHeaderInterval,
                        const string& hostname, unsigned portNumber,
                        const string& logDirectory,
                        const string& tidIndex,
                        int numberOfConnections,
                        bool meteringEnabled,
                        int delayPeriodForBlock,
                        int eventPercentageForUnblock,
                        unsigned long maxLogSize ,
                        bool wrapAround,
                        int maxNumberOfFiles,
                        bool AMSUHCSimulatorSupport,
                        bool deleteOnExit,
                        const string& conTimeout,
                        const string& conRetry) :
        _locHandler(NULL),
        _accountManager(NULL),
        _loggedIn(false),
        _logDirectory(logDirectory),
        _maxLogSize(maxLogSize),
        _wrapAround(wrapAround),
        _maxNumberOfFiles(maxNumberOfFiles),
        _deleteOnExit(deleteOnExit),
        _neId(neId),
        _retrieveHeaderInterval(atoi(retrieveHeaderInterval.c_str())),
        _rtrvCmd(rtrvCmd),
        _tid(tid),
        _keepSessionThread(NULL),
        _delayMultiplier(0),
        _connectionSemaphore(new TL1ConnectionCountSemaphore (1, numberOfConnections)),
        _shuttingDown(false),
        _logger(NULL),
        _meteringEnabled(meteringEnabled),
        _delayPeriodForBlock(delayPeriodForBlock),
        _eventPercentageForUnblock(eventPercentageForUnblock),
        _eventCount(new int[EVTRATE_NUM_SLOTS]),
        _currEventSlot(0),
        _shiftTime(0),
        _queueIsBackedUp(false),
        _eventsAreBlocked(false),
        _numberOfSlotsPerDelayPeriod(_delayPeriodForBlock / EVTRATE_SECONDS_PER_SLOT),
        _eventCountWhenBlocked(0),
        _rtrvHdrFailCnt(0),
        _rtrvHdrFailThreshold(0)
{
    memset(_eventCount, 0, EVTRATE_NUM_SLOTS * sizeof(int));

    _messageTypeNames[0] = "NONE";
    _messageTypeNames[1] = "LOG";
    _messageTypeNames[2] = "ACK";
    _messageTypeNames[3] = "RSP";
    _messageTypeNames[4] = "AUT";
    _messageTypeNames[5] = "PARSE ERROR";
    _messageTypeNames[6] = "IGNORE";
    _messageTypeNames[7] = "SYSTEM";
    _messageTypeNames[8] = "TEST COMMAND";
    _messageTypeNames[9] = "ERROR";
    _messageTypeNames[10] = "TIMEOUT";
    _messageTypeNames[11] = "REJECTED";
    _messageTypeNames[12] = "END OF MESSAGE";

    _eventHandlerTypes[0] = "FM";
    _eventHandlerTypes[1] = "CM";
    _eventHandlerTypes[2] = "NBI";
    _encryptType = -1;
    _failLogInCnt = 0;
}

TL1Session::~TL1Session()
{
    delete _connectionSemaphore;
    delete [] _eventCount;
}

bool TL1Session::initialize()
{
    _shutdown = false;
    _waitOnIP = Server_i::instance()->getDefaultConfigValue("TL1WaitOnIP", "false");

    string loginRetryWaitTimeString = Server_i::instance()->getDefaultConfigValue("TL1LoginRetryWaitTime", "10");
    _loginRetryWaitTime = atoi (loginRetryWaitTimeString.c_str());
    //default timer is 5 mins when login failure count is bigger than _loginRetryThreshold(default 5).
    string loginRetryWaitTimeStringWhenFailed = Server_i::instance()->getDefaultConfigValue("TL1LoginRetryWaitTimeWhenFailed", "300");
    _loginRetryWaitTimeWhenFailed = atoi(loginRetryWaitTimeStringWhenFailed.c_str());
    //Default TL1 login retry threshold is 5.
    string loginRetryThreshold = Server_i::instance()->getDefaultConfigValue("TL1LoginRetryThreshold", "5");
    _loginRetryThreshold = atoi(loginRetryThreshold.c_str());

    const string& neType = Server_i::instance()->getDefaultConfigValue (ConfigNames::NeTypeKey, _neId, "OTS");
    if (neType == "NGX") {
        string rtrvHdrFailThreshold = Server_i::instance()->getDefaultConfigValue("TL1RtrvHdrFailThreshold", "2");
        _rtrvHdrFailThreshold = atoi(rtrvHdrFailThreshold.c_str());
        if (_rtrvHdrFailThreshold > 5 || _rtrvHdrFailThreshold < 1) {
            _rtrvHdrFailThreshold = 2;
        }

        ErrorLogger::logVarError( sourceName, "TL1Session", "initialize", TRACE1, "Read TL1RtrvHdrFailThreshold for %s, %s, %d", _tid.c_str(), rtrvHdrFailThreshold.c_str(), _rtrvHdrFailThreshold);
    }  

    ErrorLogger::logVarError(sourceName, "TL1Session", "initialize", TRACE3, "Login re-try wait time is %d seconds", _loginRetryWaitTime);
    ErrorLogger::logVarError(sourceName, "TL1Session", "initialize", TRACE3, "Wait on IP is set to %s", _waitOnIP.c_str());
    registerSessionByTID (_tid, this);
    registerSessionByNeId (_neId, this);
    if (_keepSessionThread == NULL && (!_neId.empty()))
    {
        try
        {
            _keepSessionThread = new osi_thread(TL1Session::_processKeepSessionThread, (void*) this, 0, false);
        }
        catch (osi_thread_toolkit_error& err)
        {
            ErrorLogger::logVarError(sourceName, "TL1Session", "initialize", __LINE__, ELogger::CRITICAL, "Could not start the keep-session thread; %s", err.description(err.code()));
            return false;
        }
    }

    return true;
}

bool TL1Session::finalize()
{
    // unregister the recovery handler so LOC is not falsely reported
    unregisterHandler ();
    if (_keepSessionThread != NULL)
    {
        _shutdown = true;
        TL1ConnectionManager::instance()->dropActiveConnection(this);
        _connectionSemaphore->post();
        _eventNotify.post();
        osi_this_thread::wait_for_thread(*_keepSessionThread);
        delete _keepSessionThread;
        _keepSessionThread = NULL;
    }
    _shuttingDown = true;
    TL1Session::unregisterSessionByTID (_tid);
    TL1Session::unregisterSessionByNeId (_neId);
    TL1Session::unregisterAllSessionToCTAGMapping (this);
    unregisterEventReceivers();
    stopLogging(_tid);
    TL1ConnectionManager::instance()->removeAllAssociations(this);
    resetFailLogInCnt();
    return true;
}

//-----------------------------------------------------------------------------
// Notifications to Sysmon and software alarms
//-----------------------------------------------------------------------------

void TL1Session::depositMessage(TL1LocRecoverHandler::MessageType messageType, ELogger::ErrorCode messageToSGM, const string& additionalInfo)
{
    if (!_neId.empty()) //_neId is empty only in case if cmd comes through default proxy
    {
        TL1LocRecoverHandler::SendCmd cmd;
        cmd.messageType = messageType;
        cmd.neId = _neId;
        cmd.tid = _tid;
        cmd.protocol = "GENERIC_PROTOCOL"; // This should be TL1, but the protocol stuff is all screwed up.
        cmd.code = messageToSGM;
        cmd.data = additionalInfo;
        osi_sem_write_lock lock (rwSemaphore)
        ;
        if (_locHandler)
            _locHandler->SendMessage (cmd, true);
    }
}

void TL1Session::depositMessage(TL1LocRecoverHandler::MessageType messageType, const string& swAlarmDescription, const string& additionalSource)
{
    if (!_neId.empty()) //_neId is empty only in case if cmd comes through default proxy
    {
        TL1LocRecoverHandler::SendCmd cmd;
        cmd.messageType = messageType;
        cmd.neId = _neId;
        cmd.tid = _tid + additionalSource;
        cmd.code = 0;
        cmd.data = swAlarmDescription;
        osi_sem_write_lock lock (rwSemaphore)
        ;
        if (_locHandler)
            _locHandler->SendMessage (cmd, true);
    }
}

//-----------------------------------------------------------------------------
// Accessors and Mutators for critical states
//-----------------------------------------------------------------------------
int TL1Session::getAdnSetKeepWaiting (int newValue)
{
    osi_sem_read_lock lock (delaymultSemaphore)
    ;
    bool oldValue = _delayMultiplier;
    if (newValue != -1)
        _delayMultiplier = newValue;
    return (oldValue);
}

bool TL1Session::setLoginState (bool newState, const string &conName, const string& completionCode, const string &errorCode)
{
    string stateString = "false";
    if (newState)
        stateString = "true";
    try
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "setLoginState", TRACE1,
                                 "Setting login state to %s for %s", stateString.c_str(), _tid.c_str());
        osi_sem_write_lock lock (rwSemaphore)
        ;
        bool oldState = _loggedIn;
        _loggedIn = newState;
        if ((oldState == true) && (newState == false))
        {
            _reportedFailures.clear();
            string message = "Login session";
            if (conName.empty() == false)
            {
                message.append (" via ");
                message.append (conName);
            }
            message.append (" closed with NE ");
            message.append (_tid);
            abortPendingCommands();
            depositMessage (TL1LocRecoverHandler::MESSAGE_NOTIFYSGM, CommFailureWithAgent);
        }
        else
            if (oldState == false)
            {
                if (newState == true)
                {
                    string message = "Login session";
                    if (conName.empty() == false)
                    {
                        message.append (" via ");
                        message.append (conName);
                    }
                    message.append (" established with NE ");
                    message.append (_tid);

                    depositMessage (TL1LocRecoverHandler::MESSAGE_CLEARSWALARM, message);
                    if (completionCode != "AddNetworkElement")
                    {
                        depositMessage (TL1LocRecoverHandler::MESSAGE_NOTIFYSGM, ResyncNetworkElement);
                    }
                }
                if (newState == false)
                {
                    string fullErrorCode = completionCode + " " + errorCode;
                    if (fullErrorCode.empty() == false)
                    {
                        if ((fullErrorCode.find ("Timed Out") != -1) || (fullErrorCode.find ("timed out")) != -1)
                            fullErrorCode = "Command timed out";
                        if ((fullErrorCode.find ("Failed to send command") != -1))
                            fullErrorCode = "Connection closed while sending login command";
                        unsigned long failCnt = getFailLogInCnt();
                        if (failCnt >= _loginRetryThreshold)
                        {
                            fullErrorCode = "Failed login exceeds threshold due to timed out";
                        }

                        string errorCodeKey = fullErrorCode + conName;
                        if (_reportedFailures.find (errorCodeKey) == _reportedFailures.end())
                        {
                            string message = "Could not establish a session to " + _tid;
                            if (failCnt >= _loginRetryThreshold)
                            {
                                message = "Could not establish a session repeatedly to " + _tid;
                            }
                            if (conName.empty() == false)
                            {
                                message.append (" via ");
                                message.append (conName);
                            }
                            message.append (" - ");
                            message.append (fullErrorCode);
                            _reportedFailures.insert (pair<string, string>(errorCodeKey, errorCodeKey));
                            depositMessage (TL1LocRecoverHandler::MESSAGE_GENERATESWALARM, message);
                        }
                    }
                }
            }
        ErrorLogger::logVarError(sourceName, "TL1Session", "setLoginState", TRACE1,
                                 "Set login state to %s for %s successfully", stateString.c_str(), _tid.c_str());
    }
    catch (osi_thread_toolkit_error& ex)
    {
        //string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "setLoginState", __LINE__, MINOR, "Thread Exception caught on getRtrvHdrRetryFromInfo->%s", ex.description(ex.code()));
        //throw BaseException(desc, "TL1Connection", "getRtrvHdrRetryFromInfo");
        return false;
    }
    return true;
}

bool TL1Session::getLoginState()
{
    bool flag;
    try
    {
        osi_sem_read_lock lock (rwSemaphore)
        ;
        flag = _loggedIn;
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getLoginState", __LINE__, MINOR, "Thread Exception caught on logout->%s", ex.description(ex.code()));
        throw BaseException(desc, "TL1Connection", "getLoginState");
    }
    return flag;
}


//-----------------------------------------------------------------------------
// LOC/Recovery loop and call-backs
//-----------------------------------------------------------------------------
void* TL1Session::_processKeepSessionThread(void* arg)
{
    TL1Session *thisCR = (TL1Session *) arg;
    string tid = thisCR->gettid();
    ErrorLogger::logVarError( sourceName, "TL1Session", "_processKeepSessionThread", TRACE1, "Starting the keep-session loop for %s", tid.c_str());
    thisCR->keepSessionFunction ();
    ErrorLogger::logVarError( sourceName, "TL1Session", "_processKeepSessionThread", TRACE1, "keep-session loop has exited for %s", tid.c_str());
    return ((void*) 0);
}

void TL1Session::keepSessionFunction (void)
{
    TL1ConnectionManager *manager = TL1ConnectionManager::instance();
    _accountManager->nextAccount (_tid);
    const string& neType = Server_i::instance()->getDefaultConfigValue (ConfigNames::NeTypeKey, _neId, "OTS");
    // get a user account before we can log in
    while (!_shutdown)
    {
        bool loggedIn = getLoginState();
        if (loggedIn)
        {
            unsigned long failCnt = getFailLogInCnt();
            if (failCnt != 0)
            {
                //current state is login, so reset fail count to 0.
                resetFailLogInCnt();
            }
            string currentConenctionName;
            TL1Connection *currentConnection = manager->getConnectionInUse (this, currentConenctionName);
            CmdResult res;
            bool rtrvhdrSucceeded = retrieveHeader (res, currentConnection);
            do
            {
                if (rtrvhdrSucceeded)
                {
                    _rtrvHdrFailCnt = 0;
                    if (_connectionLossNotify.wait(_retrieveHeaderInterval) == false) //wait for socket loss or timer expiry;false=timer expiry
                    {
                        CmdResult result;
                        logout (result, currentConnection);
                        setLoginState (false, currentConenctionName);
                        getAdnSetKeepWaiting(0); // Clear the IP flag
                    }
                }
                else // retrieve header failed
                {
                    ++_rtrvHdrFailCnt;
                    ErrorLogger::logVarError( sourceName, "TL1Session", "keepSessionFunction", TRACE1, "RTRV-HDR Failed, %s, %s, FailCnt=%d, Threshold=%d", _tid.c_str(), neType.c_str(), _rtrvHdrFailCnt, _rtrvHdrFailThreshold);

                    if (neType != "NGX" || _rtrvHdrFailCnt >= _rtrvHdrFailThreshold)
                    {

                    if (getAdnSetKeepWaiting () == 0) //check if the keep waiting flag was not set
                    {
                        string message = "Retrieve header";
                        if (currentConenctionName.empty() == false)
                        {
                            message.append (" via ");
                            message.append (currentConenctionName);
                        }
                        message.append (" failed on NE ");
                        message.append (_tid);
                        message.append ("; " + res.getResultInfo() + ":" + res.getStrErrCode());
                        depositMessage (TL1LocRecoverHandler::MESSAGE_GENERATESWALARM, message);
                        CmdResult result;
                        logout (result, currentConnection);
                        setLoginState (false, currentConenctionName);
                    }

                    }   //if (neType != "NGX" || ++_rtrvHdrFailCnt >= _rtrvHdrFailThreshold)
                    else
                    {
                        if (neType == "NGX") {  // wait a rtrv-hdr interval and retry.
                            ErrorLogger::logVarError( sourceName, "TL1Session", "keepSessionFunction", TRACE1, "RTRV-HDR Failed, %s, %s, wait %d", _tid.c_str(), neType.c_str(), _retrieveHeaderInterval);
                            if (_connectionLossNotify.wait(_retrieveHeaderInterval) == false) {//wait for socket loss or timer expiry
                                CmdResult result;
                                logout (result, currentConnection);
                                setLoginState (false, currentConenctionName);
                                getAdnSetKeepWaiting(0); // Clear the IP flag
                            }
                            ErrorLogger::logVarError( sourceName, "TL1Session", "keepSessionFunction", TRACE1, "RTRV-HDR Failed, %s, %s, waited %d", _tid.c_str(), neType.c_str(), _retrieveHeaderInterval);
                        }
                    }
                }
            }
            while (getAdnSetKeepWaiting (0)); // if keepwaiting was set, reset it before looping
        }
        else // not logged in
        {
            ErrorLogger::logVarError( sourceName, "TL1Session", "keepSessionFunction", TRACE1, "No active session to %s exists", _tid.c_str());
            string currentConnectionName;
            TL1Connection *currentConnection = manager->getConnectionInUse (this, currentConnectionName);
            // try to get next active connection...if none is available, it will return the current
            string activeConnectionName;
            TL1Connection *activeConnection = manager->getStandbyConnection (this, currentConnection, activeConnectionName);
            // can be NULL if we came here after _connectionSemaphore->wait
            if (currentConnection)
            {
                manager->setAssociationState (this, currentConnection, false);
            }
            string mode = Server_i::instance()->getDefaultConfigValue("NEALWAYSALIVEMODE", "NO");
            if (activeConnection)
            {
                manager->setAssociationState (this, activeConnection, true);
                CmdResult result;
                if (login (result, activeConnection) == false)
                {
                    string completionCode = result.getResultInfo();
                    string errorCode = result.getStrErrCode();
                    if ((completionCode.find ("Timed Out") != string::npos) || (completionCode.find ("timed out")) != string::npos)
                    {
                        //login failed due to timed out, increase failure count by 1.
                        increaseFailLogInCnt();
                    }
                    else if (getFailLogInCnt() > 0)
                    {
                        //count the failure only if it is continuous timed out.
                        resetFailLogInCnt();
                    }
                    logout (result, activeConnection);
                    setLoginState (false, activeConnectionName, completionCode, errorCode);
                    //int waitTime = 10000; // in milliseconds
                    ErrorLogger::logVarError(sourceName, "TL1Session", "keepSessionFunction", __LINE__, TRACE1, "Waiting for %d seconds before attempting to login into %s", _loginRetryWaitTime, _tid.c_str());
                    // don't pound on connections..let the NE breathe a little
                    //ACE_OS::sleep(waitTime/1000);
                    // ACE_OS::sleep takes time in seconds
                    unsigned long failCnt = getFailLogInCnt();
                    if (failCnt >= _loginRetryThreshold)
                    {
                        //if failure count is larger than _loginRetryThreshold(default 5), change sleep time to 5 mins
                        //in case socket is ok, but no response for command, it will block all the other sessions in the same SG.
                        ErrorLogger::logVarError(sourceName, "TL1Session", "keepSessionFunction", __LINE__, TRACE2, "Waiting for %d seconds before attempting to login into %s when failure>%d.", _loginRetryWaitTimeWhenFailed, _tid.c_str(), _loginRetryThreshold);
                        //EmsLock::wait timeout in milli-seconds.
                        if (false == _eventNotify.wait(_loginRetryWaitTimeWhenFailed*1000)) //return false if notified.
                        {
                            if (_shutdown)
                            {
                                //exit the thread if the TL1Session is shutdown.
                                ErrorLogger::logVarError(sourceName, "TL1Session", "keepSessionFunction", __LINE__, TRACE2, "TL1 session of %s was shutdown.", _tid.c_str());
                            }
                            else
                            {
                                //TL1 connection recover, retry login.
                                ErrorLogger::logVarError(sourceName, "TL1Session", "keepSessionFunction", __LINE__, TRACE2, "TL1 connection of %s recover, retry login.", _tid.c_str());
                            }
                        }
                        resetFailLogInCnt();
                    }
                    else
                    {
                        ACE_OS::sleep(_loginRetryWaitTime);
                    }
                    _accountManager->nextAccount (_tid);
                }
                else
                {    
                    setLoginState (true, activeConnectionName);
                    resetFailLogInCnt();
                }
            }
            else if (0 == mode.compare("YES"))
            {
                manager->setAssociationState (this, activeConnection, true);
                setLoginState (true, activeConnectionName);
            }
            else
            {
                if (_connectionSemaphore->posted())
                {
                    ErrorLogger::logVarError(sourceName, "TL1Session", "keepSessionFunction", __LINE__, TRACE1, "counting semaphore is in posted state for %s; connection loss has not reset it yet... wait for it to be reset ", _tid.c_str());
                    try
                    {
                        osi_this_thread::milli_sleep(100);
                    }
                    catch (...)
                    {
                        ACE_OS::sleep(1);
                    }
                }
                else
                {
                    string message = "No active connections available for " + _tid;
                    message.append (" - Waiting for one to recover");
                    _connectionSemaphore->wait();
                }
            }
        }
    } // end of main while loop
}



void TL1Session::connectionRecovery (const string &conName)
{
    // this will increment the number of connection resources available
    // if the minimum number of resoures are available a thread waiting for connection recovery will wake up
    int numOfUpConnections = _connectionSemaphore->post();
    string message = "Recovered socket connection to host at " + conName;
    ErrorLogger::logVarError(sourceName, "TL1Session", "connectionRecovery", __LINE__, TRACE1, "Recovered connection to host %s for the NE %s", conName.c_str(), _tid.c_str());
    //  string source = ":"+conName;

    depositMessage (TL1LocRecoverHandler::MESSAGE_CLEARSWALARM, message);
    //Notify keepSessionFunction thread and clean _reportedFailures since sw alarms are all cleared by MESSAGE_CLEARSWALARM 
    _eventNotify.post();
    _reportedFailures.clear();
    // This will update the available connections config in DB for this NE and also send out a notification
    //
    depositMessage(TL1LocRecoverHandler::MESSAGE_SEND_NODE_UPDATE, message);
}

void TL1Session::ignoreConnectionLoss () // change to session closed
{
    // the connection manager will call this to make sure that
    // if an association has already been disabled using this connection, all connection loss
    // notifications should be ignored
    _connectionLossNotify.reset();
}

void TL1Session::connectionLoss (const string &conName) // change to session closed
{
    // this will decrement the number of connection resources available
    int numOfUpConnections = _connectionSemaphore->reset();
    string message = "Lost socket connection to host at " + conName;
    ErrorLogger::logVarError(sourceName, "TL1Session", "connectionRecovery", __LINE__, TRACE1, "Lost connection to host %s for the NE %s, NE now has %d up connections", conName.c_str(), _tid.c_str(), numOfUpConnections);

    //string source = ":"+conName;
    depositMessage (TL1LocRecoverHandler::MESSAGE_GENERATESWALARM, message);

    //
    // Need to send a notification against the NE here to update the Available Connections icons on the GUI
    //
    depositMessage(TL1LocRecoverHandler::MESSAGE_SEND_NODE_UPDATE, message);
}

void TL1Session::closeSession () // change to session closed
{
    // this will notify that the connection in use has been lost
    ErrorLogger::logVarError(sourceName, "TL1Session", "closeSession", __LINE__, TRACE1, "Started for %s", _tid.c_str());
    if (getLoginState() == true)
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "closeSession", __LINE__, TRACE1, "login status is true for %s posting _connectionLossNotify", _tid.c_str());
        _connectionLossNotify.post();
    }
    else
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "closeSession", __LINE__, TRACE1, "login status false for %s; NOT.posting _connectionLossNotify", _tid.c_str());
    }
}

//-----------------------------------------------------------------------------
// Force switch call-back
//-----------------------------------------------------------------------------
void TL1Session::forceSwitchConnections (void)
{
    ErrorLogger::logVarError(sourceName, "TL1Session", "forceSwitchConnections", __LINE__, TRACE1, "Force switch requested for %s...dropping current connection", _tid.c_str());

    string currentConnectionName, activeConnectionName;
    TL1Connection *currentConnection = TL1ConnectionManager::instance()->getConnectionInUse (this, currentConnectionName);
    TL1Connection *activeConnection = TL1ConnectionManager::instance()->getStandbyConnection (this, currentConnection, activeConnectionName);

    // if we get an active connection, see if
    if (activeConnection)
    {
        // if the active connection is the same as current then we have no backup connections
        if (activeConnection == currentConnection)
        {
            ErrorLogger::logVarError(sourceName, "TL1Session", "forceSwitchConnections", __LINE__, TRACE1, "Could not switch [%s] to use a different connection; standby connection is the same as current connection", _tid.c_str());
            throw BaseException ("No active standby connections are available to switch to");
        }
    }
    else
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "forceSwitchConnections", __LINE__, TRACE1, "Could not switch [%s] to a different connection; no active connections are available", _tid.c_str());
        throw BaseException ("No active connections are available to switch to");
    }

    string message = "Force switch requested for ";
    message.append (_tid);
    message.append (" - Attempting to switch to an alternate connection");
    depositMessage (TL1LocRecoverHandler::MESSAGE_GENERATESWALARM, message);
    TL1ConnectionManager::instance()->dropActiveConnection (this);
}

//-----------------------------------------------------------------------------
// Event Receiver registration functions
//-----------------------------------------------------------------------------
void TL1Session::registerEvenetReceiver (const string &type, TL1EventReceiver* receiver)
{
    AutoUnLock lock (_eventReceiversMutex)
    ;
    _eventReceivers.insert (pair<string, TL1EventReceiver*>(type, receiver));
}

bool TL1Session::forwardEventToReceiver (const string &type, TL1Message* event, const char *rawMessage)
{
    bool bRetVal = true;
    AutoUnLock lock (_eventReceiversMutex)
    ;
    map <string, TL1EventReceiver*>::iterator i = _eventReceivers.find (type);
    if (i != _eventReceivers.end())
    {
        TL1EventReceiver* eventReceiver = (*i).second;
        bRetVal = eventReceiver->handleEvent (event, rawMessage);
    }

    return bRetVal;
}

void TL1Session::unregisterEventReceivers (void)
{
    AutoUnLock lock (_eventReceiversMutex)
    ;
    _eventReceivers.erase (_eventReceivers.begin(), _eventReceivers.end());
}

//-----------------------------------------------------------------------------
// Commands Used by the Proxy Server
//-----------------------------------------------------------------------------
bool TL1Session::login(CmdResult& result, TL1Connection *connectionToUse, bool changeExpiredPassword)
{
    // setresult-info sets the completion code
    string mode = Server_i::instance()->getDefaultConfigValue("NEALWAYSALIVEMODE", "NO");
    string username, password;
    if (_accountManager)
    {
        username = _accountManager->getUsername();
        password = _accountManager->getPassword();
    }
    if ((username.empty()) || (password.empty()) )
    {
        username = _uid;
        password = _password;
    }

    if (connectionToUse == NULL)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONDOWN");
        result.setStrErrCode ("No connection available");
        return false;
    }

    if (connectionToUse->isActive() == false)
    {
        if (0==mode.compare("YES"))
        {
            result.setStrErrCode ("Data received; command assumed to have succeeded");
            result.setResultInfo (TL1DEF::RES_COMPLETECODE_COMP);
            result.setStatus (true);
            return true;
        }
        else
        {
            result.setStatus(false);
            result.setResultInfo ("CONNECTIONDOWN");
            string message = "Current connection to " + connectionToUse->getName() + " active";
            result.setStrErrCode (message);
            return false;
        }
    }

    if (username.empty() || password.empty())
    {
        result.setStatus(false);
        result.setResultInfo("USERNAMEPASSWORDMISSING");
        result.setStrErrCode ("Username and/or Password invalid");
        return false;
    }

    TL1LoginCmd lcmd(connectionToUse, _tid, username, password, _neId, _encryptType);
    if (!lcmd.initialize())
    {
        result.setStatus(false);
        result.setResultInfo("COMMANDINITFAILED");
        result.setStrErrCode ("Login command not initialized");
        return false;
    }

    string preCmd = "ACT-USER:" + _tid + ":" + username + ":";
    string postCmd = "::*********,*;";

    result = lcmd.execute();

    string completionCode = result.getResultInfo();
    string errrorCode = result.getStrErrCode();
    if (completionCode == TL1DEF::RES_COMPLETECODE_COMP)
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "login", TRACE1,
                                 "Login to NE %s succeeded  Completion Code = %s,  Error Code = %s",
                                 _tid.c_str(), completionCode.c_str(), errrorCode.c_str());
        if ((errrorCode.find("Your password has expired") != osi_npos) && (changeExpiredPassword == true))
        {
            ErrorLogger::logVarError(sourceName, "TL1Session", "login", __LINE__, TRACE1, "BUT Password has expired. Editing password");
            CmdResult res;
            if (!editPasswd (res, connectionToUse, username, password))
            {
                ErrorLogger::logVarError(sourceName, "TL1Session", "login", __LINE__, MAJOR, "COULD NOT EDIT PASSWORD. SENDING LOGOUT COMMAND");
                result = res;    // return result of editPasswd
                return false;
            }
            else
                return true;
        }
        else
            return true;
    }
    else
    {
        string duplicateMessage ;
        ErrorLogger::maskPassword(completionCode, duplicateMessage);
        ErrorLogger::logVarError(sourceName, "TL1Session", "login", MAJOR,
                                 "Login to NE %s FAILED Completion Code = %s,  Error Code = %s",
                                 _tid.c_str(), duplicateMessage.c_str(), errrorCode.c_str());

        return false;
    }
}

bool TL1Session::logout(CmdResult& result, TL1Connection *connectionToUse)
{
    string username, password;
    if (_accountManager)
    {
        username = _accountManager->getUsername();
        password = _accountManager->getPassword();
    }
    if ((username.empty()) || (password.empty()) )
    {
        username = _uid;
        password = _password;
    }

    if (connectionToUse == NULL)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONDOWN");
        result.setStrErrCode ("No connection available");
        return false;
    }

    if (connectionToUse->isActive() == false)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONINACTIVE");
        string message = "Current connection to " + connectionToUse->getName() + " active";
        result.setStrErrCode (message);
        return false;
    }

    if (username.empty() || password.empty())
    {
        result.setStatus(false);
        result.setResultInfo("USERNAMEPASSWORDMISSING");
        result.setStrErrCode ("Username and/or Password invalid");
        return false;
    }

    TL1LogoutCmd lcmd(connectionToUse, _tid, username, _neId);
    if (!lcmd.initialize())
    {
        result.setStatus(false);
        result.setResultInfo("COMMANDINITFAILED");
        result.setStrErrCode ("Logout command not initialized");
        return false;
    }

    string preCmd = "CANC-USER:" + _tid + ":" + username + ":";
    string postCmd = ";";

    result = lcmd.execute();

    string completionCode = result.getResultInfo();
    string errrorCode = result.getStrErrCode();
    if (completionCode == TL1DEF::RES_COMPLETECODE_COMP)
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "logout", TRACE1,
                                 "Logout from NE %s succeeded  Completion Code = %s,  Error Code = %s",
                                 _tid.c_str(), completionCode.c_str(), errrorCode.c_str());
        return true;
    }
    else
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "logout", MAJOR,
                                 "Logout from NE %s FAILED  Completion Code = %s,  Error Code = %s",
                                 _tid.c_str(), completionCode.c_str(), errrorCode.c_str());
        return false;
    }
}

bool TL1Session::editPasswd(CmdResult& result, TL1Connection *connectionToUse, const string &username, const string &password)
{
    if (connectionToUse == NULL)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONDOWN");
        result.setStrErrCode ("No connection available");
        return false;
    }

    if (connectionToUse->isActive() == false)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONINACTIVE");
        string message = "Current connection to " + connectionToUse->getName() + " active";
        result.setStrErrCode (message);
        return false;
    }

    if ( (username.empty()) || (password.empty()) )
    {
        result.setStatus(false);
        result.setResultInfo("USERNAMEPASSWORDMISSING");
        result.setStrErrCode ("Username and/or Password invalid");
        return false;
    }

    string new_password = _generateNewPasswd();

    if (_encryptType>0 || _password.length() > 30)
    {
        if (_encryptType >1)
        {
           new_password = Tutil::encryptRijndaelExWithPadding(new_password);
           _encryptType = 2;
        }
        else
        {
           new_password = Cryptor::encrypt(new_password, Cryptor::md5Method);
           _encryptType = 1;
        }
    }

    TL1EdPidCmd epcmd(connectionToUse, _tid, username, password, new_password, _neId, _encryptType);
    if (!epcmd.initialize())
    {
        result.setStatus(false);
        result.setResultInfo("COMMANDINITFAILED");
        result.setStrErrCode ("Edit-password command not initialized");
        return false;
    }

    string preCmd = "ed-pid:" + _tid + ":" + username + ":";
    string postCmd = "::*********,*********;";

    result = epcmd.execute();

    string completionCode = result.getResultInfo();
    string errrorCode = result.getStrErrCode();
    if (completionCode == TL1DEF::RES_COMPLETECODE_COMP)
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "editPasswd", TRACE1,
                                 "Edit Password on NE %s succeeded  Completion Code = %s,  Error Code = %s",
                                 _tid.c_str(), completionCode.c_str(), errrorCode.c_str());
        // if the username used is the same as that used to add the NE
        // notify SysMon and update the cached user-defined account
        if (username == _uid)
        {
            _password = new_password;
            ErrorLogger::logVarError(sourceName, "TL1Session", "editPasswd", TRACE1,
                                     "Notifying SysMon about password change for account [%s] for NE [%s] (which is currently in use)", username.c_str(), _tid.c_str());
            depositMessage (TL1LocRecoverHandler::MESSAGE_NOTIFYSGM, NEPasswordChange, new_password);
            // update cached password
            ErrorLogger::logVarError(sourceName, "TL1Session", "editPasswd", TRACE1,
                                     "Updating cached password for account [%s] for NE [%s] (which is currently in use)", username.c_str(), _tid.c_str());
            _accountManager->setUserDefinedAccount (username, new_password);
        }
        // attempt to update the persisted account if one exists
        _accountManager->updatePassword (_tid, username, new_password);
        return true;
    }
    else
    {
        string duplicateMessage ;
        ErrorLogger::maskPassword(completionCode, duplicateMessage);
        ErrorLogger::logVarError(sourceName, "TL1Session", "editPasswd", TRACE1,
                                 "Edit Password on NE %s FAILED  Completion Code = %s,  Error Code = %s",
                                 _tid.c_str(), duplicateMessage.c_str(), errrorCode.c_str());

        return false;
    }
}

string TL1Session::_generateNewPasswd()
{
    string newPassword;
    char specialChars[] = "!@$";
    try
    {
        osi_time_and_date currTimeDate = osi_time_and_date::now();
        osi_date currDate = currTimeDate.date();
        osi_time currTime = currTimeDate.time();
        newPassword += (char) ('A' + (currDate.day_of_year() % 26));
        newPassword += (char) ('x' - (currTime.minutes() % 20));
        newPassword += (char) ('9' - currDate.weekday());
        newPassword += (char) ('b' + (currTime.seconds() % 23));
        short weekNum = currDate.week_of_year();
        if (weekNum < 26)
            newPassword += (char) ('Z' - weekNum);
        else if (weekNum < 52)
            newPassword += (char) ('a' + (weekNum - 26));
        else
            newPassword += (char) ('9' - (weekNum - 52));
        newPassword += (char) ('Z' - currTime.hours());
        newPassword += (char) ('9' - (currDate.year() % 10));
        newPassword.insert((size_t) (currDate.year() % 7), 1, specialChars[currTime.seconds() % 3]);
    }
    catch (osi_time_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "_generateNewPasswd",
                                 TRACE1, "Caught time_toolkit exception: %s",
                                 ex.description(ex.code()));
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, "TL1Session", "_generateNewPasswd",
                              TRACE1, "Caught unknown exception");
    }
    return newPassword;
}

bool TL1Session::retrieveHeader (CmdResult& result, TL1Connection *connectionToUse)
{
    // this can be set to false for simulators
    const string &retrieveHeaderEnabled = Server_i::instance()->getDefaultConfigValue ("RetrieveHeaderEnabled", "true");
    if (retrieveHeaderEnabled == "false")
        return true;
    string mode = Server_i::instance()->getDefaultConfigValue("NEALWAYSALIVEMODE", "NO");
    if (0==mode.compare("YES"))
    {
        return true;
    }

    const string &maxAgeOfLastMessageInSeconds = Server_i::instance()->getDefaultConfigValue ("MaxAgeOfLastMessageInSeconds", "50");

    // the last message should have been received within the maximum age allowed
    long secondsSinceLastMessage = getSecondsSinceLastMessageReceived ();
    ErrorLogger::logVarError(sourceName, "TL1Session", "retrieveHeader", TRACE3,
                             "Determining if retrieve header is to be sent for NE [%s]; last response or IP arrived [%ld] seconds ago",
                             _tid.c_str(), secondsSinceLastMessage);

    if (secondsSinceLastMessage != -1)
    {
        if (secondsSinceLastMessage <= (atol (maxAgeOfLastMessageInSeconds.c_str())) )
        {
            ErrorLogger::logVarError(sourceName, "TL1Session", "retrieveHeader", TRACE1,
                                     "Skipping retrieve header as last response or IP for [%s] arrived [%ld] seconds ago",
                                     _tid.c_str(), secondsSinceLastMessage);
            return true;
        }
    }


    if (connectionToUse == NULL)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONDOWN");
        result.setStrErrCode ("No connection available");
        return false;
    }

    if (connectionToUse->isActive() == false)
    {
        result.setStatus(false);
        result.setResultInfo ("CONNECTIONINACTIVE");
        string message = "Current connection to " + connectionToUse->getName() + " active";
        result.setStrErrCode (message);
        return false;
    }

    TL1RtrvHdrCmd rtrvCmd(connectionToUse, _tid, _rtrvCmd, _neId);
    if (!rtrvCmd.initialize())
    {
        result.setStatus(false);
        result.setResultInfo("COMMANDINITFAILED");
        result.setStrErrCode ("Retrieve header command not initialized");
        return false;
    }

    result = rtrvCmd.execute();

    string completionCode = result.getResultInfo();
    string errrorCode = result.getStrErrCode();

    if (completionCode == TL1DEF::RES_COMPLETECODE_COMP)
        return true;
    else
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "retrieveHeader", MAJOR,
                                 "Retrieve header failed on NE [%s]  Completion Code [%s]  Error Code [%s]",
                                 _tid.c_str(), completionCode.c_str(), errrorCode.c_str());
        return false;
    }
}

//-----------------------------------------------------------------------------
// Logging routines
//-----------------------------------------------------------------------------
void TL1Session::startLogging (const string &tid)
{
    AutoUnLock lock (_loggerMutex)
    ;
    if (!_logger)
    {
        string _hostname;
        _logger = new TL1Logger (_logDirectory, _hostname, tid, _maxLogSize, _wrapAround, _maxNumberOfFiles, _deleteOnExit);
    }
}

void TL1Session::stopLogging (const string &tid)
{
    AutoUnLock lock (_loggerMutex)
    ;
    if (_logger)
    {
        delete _logger;
        _logger = NULL;
    }
}

void TL1Session::logTraffic(string tid, string stringMessage, const string& messageType, const string& conName)
{
    ErrorLogger::logError( sourceName, "TL1Session", "logTraffic", TRACE7, "Method starts");

    if (messageType == "CMD")
    {
        // for command tid should be blank so
        // paree tid from command whoes format is <command>:tid: ...
        int start = stringMessage.find (":");
        if (start != osi_npos)
        {
            int end = stringMessage.find (":", start + 1);
            if ((end != osi_npos) && ( end > start - 2))
                tid = stringMessage.substr(start + 1, end - start - 1);
        }

        if (stringMessage.find("act-user") != osi_npos ||
            stringMessage.find("ACT-USER") != osi_npos)    // if yes, hide the password
        {
            // get the position of the 5th colon
            int positionOflast = 0;
            for (int i = 0; i < 5; i++)
            {
                if (positionOflast != osi_npos)
                {
                    positionOflast = stringMessage.find(":", positionOflast + 1);
                }
            }
            // find the ; after the fifth colon
            if (positionOflast != osi_npos)
            {
                int end = stringMessage.find(";", positionOflast + 1);
                if ((end != osi_npos) && ( end > positionOflast - 2))
                {
                    stringMessage.replace (positionOflast + 1, end - positionOflast - 1, "**********");
                }
            }
        }
        else if (stringMessage.find("copy-rfile") != osi_npos ||
                 stringMessage.find("COPY-RFILE") != osi_npos)    // if yes, hide the password
        {
            int begin = stringMessage.find("ftp://");
            if (begin != string::npos)
            {
                begin = stringMessage.find(":", begin + 6);
                if (begin != string::npos)
                {
                    begin += 1;

                    int end = stringMessage.find("@", begin);
                    if (end != string::npos)
                    {
                        stringMessage.replace (begin, end - begin, "**********");
                    }
                }
            }
        }
    }

    if (!tid.empty() && !messageType.empty())
    {
        if (tid == _tid)
        {
            // get the logger based on the tid
            AutoUnLock lock(_loggerMutex);
            if (_logger)
            {
                _logger->logMessage(messageType, stringMessage, conName);
            }
            else
            {
                ErrorLogger::logVarError(sourceName, "TL1Session", "logTraffic", MAJOR, "Could not find logger for [%s]; message cannot be logged; NE possibly removed already", tid.c_str());
            }
        }
        else
            ErrorLogger::logVarError( sourceName, "TL1Session", "logTraffic", MAJOR, "Message is intended for [%s]; it cannot be logged for [%s]", tid.c_str(), _tid.c_str());
    }
    ErrorLogger::logError( sourceName, "TL1Session", "logTraffic", TRACE7, "Method ends");
}

void TL1Session::setTimeLastMessageReceived ()
{
    AutoUnLock timeLock(_timeLastMessageReceivedMutex);
    try
    {
        _timeLastMessageReceived = osi_time_and_date::now();
    }
    catch (...)
        {}
}

long TL1Session::getSecondsSinceLastMessageReceived()
{
    AutoUnLock timeLock(_timeLastMessageReceivedMutex);
    try
    {
        return (osi_time_and_date::now() - _timeLastMessageReceived).to_seconds();
    }
    catch (...)
    {
        return -1;
    }
}

//-----------------------------------------------------------------------------
// Session level data processing and transmission functions
//-----------------------------------------------------------------------------
void TL1Session::processMessage(const char* rawMessage, TL1Message *msg, TL1Connection *con, const string& connectionName)
{
    ErrorLogger::logError( sourceName, "TL1Session", "processMessage", TRACE11, "Method starts");

    if (_shuttingDown)
    {
        ErrorLogger::logVarError( sourceName, "TL1Session", "processMessage", TRACE1, "Ignoring following message as the session to NE [%s] is shutting down (NE being locked) \n    %s\n", _tid.c_str(), rawMessage);
        return ;
    }

    // delay retrieve header loop for time-consuming commands
    if (msg->m_code == "IP")
        getAdnSetKeepWaiting (1);

    if ( (msg->m_type < 0) || (msg->m_type >= 13) )
    {
        ErrorLogger::logVarError( sourceName, "TL1Session", "processMessage", MINOR, "Received the following message of an out-of-range type [%d] from the NE [%s] from [%s]\n    %s\n", msg->m_type, _tid.c_str(), connectionName.c_str(), rawMessage);
        return ;
    }

    const string &messageTypeString = _messageTypeNames[msg->m_type];
    logTraffic(_tid, rawMessage, messageTypeString, connectionName);

    if (ErrorLogger::isErrTypeEnabled(TRACE4))
    {
        string duplicateMessage;
        ErrorLogger::maskPassword(rawMessage, duplicateMessage);
        ErrorLogger::logVarError( sourceName, "TL1Session", "processMessage", TRACE4, "Received the following %s for the NE [%s] from [%s]\n    %s\n", messageTypeString.c_str(), _tid.c_str(), connectionName.c_str(), duplicateMessage.c_str());
    }

    if ((msg->m_type == TL1Message::Response) || (msg->m_type == TL1Message::Acknowledgement))
    {
        setTimeLastMessageReceived ();
        const string &dropSessionOnPLNA = Server_i::instance()->getDefaultConfigValue ("DropSessionOnPLNA", "false");
        if (dropSessionOnPLNA == "true")
        {
            // look for DENY, Login not active
            if (msg->m_completeCode == "DENY")
            {
                // PLNA = login not active i.e. session was dropped
                if (strstr (rawMessage, "PLNA") != NULL)
                {
                    ErrorLogger::logVarError( sourceName, "TL1Session", "processMessage", MINOR, "Received response contains [PLNA] for NE [%s]; TL1 appears to have been dropped abruptly by GNE/RNE; Initiating recovery process by dropping local session from EMS", _tid.c_str());
                    string message = "TL1 Session was abruptly dropped for ";
                    message.append (_tid);
                    message.append (" (PLNA was received) - Initiating recovery process by dropping local EMS session");
                    depositMessage (TL1LocRecoverHandler::MESSAGE_GENERATESWALARM, message);
                    TL1ConnectionManager::instance()->dropActiveConnection (this);
                    return ;
                }
            }
        }
        handleResponse (msg);
    }
    else
        if (msg->m_type == TL1Message::Autonomous)
            handleEvent (msg, rawMessage);
        else
            ErrorLogger::logVarError( sourceName, "TL1Session", "processMessage", TRACE1, "Message is of unexpected type [%s]", messageTypeString.c_str());

    ErrorLogger::logError( sourceName, "TL1Session", "processMessage", TRACE11, "Method ends");
}

bool TL1Session::sendCommand (const char *commandText, const string& ctag, TL1BaseCmd *command)
{
    ErrorLogger::logError( sourceName, "TL1Session", "sendCommand", TRACE11, "Method starts");

    if (_shuttingDown)
    {
        ErrorLogger::logVarError(sourceName, "TL1Session", "forwardCommandToSession", TRACE1, "Not forwarding command [%s] as the session to NE [%s] is shutting down (NE being locked)", commandText, _tid.c_str());
        return false;
    }

    // Outgoing traffic is now logged from TL1ConnectionManager::instance()->sendDataToConnection
    // logTraffic("", commandText, "CMD");

    if (ErrorLogger::isErrTypeEnabled(TRACE4))
    {
        string duplicateMessage ;
        ErrorLogger::maskPassword(commandText, duplicateMessage);
        ErrorLogger::logVarError(sourceName, "TL1Session", "send_data", TRACE4,
                                 "Sending the following command:\n\n%s\n", duplicateMessage.c_str());
    }

    TL1Session::registerSessionByCTAG (ctag, this);
    registerCommand (ctag, command);
    TL1ConnectionManager::instance()->sendDataToConnection (this, commandText);

    ErrorLogger::logError( sourceName, "TL1Session", "sendCommand", TRACE11, "Method ends");

    return true;
}

void TL1Session::registerHandler (TL1LocRecoverHandler *handler)
{
    osi_sem_write_lock lock (rwSemaphore)
    ;
    _locHandler = handler;
}

void TL1Session::unregisterHandler (void)
{
    osi_sem_write_lock lock (rwSemaphore)
    ;
    _locHandler = NULL;
}

void TL1Session::registerAccountManager (TL1AccountManager *manager)
{
    _accountManager = manager;
}

void TL1Session::registerCommand (const string &ctag, TL1BaseCmd* command)
{
    AutoUnLock lock (_ctagToCommandMapMutex)
    ;

    if (command->isUniversalListener())
        _ctagToUniversllListenersMap.insert ( map<string, TL1BaseCmd*>::value_type (ctag, command) );
    else
        _ctagToCommandMap.insert ( map<string, TL1BaseCmd*>::value_type (ctag, command) );
}

void TL1Session::unregisterCommand (const string &ctag)
{
    AutoUnLock lock (_ctagToCommandMapMutex)
    ;

    map<string, TL1BaseCmd*>::iterator iter = _ctagToUniversllListenersMap.find (ctag);
    if (iter != _ctagToUniversllListenersMap.end())
        _ctagToUniversllListenersMap.erase (iter);
    else
    {
        map<string, TL1BaseCmd*>::iterator iter = _ctagToCommandMap.find (ctag);
        if (iter != _ctagToCommandMap.end())
            _ctagToCommandMap.erase (iter);
    }

}

void TL1Session::abortPendingCommands (void)
{
    AutoUnLock lock (_ctagToCommandMapMutex)
    ;
    for (map<string, TL1BaseCmd*>::iterator iter = _ctagToUniversllListenersMap.begin();iter != _ctagToUniversllListenersMap.end();iter++)
    {
        TL1BaseCmd *command = (*iter).second;
        command->postCmdLock();
    }
    _ctagToUniversllListenersMap.clear();

    for (map<string, TL1BaseCmd*>::iterator iter = _ctagToCommandMap.begin();iter != _ctagToCommandMap.end();iter++)
    {
        TL1BaseCmd *command = (*iter).second;
        command->postCmdLock();
    }
    _ctagToCommandMap.clear();
}

void TL1Session::handleResponse (TL1Message* response)
{
    AutoUnLock lock (_ctagToCommandMapMutex)
    ;

    // make all universal listeners succeed
    map<string, TL1BaseCmd*>::iterator iter = _ctagToUniversllListenersMap.begin();
    while (iter != TL1Session::_ctagToUniversllListenersMap.end())
    {
        TL1BaseCmd* command = (*iter).second;
        _ctagToUniversllListenersMap.erase (iter++);
        command->handleResponse (response);
    }

    // to-to don't search for universal listeners
    iter = _ctagToCommandMap.find (response->m_tag);
    if (iter != _ctagToCommandMap.end())
    {
        TL1BaseCmd* command = (*iter).second;
        // only send responses to commands
        if (response->m_type == TL1Message::Response)
        {
            if (response->m_finished)
            {
                _ctagToCommandMap.erase (iter);
            }
            command->handleResponse (response);
        }
    }
    else
        ErrorLogger::logVarError( sourceName, "TL1Session", "handleResponse", TRACE3, "Could not find a command with ctag [%s]", response->m_tag.c_str());
}

void TL1Session::setEventListByType (const string &eventReceiverType, list<string> &eventList)
{
    _eventLists [eventReceiverType] = eventList;
}

bool TL1Session::isSupportedInServer(const string &eventReceiverType, const string &eventType)
{
    return (CmServer::instance()->isSupportedInServer (eventReceiverType, eventType, _neId));
}

void TL1Session::handleEvent (TL1Message* event, const char *rawMessage)
{
    _monitorEventRate();

    if (!_eventsAreBlocked)
    {
        _queueIsBackedUp = false;

        string eventType = event->m_outputCode1 + " " + event->m_outputCode2 + " " + event->m_outputCode3;
        ErrorLogger::logVarError(sourceName, "TL1Session", "handleEvent", __LINE__, TRACE7, "Received Autonomous Message of type %s", eventType.c_str());
        for (int i = 0; i < 3; i++)
        {
            if (isSupportedInServer (_eventHandlerTypes[i], eventType))
            {
                ErrorLogger::logVarError(sourceName, "TL1Session", "handleEvent", __LINE__, TRACE1, "Forwarding autonomous message to %s", _eventHandlerTypes[i].c_str(), eventType.c_str());
                _queueIsBackedUp = !forwardEventToReceiver(_eventHandlerTypes[i], event, rawMessage) || _queueIsBackedUp;
            }
            else
                ErrorLogger::logVarError(sourceName, "TL1Session", "handleEvent", __LINE__, TRACE11, "Autonomous Message type %s is NOT supported by the %s event handler; will not forward.", _eventHandlerTypes[i].c_str(), eventType.c_str());
        }
    }

    // make all universal listeners succeed
    AutoUnLock lock (_ctagToCommandMapMutex)
    ;
    map<string, TL1BaseCmd*>::iterator iter = _ctagToUniversllListenersMap.begin();
    while (iter != TL1Session::_ctagToUniversllListenersMap.end())
    {
        TL1BaseCmd* command = (*iter).second;
        _ctagToUniversllListenersMap.erase (iter++);
        command->handleResponse (event);
    }
}

void TL1Session::registerSessionByCTAG (const string &ctag, TL1Session *session)
{
    AutoUnLock lock (TL1Session::_ctagToSessionMapMutex)
    ;
    TL1Session::_ctagToSessionMap.insert (map <string, TL1Session*>::value_type (ctag, session));
}

void TL1Session::unregisterSessionByCTAG (const string &ctag)
{
    AutoUnLock lock (TL1Session::_ctagToSessionMapMutex)
    ;
    map <string, TL1Session*>::iterator i = TL1Session::_ctagToSessionMap.find (ctag);
    if (i != TL1Session::_ctagToSessionMap.end())
    {
        TL1Session *session = (*i).second;
        session->unregisterCommand (ctag);
        TL1Session::_ctagToSessionMap.erase (i);
    }
}

void TL1Session::unregisterAllSessionToCTAGMapping (TL1Session *targetSession)
{
    AutoUnLock lock (TL1Session::_ctagToSessionMapMutex)
    ;
    map <string, TL1Session*>::iterator i = TL1Session::_ctagToSessionMap.begin();
    while (i != TL1Session::_ctagToSessionMap.end())
    {
        TL1Session *session = (*i).second;
        if (session == targetSession)
            TL1Session::_ctagToSessionMap.erase (i++);
        else
            i++;
    }
}

void TL1Session::forwardResponseToSession(const char* responseText, TL1Message *response, TL1Connection *con, const string &connectionName)
{
    AutoUnLock lock (TL1Session::_ctagToSessionMapMutex)
    ;
    map <string, TL1Session*>::iterator i = TL1Session::_ctagToSessionMap.find (response->m_tag);
    if (i != TL1Session::_ctagToSessionMap.end())
    {
        TL1Session *session = (*i).second;
        // for acks we need to keep command in the map as the response is (perhaps) on the way
        if ((response->m_type != TL1Message::Acknowledgement) && (response->m_finished))
            TL1Session::_ctagToSessionMap.erase (i);
        // we still should pass everything to the session to decide what to do
        session->processMessage (responseText, response, con, connectionName);
    }
}

void TL1Session::registerSessionByTID (const string &tid, TL1Session *session)
{
    AutoUnLock lock (TL1Session::_tidToSessionMapMutex)
    ;
    TL1Session::_tidToSessionMap.insert (map <string, TL1Session*>::value_type (tid, session));
}

void TL1Session::unregisterSessionByTID (const string &tid)
{
    AutoUnLock lock (TL1Session::_tidToSessionMapMutex)
    ;
    map <string, TL1Session*>::iterator i = TL1Session::_tidToSessionMap.find (tid);
    if (i != TL1Session::_tidToSessionMap.end())
        TL1Session::_tidToSessionMap.erase (i);
}

void TL1Session::forwardEventToSession(const char* eventText, TL1Message *event, TL1Connection *con, const string &connectionName)
{
    AutoUnLock lock (TL1Session::_tidToSessionMapMutex)
    ;
    map <string, TL1Session*>::iterator i = TL1Session::_tidToSessionMap.find (event->m_sid);
    ;
    if (i != _tidToSessionMap.end())
    {
        TL1Session *session = (*i).second;
        session->processMessage (eventText, event, con, connectionName);
    }
}

void TL1Session::registerSessionByNeId (const string &neId, TL1Session *session)
{
    AutoUnLock lock (TL1Session::_neIdToSessionMapMutex)
    ;
    TL1Session::_neIdToSessionMap.insert (map <string, TL1Session*>::value_type (neId, session));
}

void TL1Session::unregisterSessionByNeId (const string &neId)
{
    AutoUnLock lock (TL1Session::_neIdToSessionMapMutex)
    ;
    map <string, TL1Session*>::iterator i = TL1Session::_neIdToSessionMap.find (neId);
    if (i != TL1Session::_neIdToSessionMap.end())
        TL1Session::_neIdToSessionMap.erase (i);
}

bool TL1Session::forwardCommandToSession (TL1BaseCmd *command)
{
    AutoUnLock lock (TL1Session::_neIdToSessionMapMutex)
    ;

    const string &neId = command->getNeId();
    map <string, TL1Session*>::iterator iter = TL1Session::_neIdToSessionMap.find (neId);
    if (iter != TL1Session::_neIdToSessionMap.end())
    {
        TL1Session *session = (*iter).second;
        return (session->sendCommand (command->getCommandText().c_str(), command->getCTAG(), command));
    }
    return false;
}

void TL1Session::_monitorEventRate()
{
    static const char* methodName = "_monitorEventRate";

    time_t currTime;
    time(&currTime);

    if (currTime - _shiftTime >= EVTRATE_SECONDS_PER_SLOT) // Is it time to shift the current slot?
    {
        int slotsPassed = ((currTime - _shiftTime) % EVTRATE_TOTAL_OBSERVED_SECONDS) / EVTRATE_SECONDS_PER_SLOT;
        while (slotsPassed--)
        {
            if (++_currEventSlot >= EVTRATE_NUM_SLOTS)
            {
                _currEventSlot = 0;
            }
            _eventCount[_currEventSlot] = 0;
        }
        _shiftTime = currTime;
    }

    _eventCount[_currEventSlot]++; // Update current event count slot

    if (_eventsAreBlocked) // Determine whether we need to unblock now
    {
        if (!_meteringEnabled)
        {
            ErrorLogger::logError(sourceName, "TL1Session", methodName, MAJOR,
                                  "Metering has been disabled, resuming event handling");

            _stopDroppingEvents();
        }
        else
        {
            int startSlot = _currEventSlot - _numberOfSlotsPerDelayPeriod;
            if (startSlot < 0)
            {
                startSlot += EVTRATE_NUM_SLOTS;
            }

            int currEventCount = 0;
            do
            {
                currEventCount += _eventCount[startSlot];
                if (++startSlot >= EVTRATE_NUM_SLOTS)
                {
                    startSlot = 0;
                }
            }
            while (startSlot != _currEventSlot);

            ErrorLogger::logVarError(sourceName, "TL1Session", methodName, MINOR,
                                     "Current event rate: %d events in the last %d seconds", currEventCount, _numberOfSlotsPerDelayPeriod * EVTRATE_SECONDS_PER_SLOT);

            int eventCountForUnblock = (_eventCountWhenBlocked * _eventPercentageForUnblock) / 100;
            if (eventCountForUnblock < (EVTRATE_MIN_RATE_FOR_UNBLOCK * _delayPeriodForBlock))
            {
                eventCountForUnblock = (EVTRATE_MIN_RATE_FOR_UNBLOCK * _delayPeriodForBlock);
            }

            if (currEventCount <= eventCountForUnblock)
            {
                ErrorLogger::logVarError(sourceName, "TL1Session", methodName, MAJOR,
                                         "Current event rate has dropped below %d, resuming event handling for NEID %s",
                                         eventCountForUnblock,
                                         _neId.c_str());

                _stopDroppingEvents();
            }
            else
            {
                ErrorLogger::logVarError(sourceName, "TL1Session", methodName, MINOR,
                                         "Current event rate is still higher than %d, event from NEID %s will be dropped",
                                         eventCountForUnblock,
                                         _neId.c_str());
            }
        }
    }
    else if (_queueIsBackedUp && _meteringEnabled)
    {
        _queueIsBackedUp = false;

        _numberOfSlotsPerDelayPeriod = _delayPeriodForBlock / EVTRATE_SECONDS_PER_SLOT;

        int startSlot = _currEventSlot - (_numberOfSlotsPerDelayPeriod * 2);
        if (startSlot < 0)
        {
            startSlot += EVTRATE_NUM_SLOTS;
        }

        // Count the events over the previous two periods
        int currEventCount = 0, prevEventCount = 0, i = 0;
        do
        {
            if (i++ < _numberOfSlotsPerDelayPeriod)
            {
                prevEventCount += _eventCount[startSlot];
            }
            else
            {
                currEventCount += _eventCount[startSlot];
            }

            if (++startSlot >= EVTRATE_NUM_SLOTS)
            {
                startSlot = 0;
            }
        }
        while (startSlot != _currEventSlot);

        ErrorLogger::logVarError(sourceName, "TL1Session", methodName, MINOR,
                                 "%d events received from %d seconds ago to %d seconds ago. %d events received from %d seconds ago to now",
                                 prevEventCount, _numberOfSlotsPerDelayPeriod * EVTRATE_SECONDS_PER_SLOT * 2, _numberOfSlotsPerDelayPeriod * EVTRATE_SECONDS_PER_SLOT,
                                 currEventCount, _numberOfSlotsPerDelayPeriod * EVTRATE_SECONDS_PER_SLOT);

        int eventCountForUnblock = (prevEventCount * _eventPercentageForUnblock) / 100;
        if (eventCountForUnblock < (EVTRATE_MIN_RATE_FOR_UNBLOCK * _delayPeriodForBlock))
        {
            eventCountForUnblock = (EVTRATE_MIN_RATE_FOR_UNBLOCK * _delayPeriodForBlock);
        }

        if (currEventCount >= eventCountForUnblock)
        {
            ErrorLogger::logVarError(sourceName, "TL1Session", methodName, MAJOR,
                                     "Current event rate is higher than %d in %d seconds, will start dropping events from NEID %s",
                                     eventCountForUnblock,
                                     _numberOfSlotsPerDelayPeriod * EVTRATE_SECONDS_PER_SLOT,
                                     _neId.c_str());

            _eventCountWhenBlocked = prevEventCount;
            _startDroppingEvents();
        }
        else
        {
            ErrorLogger::logVarError(sourceName, "TL1Session", methodName, MINOR,
                                     "Current event rate seems to have slowed down to lower than %d in %d seconds, ignoring queue backup for NEID %s",
                                     eventCountForUnblock,
                                     _numberOfSlotsPerDelayPeriod * EVTRATE_SECONDS_PER_SLOT,
                                     _neId.c_str());
        }
    }
}

void TL1Session::_startDroppingEvents()
{
    AutoUnLock lock (_eventReceiversMutex)
    ;

    if (!_eventsAreBlocked && _meteringEnabled)
    {
        _eventsAreBlocked = true;
        ErrorLogger::logVarError(sourceName, "TL1Session", "_startDroppingEvents", __LINE__, MAJOR, "Blocking events for NE TID %s", _tid.c_str());
        map <string, TL1EventReceiver*>::iterator i = _eventReceivers.find("CM");
        if (i != _eventReceivers.end())
        {
            i->second->blockedAction();
        }
    }
}

void TL1Session::_stopDroppingEvents()
{
    AutoUnLock lock (_eventReceiversMutex)
    ;

    if (_eventsAreBlocked)
    {
        _eventsAreBlocked = false;
        ErrorLogger::logVarError(sourceName, "TL1Session", "_stopDroppingEvents", __LINE__, MAJOR, "Resumed events for NE TID %s", _tid.c_str());
        map <string, TL1EventReceiver*>::iterator i = _eventReceivers.find("CM");
        if (i != _eventReceivers.end())
        {
            i->second->unblockedAction();
        }
    }
}

void TL1Session::configureMetering(bool enable, short delayPeriodForBlock, short percentDropForUnblock)
{
    _meteringEnabled = enable;

    if (!_meteringEnabled)
    {
        _stopDroppingEvents();
    }

    if (delayPeriodForBlock != 1)
    {
        if (0 != delayPeriodForBlock % EVTRATE_SECONDS_PER_SLOT || delayPeriodForBlock > EVTRATE_TOTAL_OBSERVED_SECONDS / 2)
        {
            char message[512];
            sprintf(message, "Delay period needs to be a multiple of %d and should be less than %d seconds", EVTRATE_SECONDS_PER_SLOT, EVTRATE_TOTAL_OBSERVED_SECONDS / 2);
            throw BaseException(message, "TL1Session", "_configureMetering");
        }

        _delayPeriodForBlock = delayPeriodForBlock;
    }

    if (percentDropForUnblock != -1)
    {
        _eventPercentageForUnblock = percentDropForUnblock;
    }
}

//
// Unblock the events for now only.
// Note that the events may get blocked again if an EventReceiver::handleEvent returns false again.
//
void TL1Session::unblockMetering()
{
    ErrorLogger::logError(sourceName, "TL1Session", "unblockMetering", TRACE1, "Method Begin");

    _queueIsBackedUp = false;
    _eventsAreBlocked = false;

    ErrorLogger::logError(sourceName, "TL1Session", "unblockMetering", TRACE1, "Method End");
}


void TL1Session::setNumberOfConnections(int numConnections)
{
    _connectionSemaphore->setMax(numConnections);
}
//****************************************************************************
//**getFailLogInCnt : return _failLogInCnt which records failure count of login
//****************************************************************************
unsigned long TL1Session::getFailLogInCnt()
{
    try
    {
        //osi_sem_read_lock lock (_rwSemForFailLogIn);
        ErrorLogger::logVarError(sourceName, "TL1Session", "getFailLogInCnt", __LINE__, TRACE11, "Current failure count is %d for %s.", _failLogInCnt, _tid.c_str());
        return _failLogInCnt;
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "getFailLogInCnt", __LINE__, MAJOR, "Thread Exception caught ->%s", ex.description(ex.code()));
        return _failLogInCnt;
    }
}
//****************************************************************************
//**increaseFailLogInCnt : increase _failLogInCnt by 1 if login failed.
//****************************************************************************
void TL1Session::increaseFailLogInCnt()
{
    try
    {
        osi_sem_write_lock lock (_rwSemForFailLogIn);
        ++_failLogInCnt;
        ErrorLogger::logVarError(sourceName, "TL1Session", "increaseFailLogInCnt", __LINE__, TRACE11, "Increase failure count by 1, and it is %d after increased for %s.", _failLogInCnt, _tid.c_str());
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "increaseFailLogInCnt", __LINE__, MAJOR, "Thread Exception caught ->%s", ex.description(ex.code()));
    }
}
//****************************************************************************
//**resetFailLogInCnt : reset _failLogInCnt to zero.
//****************************************************************************
void TL1Session::resetFailLogInCnt()
{
    try
    {
        osi_sem_write_lock lock (_rwSemForFailLogIn);
        ErrorLogger::logVarError(sourceName, "TL1Session", "resetFailLogInCnt", __LINE__, TRACE11, "Reset failure count to zero for %s.",_tid.c_str());
        _failLogInCnt = 0;
    }
    catch (osi_thread_toolkit_error& ex)
    {
        string desc = "Exception happened on sem read lock. ";
        ErrorLogger::logVarError(sourceName, "TL1Connection", "resetFailLogInCnt", __LINE__, MAJOR, "Thread Exception caught ->%s", ex.description(ex.code()));
    }
}
