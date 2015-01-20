/***********************************************************************************************
* COPYRIGHT June 16 2002 - 2007 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: TL1 Socket Acceptor
* Type: C++
* Originator: mgilbert

* File: 
* Version: 
* Description: 
* 
* Revision History:
*
* 
**********************************************************************************************/


//#include "TL1ConnectionManager.h"
#include <map>
#include <ospace/thread.h>
#include <ospace/time.h>
#include "TL1SimSocketAcceptor.h"
#include "SocketHandler.h" 
//#include "sysErrLog.h"
//#include "tutil.h"
#include "TL1ConnectionManager.h"
#include "TL1CommandResolver.h"
#include <ospace/helper.h>
#include <ospace/thread/rwsem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ospace/thread/mutex.h>
#include <EmsTaskScheduler.h>

extern const char* sourceName;
extern bool _commandAutonomous;

using namespace std;

using namespace EMS_FW;
//using namespace ELogger;
//using namespace TL1_MA;


const string TL1SimSocketAcceptor::className = "TL1SimSocketAcceptor";


TL1SimSocketAcceptor* TL1SimSocketAcceptor::getInstance(const string& acceptorId, const string& name)
{

    return NULL;
}


TL1SimSocketAcceptor::TL1SimSocketAcceptor(unsigned portNumber, const string& hostName): SocketAcceptor(portNumber, hostName)
{
    /*setClassName(className);
    setName(acceptorId);
    _acceptorId = acceptorId;
    _expectedMsg = expectMsg+";";
    _ackMsg = ackMsg+";";
    _negWaitPeriod = negWaitPeriod;
      _neIpList.clear();*/

    char buffer[2048];
    sprintf(buffer, "%d", portNumber);

    setPort(buffer);
    setIP(hostName);
}



TL1SimSocketAcceptor::~TL1SimSocketAcceptor()
{
    _neIpList.clear();
    finalize();
}


bool TL1SimSocketAcceptor::initialize()
{ // need to initialize the connection to the device

    TL1CommandResolver* resolver = TL1CommandResolver::getInstance();
    list<string> responseList;
    string command = "<RTRV-NET>";

    list<string>::iterator iter;
    iter = m_tids.begin();
    string tid = (*iter);

    string configType;
    getConfigType (tid, configType);
    bool isRNE = false;
    if (resolver->resolveCommand(configType, command, responseList))
    {
        list<string>::iterator counter;
        for (counter = responseList.begin(); counter != responseList.end(); counter++)
        {
            string text = (*counter);
            // see if RNE is in TID-IPMAP
            if ( (text.find ("RNE")) != -1)
            {
                isRNE = true;
                break;
            }
        }
    }

    //ErrorLogger::logVarError(sourceName,"TL1SimSocketAcceptor","initialize",__LINE__,TRACE4,"Host is ->%s",_hostname.c_str());

    if (isRNE)
    {
        //cout << "NE [" << tid << "] is an RNE and will not allow direct connections " << endl;
        //return true;
    }

    if (!SocketAcceptor::initialize())
    {
        cout << "NE [" << tid << "] failed to create a listener on ip: " << m_ip << " port: " << m_port << endl;
        return false; // fall to initialize the connection
    }

    //ACE_OS::sleep(3); // wait the connection to be established before send login
    // need to send the command to the device
    cout << "Listening for connections on ip: " << m_ip << " port: " << m_port << endl;
    return true;
}
void TL1SimSocketAcceptor::processMessage (const string& message)
{}

void TL1SimSocketAcceptor::processMessage(const string& message, const string &key)
{
    //bool debugOutput = false;
    time_t seconds;
    long start = time(NULL);
    long finish;
    //if(ErrorLogger::isTraceListOn())
    // ErrorLogger::logVarError(sourceName,"TL1SimSocketAcceptor","processMessage",__LINE__,TRACE3,"Got the message length = %d ->%s",strlen(message),message);
    // here we need to process the message and notify
    //extract key from head of message

    //  cout << "Message received from client at [" << key << "] >>" << endl << message << endl;

    string msg1 = message;
    //string key2 = key;
    /*  int keystartpos = msg1.find_first_of ("[");
      int keyendpos = msg1.find_first_of ("]");
     string key = msg1.substr (keystartpos+1, keyendpos-keystartpos);*/

    if (key.empty())
    {
        cout << "No valid source info in message; ignoring";
        return ;
    }


    try
    {
        // sometimes a command is received from the craftstation with the first character
        // being a carriage return - we want to remove it
        int pos4 = msg1.find("\n");
        if (pos4 != -1)
        {
            cout << "processMessage found newline at " << pos4 << " in " << msg1 << endl;
            msg1.erase(0, pos4 + 1);
        }

        string ctag = getCtagFromTL1Cmd(msg1);
        if (ctag.empty())
        {
            cout << "processMessage could not find CTAG in " << msg1 << endl;
            ctag = "def01";
        }

        string tid = getTidFromTL1Cmd(msg1);
        if (tid.empty())
        {
            // this has to be a non-EMS command so assume it's meant for this NE
            list<string>::iterator iter;
            iter = m_tids.begin();
            tid = (*iter);
        }
        else
        {
            // can be an RNE or a GNE
            // get my TID
            list<string>::iterator iter;
            iter = m_tids.begin();
            string gneTid = (*iter);

            // if the tid supplied is now my own, look for an RNE with this TID
            if (gneTid != tid)
            {
                // get my config t
                string configType;
                getConfigType (gneTid, configType);

                osi_date todayDate = osi_date::today();
                osi_time todayTime = osi_time::now();

                string dateString, timeString;
                GetTimeInAgentFormat(todayDate, todayTime, dateString, timeString);

                string iitaresponse = gneTid + " " + dateString + " " + timeString + "\r\n" + "M " + ctag + " DENY" + "\r\n IITA \n /* Altaf says ... Input Invalid Target ID */ \n;";
                string srtoresponse = gneTid + " " + dateString + " " + timeString + "\r\n" + "M " + ctag + " DENY" + "\r\n SRTO \n /* Altaf says ... Unable to reach the target NE */ \n;";

                TL1CommandResolver* resolver = TL1CommandResolver::getInstance();
                list<string> responseList;
                string command = "<RTRV-TIDIPMAP>";
                if (resolver->resolveCommand(configType, command, responseList))
                {
                    if (responseList.size() == 0)
                    {
                        cout << "ResponseList size 0 for " << command << " on TID " << gneTid << "  " << configType << endl;
                        // send IITA
                        send_data (iitaresponse, key);
                        return ;

                    }
                    else
                    {
                        list<string>::iterator counter;
                        bool neintidmap = false;
                        for (counter = responseList.begin(); counter != responseList.end(); counter++)
                        {
                            string text = (*counter);
                            // see if RNE is in TID-IPMAP
                            if ( (text.find (tid)) != -1)
                            {
                                neintidmap = true;
                                break;
                            }
                        }
                        if (!neintidmap)
                        {
                            cout << "could not find NE  " << tid << " in " << gneTid << "'s TIDIPMAP (" << "  " << configType << ")" << endl;
                            // RNE not found send IITS
                            send_data (iitaresponse, key);
                            return ;
                        }
                        else
                        {
                            // RNE found in TIDIPMAP bout not in the configuration
                            string rneConfType;
                            getConfigType (tid, rneConfType);
                            if (rneConfType.empty())
                            {
                                send_data (srtoresponse, key);
                                return ;
                            }
                        }
                    }
                }
                // send IITA
                else
                {
                    cout << "could not resolve command " << command << " on TID " << gneTid << "  " << configType << endl;
                    send_data (iitaresponse, key);
                    return ;
                }
            }
        }





        string altCommand;
        string origAid;
        string aid = getAidFromTL1Cmd(msg1);
        origAid = aid;
        if (!aid.empty())
            aid = "::" + aid;

        int pos = aid.find("ALL");
        if (pos != -1)
            aid = "";

        if (!tid.empty())
        {
            list<string> responseList;
            list<string>::iterator counter;
            string configType;
            string response;
            string command;
            int pos = msg1.find(":");
            altCommand = "<" + msg1.substr(0, pos) + "::[AID]" + ">";
            command = "<" + msg1.substr(0, pos) + aid + ">";

            ConfigManager::toUpper(command);

            osi_date todayDate = osi_date::today();
            osi_time todayTime = osi_time::now();

            string dateString, timeString;

            GetTimeInAgentFormat(todayDate, todayTime, dateString, timeString);

            string defaultResponse;
            getDefResCond(tid, defaultResponse);

            int delay = 0;
            string tl1Code = message;
            tl1Code = tl1Code.substr(0, tl1Code.find_first_of(":"));

            if (!getCommandDelay(tl1Code, delay))
                getDelay(tid, delay);

            string connectionState;
            getCommunicationState(tid, connectionState);

            string header;
            int memPos = command.find("RTRV-MEMSTAT-MEM");
            if (memPos != -1)
            {
                header = tid + " " + dateString + " " + timeString + "\r\n" + "M  " + ctag + " " + " DENY\r\n" + "IICM\r\n" + "/* Input, Invalid MOD1 */\n";

            }
            else if (getConfigType(tid.c_str(), configType) && (connectionState == "ENABLE"))
            {
                TL1CommandResolver* resolver = TL1CommandResolver::getInstance();
                if (resolver->resolveCommand(configType, command, responseList) == false)
                    resolver->resolveCommand(configType, altCommand, responseList);

                //consistent with response from real NE in the log
                header = "\r\n\n   " + tid + " " + dateString + " " + timeString + "\r\n" + "M  " + ctag + " " + defaultResponse;

            }
            else
                header = "\r\n\n   " + tid + " " + dateString + " " + timeString + "\r\n" + "M " + ctag + " DENY";

            response = header;
            bool supportedAutonomousSectionBegin = false;

            list<string> autonomousList;
            string autonomousAid;
            list<string>::iterator autonomousListCounter;

            for (counter = responseList.begin(); counter != responseList.end(); counter++)
            {
                string text = (*counter);

                if (text.find("<SUPPORTEDAUTONOMOUS>") != -1)
                    supportedAutonomousSectionBegin = true;
                else if ( (supportedAutonomousSectionBegin == false) && (text.find("</SUPPORTEDAUTONOMOUS>") == -1))
                {
                    response = response + "\r\n   " + text;

                }
                else if ( (supportedAutonomousSectionBegin == true) && (text.find("</SUPPORTEDAUTONOMOUS>") == -1))
                {

                    autonomousAid = text;
                    autonomousList.push_back(autonomousAid);
                }

            }


            string part1;
            string part2;

            // substitue [AID] with the actual AID
            pos = response.find("[AID]");
            while (pos != -1)
            {
                response.replace(pos, 5, origAid);
                pos = response.find("[AID]");
            }

            int responseSize = response.size();
            int remainingSize = responseSize;
            int headerSize = header.size();
            string termination = string("\r\n;");
            int terminationSize = termination.size();
            string continuation = string("\n>\n");


            int begPrevBreak = 0;
            list<string> partialResponses;
            int blockBreak = 4092;

            //cout << "Process Message: response size = " << responseSize << " delay = " << delay << endl;

            //PARTIAL RESPONSES
            if (responseSize > 4095) //need to send partial responses
            {
                if (delay > 0)
                {
                    response = header + "***HEADER_END_MARK***" + response; //header will be extracted within the task

                    string* responsePointer = new string(response);
                    string* keyPointer = new string(key);

                    TL1SimSocketAcceptor* thisPointer = this;

                    pair<string*, string*> responseAndKey(responsePointer, keyPointer);
                    pair<string*, string*>* rAKPointer = new pair<string*, string*>(responseAndKey);

                    pair<pair<string*, string*>*, TL1SimSocketAcceptor*> args(rAKPointer, thisPointer);

                    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* argsPointer = new pair<pair<string*, string*>*, TL1SimSocketAcceptor*>(args);
                    void* voidArgs = argsPointer;

                    long longTypeCtag = atoi(ctag.c_str());

                    if (delay > 2000) //need to send in progress responses
                    {
                        string* ctagPointer = new string(ctag);

                        pair<string*, string*> ctagAndKey(ctagPointer, keyPointer);
                        pair<string*, string*>* cAKPointer = new pair<string*, string*>(ctagAndKey);
                        pair<pair<string*, string*>*, TL1SimSocketAcceptor*> args2(cAKPointer, thisPointer);
                        pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* args2Pointer = new pair<pair<string*, string*>*, TL1SimSocketAcceptor*>(args2);

                        void* voidArgs2 = args2Pointer;

                        ACE_Time_Value aceDelay, period, firstIP;
                        period.set(2, 0);
                        long absTime = delay / 1000 + time(NULL);
                        long absTime2 = 2 + time(NULL);
                        aceDelay.set(absTime, 0);
                        firstIP.set(absTime2, 0);

                        EmsTaskScheduler::instance()->start();
                        EmsTaskScheduler::instance()->scheduleNewTask(longTypeCtag, voidArgs, stopIPandSendPartialResponses, aceDelay);
                        EmsTaskScheduler::instance()->scheduleNewTask((longTypeCtag + 1000)*3, voidArgs2, sendIP, firstIP, period);
                    }

                    else //no in progress responses need to be sent
                    {
                        ACE_Time_Value aceDelay, period, firstIP;
                        period.set(2, 0);
                        long absStartTime = delay / 1000 + time(NULL);
                        aceDelay.set(absStartTime, 0);

                        EmsTaskScheduler::instance()->start();
                        EmsTaskScheduler::instance()->scheduleNewTask(longTypeCtag, voidArgs, sendDelayedPartialResponses, aceDelay);
                    }
                }

                else   //no delay, send partial responses here
                {
                    int remainingSize = responseSize;
                    int headerSize = header.size();
                    string termination = string("\r\n;");
                    int terminationSize = termination.size();
                    string continuation = string("\n>\n");

                    int begPrevBreak = 0;
                    //list<string> partialResponses;
                    int blockBreak = 4092;

                    //response = response + "\r";

                    if (response.find(">") == -1) //must calculate where to break blocks and add headers
                    {
                        //response = response.substr(0, remainingSize);

                        while (remainingSize > 4092) //4092 instead of 4095 to account for terminationSize
                        {
                            while (response.at(blockBreak) != '\n')
                                blockBreak--;

                            part1 = response.substr(0, blockBreak) + continuation;
                            //part2 = header + response.substr(blockBreak);

                            send_data(part1, key);

                            response.replace(0, blockBreak, header);
                            remainingSize = response.size();
                            //response = part2;
                            blockBreak = 4092;
                        }
                        response.append("\r\n;\n");
                        send_data(response, key); //send last block
                    }
                    else
                    {
                        //This section is for xml files that have been generated by an alternate
                        //script which leaves the continuation characters in the xml files. This
                        //way the responses are broken up exactly as they are in the real NE
                        //instead of being calculated by the simulator.

                        blockBreak = response.find(">");

                        while (blockBreak != -1)
                        {
                            part1 = response.substr(0, blockBreak) + ">\n";

                            send_data(part1, key);

                            //part2 = header + response.substr(blockBreak + 2);
                            //response = part2;
                            response.replace(0, blockBreak + 2, header);
                            blockBreak = response.find(">");
                        }
                        response.append("\r\n;\n");
                        send_data(response, key); //send last block
                    }
                }
            }

            //NO PARTIAL RESPONSES
            else
            {
                response = response + "\r\n;\n";

                if (delay > 0)
                {
                    string* responsePointer = new string(response);
                    string* keyPointer = new string(key);

                    TL1SimSocketAcceptor* thisPointer = this;

                    pair<string*, string*> responseAndKey(responsePointer, keyPointer);
                    pair<string*, string*>* rAKPointer = new pair<string*, string*>(responseAndKey);

                    pair<pair<string*, string*>*, TL1SimSocketAcceptor*> args(rAKPointer, thisPointer);

                    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* argsPointer = new pair<pair<string*, string*>*, TL1SimSocketAcceptor*>(args);
                    void* voidArgs = argsPointer;

                    long longTypeCtag = atoi(ctag.c_str());

                    if (delay > 2000) //need to send in progress responses
                    {
                        string* ctagPointer = new string(ctag);

                        pair<string*, string*> ctagAndKey(ctagPointer, keyPointer);
                        pair<string*, string*>* cAKPointer = new pair<string*, string*>(ctagAndKey);
                        pair<pair<string*, string*>*, TL1SimSocketAcceptor*> args2(cAKPointer, thisPointer);
                        pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* args2Pointer = new pair<pair<string*, string*>*, TL1SimSocketAcceptor*>(args2);

                        void* voidArgs2 = args2Pointer;

                        ACE_Time_Value aceDelay, period, firstIP;
                        period.set(2, 0);
                        long absTime = delay / 1000 + time(NULL);
                        long absTime2 = 2 + time(NULL);
                        aceDelay.set(absTime, 0);
                        firstIP.set(absTime2, 0);

                        EmsTaskScheduler::instance()->start();
                        EmsTaskScheduler::instance()->scheduleNewTask(longTypeCtag, voidArgs, stopIPandSendResponse, aceDelay);
                        EmsTaskScheduler::instance()->scheduleNewTask((longTypeCtag + 1000)*3, voidArgs2, sendIP, firstIP, period);
                    }

                    else //no in progress responses need to be sent
                    {
                        ACE_Time_Value aceDelay, period, firstIP;
                        period.set(2, 0);
                        long absStartTime = delay / 1000 + time(NULL);
                        aceDelay.set(absStartTime, 0);

                        EmsTaskScheduler::instance()->start();
                        EmsTaskScheduler::instance()->scheduleNewTask(longTypeCtag, voidArgs, sendDelayedResponse, aceDelay);
                    }
                }

                else //no delay, send response normally
                    send_data(response, key);
            }




            // this response has an associated autonomous message, generate it!
            if ((supportedAutonomousSectionBegin == true) && (_commandAutonomous == true))
            {
                string response1;


                list<string>::iterator autonomousListCounter;
                for (autonomousListCounter = autonomousList.begin(); autonomousListCounter != autonomousList.end(); autonomousListCounter++)
                {
                    // use the same interface as the command line user
                    string text = (*autonomousListCounter);
                    ConfigManager::toUpper(tid);
                    ConfigManager::toUpper(text);
                    string command = "GENERATE " + tid + " " + text;
                    string dontcare;
                    TL1CommandResolver* resolver = TL1CommandResolver::getInstance();
                    resolver->resolveUserCommand(command, response1, dontcare);
                }

            }

        }
    }
    catch (...)
    {
        cout << "exception in TL1SimSocketAcceptor::processMessage" << endl;
    }
}





void TL1SimSocketAcceptor::processThread()
{
    //ErrorLogger::logError( sourceName, "TL1SimSocketAcceptor", "processThread", TRACE4, "Method started");

    //ErrorLogger::logError( sourceName, "TL1SimSocketAcceptor", "processThread", TRACE4, "Method Ends");
}



bool TL1SimSocketAcceptor::incompleteMessage(const string& msg, int& iPos)
{
    if (msg.empty())
        return true;
    iPos = msg.find_first_of(";");
    if (iPos != osi_npos)
    {
        //ErrorLogger::logError( sourceName, "TL1SimSocketAcceptor", "incompleteMessage", TRACE7, "Found Message Terminator");
        return false;
    }
    //ErrorLogger::logError( sourceName, "TL1SimSocketAcceptor", "incompleteMessage", TRACE7, "Didn't find Message Terminator");
    return true;
}

void TL1SimSocketAcceptor::addNeIpList(const string& tid, const string& keyName)
{
    _neIpList.insert( pair<string, string>(tid, keyName));
}

bool TL1SimSocketAcceptor::eraseNeIpList(const string& tid)
{
    map <string, string>::iterator i = _neIpList.find (tid);
    if (i != _neIpList.end())
    {
        _neIpList.erase(i);
        return true;
    }
    return false;

}

string TL1SimSocketAcceptor::getIpbyTid(const string& tid)
{
    map <string, string>::iterator i = _neIpList.find (tid);
    string ip = "";
    if (i != _neIpList.end())
    {
        ip = (*i).second;
    }
    return ip;
}

string TL1SimSocketAcceptor::getTidFromTL1Cmd(const string& cmd)
{

    osi_tokenizer tokenizer( ":", true, "\\", "\n", "", false );

    vector <string> tokens = tokenizer.tokenize( cmd );
    if (tokens.size() > 1)
        return tokens[1];
    else
        return "";
}

string TL1SimSocketAcceptor::getCtagFromTL1Cmd(const string& cmd)
{
    osi_tokenizer tokenizer( ":", true, "\\", "\n", "", false );
    vector <string> tokens = tokenizer.tokenize( cmd );
    string token;
    if (tokens.size() > 3)
    {
        token = tokens[3];
        int pos = token.find(";");
        if (pos != -1)
            token = token.substr(0, pos);

    }

    return token;
}

string TL1SimSocketAcceptor::getAidFromTL1Cmd(const string& cmd)
{
    osi_tokenizer tokenizer( ":", true, "\\", "\n", "", false );
    vector <string> tokens = tokenizer.tokenize( cmd );
    string token;
    if (tokens.size() > 3)
    {
        return tokens[2];

    }

    return token;
}

bool TL1SimSocketAcceptor::setTid(string tid)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    bool found = false;
    list<string>::iterator iter;
    for (iter = m_tids.begin(); iter != m_tids.end(); iter++)
    {
        if ((*iter) == tid)
        {
            found = true;
            break;
        }
    }

    if (found == false)
        m_tids.push_back(tid);


    return true;
}

bool TL1SimSocketAcceptor::getTids(list<string> &tids)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    list<string>::iterator iter;
    for (iter = m_tids.begin(); iter != m_tids.end(); iter++)
        tids.push_back(*iter);

    return true;
}

bool TL1SimSocketAcceptor::supportedTid(string& tid)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    list<string>::iterator iter;
    for (iter = m_tids.begin(); iter != m_tids.end(); iter++)
    {
        if ((*iter) == tid)
            return true;
    }
    return false;
}

bool TL1SimSocketAcceptor::setConfigType(string &tid, string &type)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, string>::iterator iter = m_configTypes.find(tid);

    if (iter == m_configTypes.end())
    {
        m_configTypes.insert(pair<string, string>(tid, type));
    }
    else
        (*iter).second = type;


    return true;
}

bool TL1SimSocketAcceptor::getConfigType(const string &tid, string &type)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, string>::iterator iter;
    for (iter = m_configTypes.begin(); iter != m_configTypes.end(); iter++)
    {
        if ((*iter).first == tid)
        {
            type = ((*iter).second);
            break;
        }
    }

    return true;
}

void TL1SimSocketAcceptor::getDefResCond(string& tid, string &defaultResp)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, string>::iterator iter;
    for (iter = m_defaultResponse.begin(); iter != m_defaultResponse.end(); iter++)
    {
        if ((*iter).first == tid)
        {
            defaultResp = ((*iter).second);
            break;
        }
    }

}

bool TL1SimSocketAcceptor::setDefResCond(string& tid, string &defaultResponse)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, string>::iterator iter = m_defaultResponse.find(tid);

    if (iter == m_defaultResponse.end())
    {
        m_defaultResponse.insert(pair<string, string>(tid, defaultResponse));
    }
    else
        (*iter).second = defaultResponse;

    return true;
}

bool TL1SimSocketAcceptor::getCommandDelay(string &command, int &delay)
{
    ConnectionLock lock (m_connectionMutex)
        ;

    ConfigManager *configManager = ConfigManager::getInstance();

    if (configManager->getCommandDelay(command, delay))
        return true;
    else
        return false;
}

void TL1SimSocketAcceptor::getDelay(const string& tid, int &delay)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, int>::iterator iter = m_delay.find(tid);
    if (iter != m_delay.end())
    {
        delay = ((*iter).second);
    }

}

bool TL1SimSocketAcceptor::setDelay(string& tid, string &delay)
{
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, int>::iterator iter = m_delay.find(tid);

    if (iter == m_delay.end())
    {
        m_delay.insert(pair<string, int>(tid, atoi(delay.c_str())));
    }
    else
        (*iter).second = atoi(delay.c_str());

    return true;
}

void
TL1SimSocketAcceptor::GetTimeInAgentFormat(osi_date today, osi_time Timenow, string &todayDate, string &todayTime)
{
    char month[3], day[3], year[5];
    char hour[3], minute[3], second[3];
    char usecond[3];
    char microsecond[7]; //Used for converting 6byte string returned by microseconds to usecond

    sprintf(year, "%d", today.year());
    if (today.year() >= 0 && today.year() < 10)
    {
        todayDate = todayDate + "0";
    }

    todayDate = todayDate + year + "-";
    todayDate = todayDate.substr(2);

    sprintf(month, "%d", today.month());
    if (today.month() >= 0 && today.month() < 10)
        todayDate = todayDate + "0";
    todayDate = todayDate + month + "-";


    sprintf(day, "%d", today.day());
    if (today.day() >= 0 && today.day() < 10)
        todayDate = todayDate + "0";
    todayDate = todayDate + day;

    sprintf(hour, "%d", Timenow.hours());
    if (Timenow.hours() >= 0 && Timenow.hours() < 10)
        todayTime = todayTime + "0";
    todayTime = todayTime + hour + ":";

    sprintf(minute, "%d", Timenow.minutes());
    if (Timenow.minutes() >= 0 && Timenow.minutes() < 10)
        todayTime = todayTime + "0";
    todayTime = todayTime + minute + ":";

    sprintf(second, "%d", Timenow.seconds());
    if (Timenow.seconds() >= 0 && Timenow.seconds() < 10)
        todayTime = todayTime + "0";
    todayTime = todayTime + second;
}

void TL1SimSocketAcceptor::sendData(const string& response)
{
    // autonomous messages are sent to everybody
    send_data(response);

}

bool TL1SimSocketAcceptor::getCommunicationState(const string& neTid, string& commState)
{
    ConnectionLock lock (m_connectionMutex)
        ;

    map<string, string>::iterator iter = m_commState.find(neTid);

    if (iter != m_commState.end())
    {
        commState = ((*iter).second);
        return true;
    }

    return false;
}

bool TL1SimSocketAcceptor::setCommunicationState(string& neTid, string& commState)
{
    map<string, string>::iterator communicationStateIterator;
    ConnectionLock lock (m_connectionMutex)
        ;

    communicationStateIterator = m_commState.find(neTid);
    if (communicationStateIterator == m_commState.end())
    {
        m_commState.insert(pair<string, string>(neTid, commState));
    }
    else
        ((*communicationStateIterator).second) = commState;

    return true;
}

int TL1SimSocketAcceptor::stopIPandSendResponse(long taskId, const void *taskData)
{


    EmsTaskScheduler::instance()->cancelScheduledTask((taskId + 1000)*3); //first cancel the task which is sending IP responses

    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* data = (pair<pair<string*, string*>*, TL1SimSocketAcceptor*>*) taskData;

    TL1SimSocketAcceptor* connection = data->second;

    pair<string*, string*>* responseAndKey = (data->first);

    string response = (*(responseAndKey->first));
    string key = (*(responseAndKey->second));

    connection->send_data(response, key);

    delete responseAndKey->first;
    responseAndKey->first = NULL;
    delete responseAndKey->second;
    responseAndKey->second = NULL;
    delete responseAndKey;
    responseAndKey = NULL;
    delete data;
    data = NULL;

    return 0;
}

int TL1SimSocketAcceptor::stopIPandSendPartialResponses(long taskId, const void *taskData)
{
    //this function is called for a response that requires IP and partial responses

    EmsTaskScheduler::instance()->cancelScheduledTask((taskId + 1000)*3);

    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* data = (pair<pair<string*, string*>*, TL1SimSocketAcceptor*>*) taskData;

    TL1SimSocketAcceptor* connection = data->second;

    pair<string*, string*>* responseAndKey = (data->first);

    string response = (*(responseAndKey->first));
    string key = (*(responseAndKey->second));

    string termination = string("\r\n;");
    int terminationSize = termination.size();
    string continuation = string("\n>\n");

    int begPrevBreak = 0;
    list<string> partialResponses;
    int blockBreak = 4092;
    string part1;
    string part2;

    string headerEndMark = "***HEADER_END_MARK***";

    int endOfHeader = response.find(headerEndMark);

    if (endOfHeader == -1)
        cout << "ERROR in sending delayed partial responses\n";

    string header = response.substr(0, endOfHeader);
    response = response.substr(header.size() + headerEndMark.size()); //cut off header and headerEndMark
    int remainingSize = response.size();

    response = response + "\r";

    if (response.find(">") == -1) //must calculate where to break blocks and add headers
    {
        response = response.substr(0, remainingSize);

        while (remainingSize > 4092) //4092 instead of 4095 to account for terminationSize
        {
            while (response[blockBreak] != '\n')
                blockBreak--;

            part1 = response.substr(0, blockBreak) + continuation;
            part2 = header + response.substr(blockBreak, remainingSize);

            connection->send_data(part1, key);

            remainingSize = remainingSize - blockBreak;
            response = part2;
            blockBreak = 4092;
        }
        response = response + "\r\n;";

        connection->send_data(response, key);
    }

    else
    {
        //This section is for xml files that have been generated by an alternate
        //script which leaves the continuation characters, < , in the xml files. This
        //way the responses are broken up in the same way they are in the real NE
        //instead of being calculated by the simulator.

        blockBreak = response.find(">");

        while (blockBreak != -1)
        {
            part1 = response.substr(0, blockBreak) + ">\n";
            response = header + part1.substr(blockBreak + 2);

            connection->send_data(part1, key);

            blockBreak = response.find(">");
        }

        response = response + "\r\n;";

        connection->send_data(response, key);
    }

    delete responseAndKey->first;
    responseAndKey->first = NULL;
    delete responseAndKey->second;
    responseAndKey->second = NULL;
    delete responseAndKey;
    responseAndKey = NULL;
    delete data;
    data = NULL;

    return 0;
}

int TL1SimSocketAcceptor::sendIP(long taskId, const void *taskData)
{
    //this function sends the in progress responses every 2 seconds, it gets canceled
    // by one of the stopIP functions after the delay time has passed

    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* data = (pair<pair<string*, string*>*, TL1SimSocketAcceptor*>*) taskData;

    TL1SimSocketAcceptor* connection = data->second;

    pair<string*, string*>* ctagAndKey = (data->first);

    string ctag = (*(ctagAndKey->first));
    string key = (*(ctagAndKey->second));

    string inProgress = "IP " + ctag + "\r\n<";

    connection->send_data(inProgress, key);

    return 0;
}

int TL1SimSocketAcceptor::sendDelayedResponse(long taskId, const void *taskData)
{
    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* data = (pair<pair<string*, string*>*, TL1SimSocketAcceptor*>*) taskData;

    TL1SimSocketAcceptor* connection = data->second;

    pair<string*, string*>* responseAndKey = (data->first);

    string response = (*(responseAndKey->first));
    string key = (*(responseAndKey->second));

    connection->send_data(response, key);

    delete responseAndKey->first;
    responseAndKey->first = NULL;
    delete responseAndKey->second;
    responseAndKey->second = NULL;
    delete responseAndKey;
    responseAndKey = NULL;
    delete data;
    data = NULL;

    return 0;
}

int TL1SimSocketAcceptor::sendDelayedPartialResponses(long taskId, const void *taskData)
{
    //this function sends partial responses for a delayed command or NE
    pair<pair<string*, string*>*, TL1SimSocketAcceptor*>* data = (pair<pair<string*, string*>*, TL1SimSocketAcceptor*>*) taskData;

    TL1SimSocketAcceptor* connection = data->second;

    pair<string*, string*>* responseAndKey = (data->first);

    string response = (*(responseAndKey->first));
    string key = (*(responseAndKey->second));

    string part1;
    string part2;

    string termination = string("\r\n;");
    int terminationSize = termination.size();
    string continuation = string("\n>\n");

    int begPrevBreak = 0;
    list<string> partialResponses;
    int blockBreak = 4092;

    string headerEndMark = "***HEADER_END_MARK***";

    int endOfHeader = response.find(headerEndMark);

    if (endOfHeader == -1)
        cout << "ERROR in sending delayed partial responses\n";

    string header = response.substr(0, endOfHeader);
    response = response.substr(header.size() + headerEndMark.size()); //cut off header and headerEndMark
    int remainingSize = response.size();

    response = response + "\r";

    if (response.find(">") == -1) //must calculate where to break blocks and add headers
    {
        response = response.substr(0, remainingSize);

        while (remainingSize > 4092) //4092 instead of 4095 to account for terminationSize
        {
            while (response[blockBreak] != '\n')
                blockBreak--;

            part1 = response.substr(0, blockBreak) + continuation;
            part2 = header + response.substr(blockBreak, remainingSize);

            connection->send_data(part1, key);

            remainingSize = remainingSize - blockBreak;
            response = part2;
            blockBreak = 4092;
        }
    }

    else
    {
        //This section is for xml files that have been generated by an alternate
        //script which leaves the continuation characters in the xml files. This
        //way the responses are broken up exactly as they are in the real NE
        //instead of being calculated by the simulator.

        blockBreak = response.find(">");

        while (blockBreak != -1)
        {
            part1 = response.substr(0, blockBreak) + ">\n";
            response = header + part1.substr(blockBreak + 2);

            connection->send_data(part1, key);

            blockBreak = response.find(">");
        }
    }

    response = response + "\r\n;";
    connection->send_data(response, key); //send last block

    delete responseAndKey->first;
    responseAndKey->first = NULL;
    delete responseAndKey->second;
    responseAndKey->second = NULL;
    delete responseAndKey;
    responseAndKey = NULL;
    delete data;
    data = NULL;

    return 0;
}
