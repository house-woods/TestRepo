/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Base Command
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1BaseCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Base TL1 command implementing async interface.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _TL1BaseCmd_H_
#define _TL1BaseCmd_H_
#include <string>
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
#include "BaseCmd.h"
#include "TL1EventReceiver.h"
#include "TL1ConnectionResponse.h"
#include "EmsLock.h"
#include <list>

#include <ospace/thread.h>

namespace EMS_FW
{

class BaseCmdInfo;
class TL1Connection;
class TL1MsgConverter;

typedef osi_sem_lock<osi_mutex_semaphore> MessageLock;

class TL1BaseCmd : public BaseCmd , public TL1EventReceiver
{
public:
    TL1BaseCmd()
    { }
    ~TL1BaseCmd();
    TL1BaseCmd(BaseCmdInfo* info, TL1Connection *con = NULL);
    virtual bool hasMoreElements();
    virtual bool getNextElement(EMS_FW::BaseCmdInfo* info);
    virtual bool getNextResponse();
    void handleResponse(TL1Message* event, const string& rawMessage = "");
    void getCompletionInfo (string &completionCode, string &failureReason);
    void postCmdLock ();
    CmdResult sendCommand (TL1Connection *con, const string& preCmd, const string &postCmd, int timeout, int trys = 1);
    CmdResult sendCommand(const string& preCmd, const string& postCmd, int timeout, int trys = 1);
    virtual bool initialize();
    virtual bool finalize();
    string getCommandText ();
    string getCTAG ();
    virtual string getNeId ();
    static string generateCTAG (void);
    virtual CmdResult execute();
    virtual CmdResult execute (TL1Connection *con);
    virtual bool isUniversalListener ();
    virtual void cleanUpResList();

protected:
    bool _discardFlag;
    CmdResult _result;
    virtual void prepare (string &preCmd, string &postCmd, int &timeOut);
    string _commandText, _ctag;
    EmsLock _lock;
    list<BaseCmdInfo*> _responseList;

private:
    bool _inLock;
    osi_mutex _lockMutex;
    bool _keepWaiting;
    string _waitOnIP;
    static osi_mutex _ctagsInUseMutex;
    TL1Session *_session;

    static void releaseCTAG (const string &ctag);
    static bool isCTAGInUse (const string &ctag);
    static list<string> _ctagsInUse;

};
};
#endif



