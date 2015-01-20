/***********************************************************************************************
* COPYRIGHT 2002 - 2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Deihl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: TL1 Connection
* Type: C++
* Originator: dtchalak
* File: TL1Connection.h
* Version:
* Description:
* Class responsible to maintain the TL1 protocols connection to the socket.
*
* Revision History:
* Revision History:
* Rev 1.3  02/14/2002  lzou
*     Add the implemenation to retrieve/update the varibles in NeInfo
* Rev 1.3  02/14/2002  lzou
*  Add the implementation for logout and break TL1 message block
* Rev 1.2  01/10/2002  lzou
*  Add the implementation for thread to maintain the session
* Rev 1.1  12/2002   lzou
*  Fixing the problem in incompletemessage check
*  Change the config value used for Tid, Uid, Password, Port so that server use the value
*   from the client instead of config file.
* Rev 1.0  init version  dtchalak

*
**********************************************************************************************/


#ifndef _TL1Connection_H_
#define _TL1Connection_H_

#include <ospace/thread/rwsem.h>

#include "SocketConnection.h"
#include "TL1EventReceiver.h"
#include "EmsLock.h"
#include "TL1LocRecoverHandler.h"
#include "BaseCmdInfo.h"
#include "TL1Message.h"

namespace EMS_FW
{

typedef struct neInfo
{
    string neId;
    string username;
    string password;
    bool tryLoggingIn;
    string lastATAG;
    int retrieveHeaderRetry;
}
neInfo;

typedef struct ResponseCombo
{
    TL1Message partialResponse;
    string partialResponseText;
}
ResponseCombo;

class TL1Connection : public SocketConnection
{

public:

    static const string className;

    static TL1Connection* getInstance(const string& neId = "", const string& name = "");

    TL1Connection()
    {}  // we need to initialize this connection using NeId

    TL1Connection(const string& neId, unsigned portNumber, const string& hostName,
                  bool AMSUHCSimulatorSupport = false, const string& conTimeout = "",
                  const string& conRetry = "");

    TL1Connection(const string& neId);

    TL1Connection (const string &hostName, unsigned long portNumber);

    virtual ~TL1Connection();


    bool initialize();

    // Send async command to the agent and return ctag
    string sendCommand(const string& preCmd, const string& postCmd,
                       TL1EventReceiver* receiver , int timeout = -1, const string &pCTAG = "");

    void registerListener(const string &keyName, TL1EventReceiver* receiver);

    bool unregisterListener(const string &keyName);

    TL1EventReceiver* getListener (const string &keyName);

    void registerHandler(EMS_FW::TL1LocRecoverHandler* handler);

    void unregisterHandler();

    TL1LocRecoverHandler* getHandler()
    {
        return _locHandler;
    }

    void addNeInfo (const string &tid, const string &neid, const string &username, const string &password);

    void removeNeInfo (const string &tid);

    neInfo *getNeInfo (const string &tid);

    const string& getNeId()
    {
        return _neId;
    }

    virtual void connectionLoss();

    bool login(const string& tid, CmdResult& result);

    bool logout(const string& tid, CmdResult& result);

    int getNeCount ()
    {
        return _neList.size();
    }

    void releaseReceiver(const string& ctag);

    bool getLoginFlagFromInfo(const string& tid);

    bool updateLoginFlagFromInfo(const string& tid, bool newFlag);

    bool isActive()
    {
        return (_isActive);
    };

    TL1EventReceiver* _getReceiver(TL1Message *, const char *);

    bool _receiverExists(const string &ctag);

    void clearCmdMap ();

    void clearCmdMapForNe (const string& neId);

    void insertReceiver (const string& ctag, TL1EventReceiver* receiver);

    bool connectionNegotiate (const string& negotiateMsg, const string& expectMsg, int timeout = 5);

    virtual bool incompleteMessage(string& msg, string& retTL1Msg);
protected:

    string _neId;

    virtual void processMessage(const char* message);

    virtual void processThread();

    map <string, TL1EventReceiver*> _listeners;

    map <string, neInfo*> _neList;

    TL1LocRecoverHandler* _locHandler;

    // FM/CM may derrive this method to pre-process raw TL1 messages
    virtual string preProcess (const char*);

    // FM/CM may derrive this method to post process TL1 message strcutres
    virtual void postProcess (TL1Message*);

    string getUidFromInfo(const string& tid);

    string getNeIdFromInfo(const string& tid);

    string getPasswdFromInfo(const string& tid);

    string getATAGFromInfo(const string& tid);

    int getRtrvHdrRetryFromInfo(const string& tid);

    bool updatePasswdFromInfo(const string& tid, const string& newPasswd);

    bool updateATAGFromInfo(const string& tid, const string& newATAG);

    bool updateRtrvHdrRetryFromInfo(const string& tid, int newRetry);

    bool _AMSUHCSimulatorSupport;



private:

    typedef map<string , TL1EventReceiver* , less<string> > cmdMapType;

    cmdMapType _cmdMap;

    // TL1EventReceiver* _getReceiver(const string& ctag);

    osi_read_write_semaphore rwSemaphore;
    osi_read_write_semaphore cmdMapLock;

    neInfo* _getNeInfo (const string &tid);
    string _findCtag (const string& msg, int startingPos, int& nposCtag);
    int _popCountFromMap (const string& key);
    int _getCountFromMap (const string& key);
    void _appendStrings (string& source, const string& appendStr, int& count);
    EmsLock _negotiateLock;
    string _expectedMsg;
    bool _acceptNewCmd;
    string _waitOnIP;


    map <string, list<ResponseCombo> > _responseBuffers;
    map <string, int > _incompleteResMap;    // this map of CTAG and count of open comments levels
};


};


#endif
