/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Socket Acceptor
* Type: C++
* Originator: $Author:   mgilbert  $
* File: $Workfile:    $
* Version: $Revision:     $
* Description: 
* 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#include "TL1ConnectionManager.h"
#include "TL1SimSocketAcceptor.h"
#include <map>
using namespace std;


TL1ConnectionManager* TL1ConnectionManager::m_connectionMgr = NULL;

TL1ConnectionManager::~TL1ConnectionManager()
{
    cout << "Connection Manager destructed!" << endl;
}

TL1ConnectionManager* TL1ConnectionManager::getInstance()
{
    if (TL1ConnectionManager::m_connectionMgr == NULL)
        TL1ConnectionManager::m_connectionMgr = new TL1ConnectionManager();

    return TL1ConnectionManager::m_connectionMgr ;

}

bool TL1ConnectionManager::initializeRne(string &tid, string &ip, string &port, string &ip2, string &port2, map<string, PropertiesList> &neList, ConfigManager *configManager)
{
    map<string, PropertiesList>::iterator neListIter;
    list<string> attrList;
    string gneTid;

    //searches through neList for a GNE whose tidipmap contains the RNE
    for (neListIter = neList.begin(); neListIter != neList.end(); neListIter++)
    {

        gneTid = (*neListIter).first;

        attrList.clear();
        configManager->getAttribute(gneTid, string("<NE>"), string("<CONFIGTYPE>"), attrList);
        string configType = (*attrList.begin());
        attrList.clear();
        configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<RTRV-TIDIPMAP>"), attrList);
        if (!attrList.empty())
        {
            list<string>::iterator tidipmapIndex;

            for (tidipmapIndex = attrList.begin(); tidipmapIndex != attrList.end(); tidipmapIndex++)
            {

                int pos = (*tidipmapIndex).find(tid + "\\");
                if (pos != -1)
                {

                    list<string> temp;
                    configManager->getAttribute(gneTid, string("<NE>"), string("<IP>"), temp);
                    ip = (*temp.begin());

                    temp.clear();
                    configManager->getAttribute(gneTid, string("<NE>"), string("<PORT>"), temp);
                    port = (*temp.begin());



                    map<AttributeName, AttributeValueList> newAttrList;
                    configManager->getAttributes(tid, newAttrList, string("<NE>"));
                    list<string> iplist;
                    list<string> portlist;
                    iplist.push_back(ip);
                    portlist.push_back(port);
                    newAttrList[string("<IP>")] = iplist;
                    newAttrList[string("<PORT>")] = portlist;
                    configManager->setAttributes(tid, newAttrList, string("<NE>"));


                    //look for back up gne
                    neListIter++;
                    list<string> aList;
                    list<string> tidipmap;
                    while (neListIter != neList.end())
                    {
                        gneTid = (*neListIter).first;

                        aList.clear();
                        configManager->getAttribute(gneTid, string("<NE>"), string("<CONFIGTYPE>"), aList);
                        configType = (*aList.begin());

                        tidipmap.clear();
                        configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<RTRV-TIDIPMAP>"), tidipmap);

                        if (!tidipmap.empty())
                        {

                            for (tidipmapIndex = tidipmap.begin(); tidipmapIndex != tidipmap.end(); tidipmapIndex++)
                            {
                                pos = (*tidipmapIndex).find(tid + "\\");
                                if (pos != -1)
                                {
                                    temp.clear();
                                    configManager->getAttribute(gneTid, string("<NE>"), string("<IP>"), temp);
                                    ip2 = (*temp.begin());

                                    temp.clear();
                                    configManager->getAttribute(gneTid, string("<NE>"), string("<PORT>"), temp);
                                    port2 = (*temp.begin());

                                    return true;
                                }
                            }
                        }
                        neListIter++;
                    }
                    return false;
                }
            }
        }
    }
}

bool TL1ConnectionManager::initializeGneb(string &tid, string &ip, string &port, map<string, PropertiesList> &neList, ConfigManager *configManager)
{
    map<string, PropertiesList>::iterator neListIter;
    list<string> attrList;

    //searches through neList for a GNE whose tidipmap contains the RNE
    for (neListIter = neList.begin(); neListIter != neList.end(); neListIter++)
    {

        string gneTid = (*neListIter).first;

        attrList.clear();
        configManager->getAttribute(gneTid, string("<NE>"), string("<CONFIGTYPE>"), attrList);

        string configType = (*attrList.begin());
        attrList.clear();
        configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<RTRV-TIDIPMAP>"), attrList);
        if (!attrList.empty())
        {
            list<string>::iterator tidipmapIndex;

            for (tidipmapIndex = attrList.begin(); tidipmapIndex != attrList.end(); tidipmapIndex++)
            {
                int pos = (*tidipmapIndex).find(tid + "\\");
                if (pos != -1)
                {
                    list<string> temp;

                    configManager->getAttribute(gneTid, string("<NE>"), string("<IP>"), temp);
                    ip = (*temp.begin());

                    temp.clear();
                    configManager->getAttribute(gneTid, string("<NE>"), string("<PORT>"), temp);
                    port = (*temp.begin());

                    map<AttributeName, AttributeValueList> newAttrList;
                    configManager->getAttributes(tid, newAttrList, string("<NE>"));
                    list<string> iplist;
                    list<string> portlist;
                    iplist.push_back(ip);
                    portlist.push_back(port);
                    newAttrList[string("<IP>")] = iplist;
                    newAttrList[string("<PORT>")] = portlist;
                    configManager->setAttributes(tid, newAttrList, string("<NE>"));

                    return true;
                }
            }
        }
    }
}

bool TL1ConnectionManager::startConnections(void)
{
    const string hostName = "localhost";

    map<string, PropertiesList> neList;
    map<string, PropertiesList>::iterator counter, rnePointer;
    map<AttributeName, AttributeValueList>::iterator counter2;
    list<string>::iterator responseListCounter, ipPointer, portPointer;

    ConfigManager* configManager = ConfigManager::getInstance();
    configManager->getTcpConfigurations(neList);

    string eontype;
    string ip;
    string ip2; //ip of the backup gne
    string port;
    string port2; //port of the backup gne
    string tid;
    string configType;
    string defaultResponseCond;
    string delay;
    string connectionState = "ENABLE";

    for (counter = neList.begin(); counter != neList.end(); counter++)
    {
        ip2 = "-1";
        port2 = "-1";
        tid = ((*counter).first);

        bool parentTIDs = false;
        for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
        {
            responseListCounter = ((*counter2).second).begin();

            if ((*counter2).first == "<PORT>")
            {
                port = (*responseListCounter);

            }
            else if ((*counter2).first == "<IP>")
            {
                ip = (*responseListCounter);

            }

            else if ((*counter2).first == "<CONFIGTYPE>")
                configType = (*responseListCounter);

            else if ((*counter2).first == "<DEFAULTRESPONSE>")
                defaultResponseCond = (*responseListCounter);

            else if ((*counter2).first == "<EONTYPE>")
                eontype = (*responseListCounter);

        }

        if (eontype == "RNE")
        {

            if (!initializeRne(tid, ip, port, ip2, port2, neList, configManager))
                cout << "Problem initializing RNE " << tid << "." << endl;



        }


        else if (eontype.find("GNEB") != -1)
        {
            ip2 = ip;
            port2 = port;

            if (!initializeGneb(tid, ip, port, neList, configManager))
                cout << "Problem initializing GNEB " << tid << "." << endl;



        }


        unsigned portNumber = atoi(port.c_str());
        map<string, TL1SimSocketAcceptor*>::iterator iter = m_listAcceptors.find(ip + port);

        cout << endl << tid << "--------------------" << endl;
        if (iter == m_listAcceptors.end())
        {
            cout << "Acceptor created on ip " << ip << " and port " << port << endl;
            TL1SimSocketAcceptor* acceptor = new TL1SimSocketAcceptor(portNumber, ip);
            acceptor->setIP(ip);
            acceptor->setTid(tid);
            acceptor->setPort(port);
            acceptor->setConfigType(tid, configType);
            acceptor->setDefResCond(tid, defaultResponseCond);
            acceptor->setCommunicationState(tid, connectionState);

            list<string> configList;
            list<string> ::iterator iter;
            configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<DELAY>"), configList);
            iter = configList.begin();
            delay = (*iter);
            acceptor->setDelay(tid, delay);

            m_listAcceptors.insert(pair<string, TL1SimSocketAcceptor*>(ip + port, acceptor));

        }

        else
        {
            cout << "Using existing acceptor on ip " << ip << " and port " << port << endl;
            ((TL1SimSocketAcceptor*)(*iter).second)->setTid(tid);
            ((TL1SimSocketAcceptor*)(*iter).second)->setConfigType(tid, configType);
            ((TL1SimSocketAcceptor*)(*iter).second)->setDefResCond(tid, defaultResponseCond);
            list<string> configList;
            list<string> ::iterator iter1;
            configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<DELAY>"), configList);
            iter1 = configList.begin();
            delay = (*iter1);
            ((TL1SimSocketAcceptor*)(*iter).second)->setDelay(tid, delay);
            ((TL1SimSocketAcceptor*)(*iter).second)->setCommunicationState(tid, connectionState);
            //TL1SimSocketAcceptor* acceptor = ((TL1SimSocketAcceptor*)(*iter).second);
        }

        //if (eontype.find("GNEB") != -1)
        //  cloneAcceptor((TL1SimSocketAcceptor*)(*iter).second);

        //add backup acceptor for current ne
        portNumber = atoi(port2.c_str());
        iter = m_listAcceptors.find(ip2 + port2);

        if (iter == m_listAcceptors.end() && ip2 != "-1")
        {
            cout << "Acceptor created on ip " << ip2 << " and port " << port2 << endl;
            TL1SimSocketAcceptor* acceptor = new TL1SimSocketAcceptor(portNumber, ip2);
            acceptor->setIP(ip2);
            acceptor->setTid(tid);
            acceptor->setPort(port2);
            acceptor->setConfigType(tid, configType);
            acceptor->setDefResCond(tid, defaultResponseCond);
            acceptor->setCommunicationState(tid, connectionState);

            list<string> configList;
            list<string> ::iterator iter;
            configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<DELAY>"), configList);
            iter = configList.begin();
            delay = (*iter);
            acceptor->setDelay(tid, delay);

            m_listAcceptors.insert(pair<string, TL1SimSocketAcceptor*>(ip2 + port2, acceptor));

        }
        else if (ip2 != "-1")
        {
            cout << "Using existing acceptor on ip " << ip2 << " and port " << port2 << endl;
            ((TL1SimSocketAcceptor*)(*iter).second)->setTid(tid);
            ((TL1SimSocketAcceptor*)(*iter).second)->setConfigType(tid, configType);
            ((TL1SimSocketAcceptor*)(*iter).second)->setDefResCond(tid, defaultResponseCond);
            list<string> configList;
            list<string> ::iterator iter1;
            configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<DELAY>"), configList);
            iter1 = configList.begin();
            delay = (*iter1);
            ((TL1SimSocketAcceptor*)(*iter).second)->setDelay(tid, delay);
            ((TL1SimSocketAcceptor*)(*iter).second)->setCommunicationState(tid, connectionState);
            //TL1SimSocketAcceptor* acceptor = ((TL1SimSocketAcceptor*)(*iter).second);
        }
    }
    // a quick fix to support GNE and RNE EMS switch requests
    // all details about all NEs are added to all ports


    //TL1SimSocketAcceptor* acceptor = new TL1SimSocketAcceptor(3082, string("172.23.169.5"));
    //acceptor->setIP(string("172.23.169.5"));
    //acceptor->setPort(string("3082"));
    //m_listAcceptors.insert(pair<string, TL1SimSocketAcceptor*>(string("172.23.169.53082"), acceptor));

    cout << endl << "Initializing socket acceptors..." << endl;
    cout << "----------------------------------" << endl;
    map<string, TL1SimSocketAcceptor*>::iterator iter;
    for (iter = m_listAcceptors.begin(); iter != m_listAcceptors.end(); iter++)
    {
        for (counter = neList.begin(); counter != neList.end(); counter++)
        {
            tid = ((*counter).first);
            for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
            {
                responseListCounter = ((*counter2).second).begin();

                if ((*counter2).first == "<PORT>")
                    port = (*responseListCounter);

                else if ((*counter2).first == "<IP>")
                    ip = (*responseListCounter);

                else if ((*counter2).first == "<CONFIGTYPE>")
                    configType = (*responseListCounter);

                else if ((*counter2).first == "<DEFAULTRESPONSE>")
                    defaultResponseCond = (*responseListCounter);
            }
            ((TL1SimSocketAcceptor*)(*iter).second)->setTid(tid);
            ((TL1SimSocketAcceptor*)(*iter).second)->setConfigType(tid, configType);
            ((TL1SimSocketAcceptor*)(*iter).second)->setDefResCond(tid, defaultResponseCond);
            list<string> configList;
            list<string> ::iterator iter1;
            configManager->getAttribute(configType, string("<CONFIGURATION>"), string("<DELAY>"), configList);
            iter1 = configList.begin();
            delay = (*iter1);
            ((TL1SimSocketAcceptor*)(*iter).second)->setDelay(tid, delay);
            ((TL1SimSocketAcceptor*)(*iter).second)->setCommunicationState(tid, connectionState);







        }

        ((TL1SimSocketAcceptor*)(*iter).second)->getPort(port);
        if (!(((TL1SimSocketAcceptor*)(*iter).second))->initialize())
        {

            //cout << "failed to create connection for " << (*counter).first << endl;
        }
        else
        {


            //         cout << "listening for a connection on ip: " << ip << " port: " <<  port << endl;
        }

        /*
        map<string, PropertiesList>::iterator neListIter;
        string ipp,pport;
        cout<<"connection info***********************"<<endl;
        for (iter = m_listAcceptors.begin(); iter != m_listAcceptors.end(); iter++)
          {
            iter->second->getIP(ipp);
            iter->second->getPort(pport);
            cout<<"Acceptor with tid "<<iter->first<<", IP "<<ipp<<", and port "<<pport<<" supports:"<<endl;
            for(neListIter = neList.begin(); neListIter != neList.end(); neListIter++)
              {
         string curtid = (*neListIter).first;
         if(iter->second->supportedTid(curtid))
           cout<<curtid<<endl;
              }
            cout<<endl;
          }
        */
    }


    // find the acceptor

    // and add TIDs to each acceptor

    return true;
}

bool TL1ConnectionManager::stopConnections(void)
{

    map<string, TL1SimSocketAcceptor*>::iterator acceptorIterator;
    for (acceptorIterator = m_listAcceptors.begin(); acceptorIterator != m_listAcceptors.end(); acceptorIterator++)
    {
        TL1SimSocketAcceptor* acceptor = (TL1SimSocketAcceptor*) ((*acceptorIterator).second);
        if (acceptor != NULL)
        {
            acceptor->finalize();
            ACE_OS::sleep(1);
            delete acceptor;
        }

        cout << "closing connection" << endl;

    }
    m_listAcceptors.clear();

    return true;
}

bool TL1ConnectionManager::stopSingleConnection(string &ipPort)
{

    map<string, TL1SimSocketAcceptor*>::iterator acceptorIterator;
    for (acceptorIterator = m_listAcceptors.begin(); acceptorIterator != m_listAcceptors.end(); acceptorIterator++)
    {
        if ( ((*acceptorIterator).first) == ipPort)
        {
            TL1SimSocketAcceptor* acceptor = (TL1SimSocketAcceptor*) ((*acceptorIterator).second);
            if (acceptor != NULL)
            {
                acceptor->finalize();
                m_listAcceptors.erase(acceptorIterator);
                delete acceptor;
            }
            break;
        }

        cout << "closing connection" << endl;

    }

    return true;
}

bool TL1ConnectionManager::sendAutonomous(string& ipPort, string& response)
{
    // synchronize access to the connections
    ConnectionLock lock (m_connectionMutex)
        ;
    map<string, TL1SimSocketAcceptor*>::iterator acceptorIterator;
    acceptorIterator = m_listAcceptors.find(ipPort);

    if (acceptorIterator != m_listAcceptors.end())
    {
        TL1SimSocketAcceptor *acceptor = (*acceptorIterator).second;
        acceptor->sendData(response);
    }

    return true;
}

bool TL1ConnectionManager::setCommunicationState(string& neTid, string& messageTid)
{
    map<string, TL1SimSocketAcceptor*>::iterator acceptorIterator;
    for (acceptorIterator = m_listAcceptors.begin();acceptorIterator != m_listAcceptors.end();acceptorIterator++)
    {
        TL1SimSocketAcceptor *acceptor = (*acceptorIterator).second;
        if (acceptor->supportedTid(neTid))
            acceptor->setCommunicationState(neTid, messageTid);
    }
    if (acceptorIterator != m_listAcceptors.end())
        return true;
    else
        return false;
}

void TL1ConnectionManager::cloneAcceptor (TL1SimSocketAcceptor *acceptor)
{

    unsigned portNumber = 3082;
    string portString = "3082";
    string ip = "172.23.110.75";
    if (acceptor == NULL)
    {}

    cout << "Spawning a new acceptor on port " << portNumber;
    TL1SimSocketAcceptor* newAcceptor = new TL1SimSocketAcceptor(portNumber, ip);
    newAcceptor->setIP(ip);
    //newAcceptor->setTid(acceptor->getTi
    newAcceptor->setPort(portString);
    newAcceptor->initialize();
    /*  newAcceptor->setConfigType(tid, configType);
     newAcceptor->setDefResCond(tid, defaultResponseCond);
     newAcceptor->setCommunicationState(tid, connectionState);*/
}

