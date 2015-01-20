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
* Originator: $Author:    $
* File: $Workfile:    $
* Version: $Revision:   1.5  $
* Description: 
* * 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#ifndef _TL1SimSocketAcceptor_H_
#define _TL1SimSocketAcceptor_H_

#include "SocketAcceptor.h"
#include <ospace/time.h>
#include <string>
#include <map>
#include <list>
#include <EmsTaskScheduler.h>

typedef int (*TaskHandler)(long taskId, const void *taskData);

using namespace std;
using namespace EMS_FW;
//namespace EMS_FW
//{
class TL1SimSocketAcceptor : public SocketAcceptor
{

public:

    static const string className;

    static TL1SimSocketAcceptor* getInstance(const string& acceptorId = "", const string& name = "");

    TL1SimSocketAcceptor()
    {}  // we need to initialize this connection using NeId

    TL1SimSocketAcceptor(unsigned portNumber, const string& hostName = "");

    virtual ~TL1SimSocketAcceptor();


    bool initialize();
    bool setTid(string tid);
    bool setConfigType(string &tid, string &type);
    bool setDefResCond(string& tid, string &response);
    bool setDelay(string& tid, string &delay);
    void getDefResCond(string& tid, string &defaultResp);
    void setPort(string port)
    {
        m_port = port;
    };
    void setIP(string ip)
    {
        m_ip = ip;
    };
    bool getTids(list<string> &tid);
    void getPort(string &port)
    {
        port = m_port;
    };
    void getIP(string &ip)
    {
        ip = m_ip;
    };

    bool getCommandDelay(string &command, int &delay);
    void getDelay(const string& tid, int &delay);
    bool getConfigType(const string &tid, string &type);
    string getCtagFromTL1Cmd(const string& ctag);
    void sendData(const string& response);
    bool getCommunicationState(const string& neTid, string& commState);
    bool setCommunicationState(string& neTid, string& commState);
    bool supportedTid(string& tid);

    static void GetTimeInAgentFormat(osi_date today, osi_time Timenow, string &todayDate, string &todayTime);

    void addNeIpList(const string& tid, const string& keyName);

    bool eraseNeIpList(const string& tid);

    static int stopIPandSendResponse(long taskId, const void *taskData);
    static int sendIP(long taskId, const void *taskData);
    static int sendDelayedResponse(long taskId, const void *taskData);
    static int stopIPandSendPartialResponses(long taskId, const void *taskData);
    static int sendDelayedPartialResponses(long taskId, const void *taskData);

    //bool login();

protected:

    string _acceptorId;

    map <std::string, std::string> _neIpList;

    virtual void processMessage (const string& message, const string &key);

    virtual void processMessage(const string& message);

    virtual void processThread();

    virtual bool incompleteMessage(const string& msg, int& iPos);

private:


    string getTidFromTL1Cmd(const string& cmd);
    string getAidFromTL1Cmd(const string& cmd);
    string getIpbyTid(const string& tid);
    list<string> m_tids;
    string m_port;
    string m_ip;

    std::map<std::string, std::string> m_defaultResponse;
    std::map<std::string, int> m_delay;
    std::map<std::string, std::string> m_configTypes;
    std::map<std::string, std::string> m_commState;
    osi_mutex m_connectionMutex;

};

//};



#endif
