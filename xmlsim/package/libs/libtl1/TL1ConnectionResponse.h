/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: TL1 Connection Response
* Type: C++
* Originator: $Author:   $
* File: $Workfile:   TL1Session.h  $
* Description:
* Class responsible to maintain the TL1 protocols connection to the socket.
*
* Revision History:
**********************************************************************************************/


#ifndef _TL1Session_H_
#define _TL1Session_H_

#include <string>
#include <map>
#include <list>

#include <ospace/thread/lock.h>
#include <ospace/thread/mutex.h>
#include <ospace/thread/rwsem.h>
#include <ospace/time/timedate.h>

#include "sysErrLog.h"
#include "EmsLock.h"
#include "TL1LocRecoverHandler.h"

class TL1AccountManager;
class TL1Message;

namespace EMS_FW
{

class CmdResult;
class TL1Logger;
class TL1Connection;
class TL1ConnectionCountSemaphore;
class TL1EventReceiver;
class TL1BaseCmd;

typedef osi_sem_lock<osi_mutex_semaphore> AutoUnLock;

class TL1Session
{
public:
    TL1Session();
    TL1Session (const std::string& neId, const std::string &tid,
                const std::string& rtrvCmd, const std::string& retry,
                const std::string& retrieveHeaderInterval,
                const std::string& hostname, unsigned portNumber,
                const std::string& logDirectory,
                const std::string& tidIndex,
                int numberOfConnections,
                bool meteringEnabled,
                int delayPeriodForBlock,
                int eventPercentageForUnblock,
                unsigned long maxLogSize = 20,
                bool wrapAround = true,
                int maxNumberOfFiles = 5,
                bool AMSUHCSimulatorSupport = false,
                bool deleteOnExit = false,
                const std::string& conTimeout = "",
                const std::string& conRetry = "");
    virtual ~TL1Session();

    bool initialize();
    bool finalize();

    virtual void connectionLoss(const std::string &conName);
    virtual void closeSession ();
    virtual void connectionRecovery (const std::string &conName);
    void ignoreConnectionLoss (void);
    void forceSwitchConnections (void);

    std::string gettid ()
    {
        return _tid;
    };
    std::string getNeId()
    {
        return _neId;
    };

    void startLogging (const std::string &tid);
    void stopLogging (const std::string &tid);

    void processMessage(const char* , TL1Message*, TL1Connection *con, const std::string &conName);
    bool sendCommand (const char* , const std::string& ctag, TL1BaseCmd *);

    bool login(CmdResult& result, TL1Connection *connectionToUse, bool changeExpiredPassword = true);
    bool logout(CmdResult& result, TL1Connection *);
    bool editPasswd(CmdResult& result, TL1Connection *, const std::string &username, const std::string &password);
    bool setLoginState (bool state, const std::string &conName = "", const std::string &completionCode = "", const std::string &errorCode = "");
    bool getLoginState (void);

    bool forwardEventToReceiver (const std::string &type, TL1Message* event, const char *);
    void registerEvenetReceiver (const std::string &type, TL1EventReceiver* receiver);
    void unregisterEventReceivers (void);


    void setUsername (const std::string& username)
    {
        _uid = username;
    };
    void setPassword (const std::string& password, short encryptType=0)
    {
        _password = password;
        _encryptType = encryptType;
    };
    short getPassword (std::string& password)
    {
        password = _password;
        return _encryptType;
    };

    void depositMessage (TL1LocRecoverHandler::MessageType messageType, ELogger::ErrorCode messageToSGM, const std::string& additionalInfo = "");

    void depositMessage (TL1LocRecoverHandler::MessageType messageType, const std::string& swAlarmDescription, const std::string& additionalSource = "");

    void registerHandler (TL1LocRecoverHandler* handler);
    void unregisterHandler (void);

    void registerAccountManager (TL1AccountManager *manager);

    void registerCommand (const std::string &ctag, TL1BaseCmd* command);
    void unregisterCommand (const std::string &ctag);
    void handleResponse (TL1Message *response);
    void handleEvent (TL1Message *event, const char *);
    void setEventListByType (const std::string &receiverType, std::list<std::string> &eventList);

    void configureMetering(bool enable, short delayPeriodForBlock = -1, short percentDropForUnblock = -1);
    void unblockMetering();

    void setNumberOfConnections(int numConnections);


    static void registerSessionByCTAG (const std::string &ctag, TL1Session *session);
    static void unregisterSessionByCTAG (const std::string &ctag);
    static void unregisterAllSessionToCTAGMapping (TL1Session *session);

    static void registerSessionByTID (const std::string &tid, TL1Session *session);
    static void unregisterSessionByTID (const std::string &tid);
    static void registerSessionByNeId (const std::string &neId, TL1Session *session);
    static void unregisterSessionByNeId (const std::string &neId);
    static bool forwardCommandToSession (TL1BaseCmd *command);
    static void forwardResponseToSession(const char* responseText, TL1Message *response, TL1Connection *con, const std::string &connectionName);
    static void forwardEventToSession(const char* eventText, TL1Message *event, TL1Connection *con, const std::string &connectionName);

    void logTraffic(string tid, string stringMessage, const string &messageType, const string& conName);

private:
    osi_time_and_date _timeLastMessageReceived;
    osi_mutex _timeLastMessageReceivedMutex;

    void setTimeLastMessageReceived ();
    long getSecondsSinceLastMessageReceived ();

    void _updateConnectionStatusInDB(const string& conName, bool connectionLoss);

    osi_mutex _ctagToCommandMapMutex;
    std::map<std::string, TL1BaseCmd*> _ctagToCommandMap;
    std::map<std::string, TL1BaseCmd*> _ctagToUniversllListenersMap;
    osi_read_write_semaphore rwSemaphore;
    osi_read_write_semaphore _locHanlderSemaphore;
    TL1AccountManager *_accountManager;
    std::string _waitOnIP;
    unsigned int _loginRetryWaitTime;
    EmsLock _connectionLossNotify;
    bool _shutdown;
    std::string _neId;
    unsigned long _retrieveHeaderInterval;
    std::string _rtrvCmd, _tid;
    osi_read_write_semaphore delaymultSemaphore;
    osi_thread* _keepSessionThread;
    static void* _processKeepSessionThread (void*);
    std::string _logDirectory;
    unsigned long _maxLogSize;
    bool _wrapAround;
    int _maxNumberOfFiles;
    bool _deleteOnExit;
    osi_mutex _loggerMutex;
    TL1Logger *_logger;
    void keepSessionFunction (void);
    bool retrieveHeader (CmdResult& result, TL1Connection *);
    std::map <std::string, TL1EventReceiver*> _eventReceivers;
    osi_mutex _eventReceiversMutex;
    std::string _uid;
    std::string _password;
    short _encryptType;    // 0: N, 1: Y, 2: A, -1: Unknown
    TL1LocRecoverHandler* _locHandler;
    bool _loggedIn;
    int _delayMultiplier;
    std::map<std::string, std::string> _reportedFailures;
    TL1ConnectionCountSemaphore *_connectionSemaphore;
    static osi_mutex _neIdToSessionMapMutex;
    static osi_mutex _tidToSessionMapMutex;
    static osi_mutex _ctagToSessionMapMutex;
    bool isSupportedInServer (const std::string &eventReceiverType, const std::string &eventType);

    std::map <std::string, std::list<std::string> > _eventLists;

    int getAdnSetKeepWaiting (int newValue = -1); // passing no parameter means get only
    std::string _generateNewPasswd();

    // Event Rate Metering
    //
    // The following three parameters are configurable on the fly.
    bool _meteringEnabled;
    int _delayPeriodForBlock;
    int _eventPercentageForUnblock;

    int* _eventCount; // Slots to store event count
    unsigned int _currEventSlot;
    time_t _shiftTime;

    bool _queueIsBackedUp; // Flag raised when a receiver::handleEvent returns false
    bool _eventsAreBlocked; // Flag to indicate that the events are being dropped right now

    int _numberOfSlotsPerDelayPeriod;  // _delayPeriodForBlock / EVTRATE_SECONDS_PER_SLOT stored when block happens
    int _eventCountWhenBlocked; // the number of events in _delayPeriodForBlock seconds when blocked happened

    // t71mr00165467 NGX NE change to LOC after 2 continuous RTRV-HDR fail
    int _rtrvHdrFailCnt;
    int _rtrvHdrFailThreshold;

    void _monitorEventRate();
    void _startDroppingEvents();
    void _stopDroppingEvents();

    static std::map <std::string, TL1Session *> _neIdToSessionMap;
    static std::map <std::string, TL1Session *> _tidToSessionMap;
    static std::map <std::string, TL1Session *> _ctagToSessionMap;

    bool _shuttingDown;

    std::string _messageTypeNames [13];
    std::string _eventHandlerTypes [3];
    void abortPendingCommands();
    osi_read_write_semaphore _rwSemForFailLogIn;
    unsigned long _failLogInCnt;
    unsigned int _loginRetryWaitTimeWhenFailed;
    unsigned int _loginRetryThreshold;
    EmsLock _eventNotify;
    unsigned long getFailLogInCnt();
    void increaseFailLogInCnt();
    void resetFailLogInCnt();
};
};
#endif
