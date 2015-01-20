/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Socket Acceptor
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1SocketAcceptor.h  $
* Version: $Revision:   1.5  $
* Description: 
* * 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#ifndef _TL1SocketAcceptor_H_
#define _TL1SocketAcceptor_H_

#include "SocketAcceptor.h"
#include <string>
#include <map>
#include <list>
#include "TL1EventReceiver.h"

namespace EMS_FW
{


class TL1SocketAcceptor : public SocketAcceptor
{

public:

    static const string className;

    static TL1SocketAcceptor* getInstance(const string& acceptorId = "", const string& name = "");

    TL1SocketAcceptor()
    {}  // we need to initialize this connection using NeId

    TL1SocketAcceptor(const string& acceptorId, const string& expectMsg,
                      const string& ackMsg, int negWaitPeriod, unsigned portNumber, const string& hostName = "");

    virtual ~TL1SocketAcceptor();


    bool initialize();

    void registerListener(const string &keyName, TL1EventReceiver* receiver);

    bool unregisterListener(const string &keyName);

    TL1EventReceiver* getListener (const string &keyName);


    const string& getId()
    {
        return _acceptorId;
    }

    virtual void connectionLoss();

    void addNeIpList(const string& tid, const string& keyName);

    bool eraseNeIpList(const string& tid);

    //bool login();

protected:

    string _acceptorId;

    map <string, string> _neIpList;

    virtual void processMessage(const char* message);

    virtual void processThread();

    virtual bool incompleteMessage(string& msg, string& retTL1Msg );

private:

    map <string, TL1EventReceiver*> _listeners;

    string getTidFromTL1Cmd(const string& cmd);

    string getIpbyTid(const string& tid);


};


};


#endif
