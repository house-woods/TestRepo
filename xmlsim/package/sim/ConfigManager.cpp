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


#include "ConfigManager.h"
#include <stdlib.h>

ConfigManager* ConfigManager::m_configManager = NULL;


ConfigManager* ConfigManager::getInstance()
{
    if (ConfigManager::m_configManager == NULL)
        ConfigManager::m_configManager = new ConfigManager();

    return ConfigManager::m_configManager ;

}

ConfigManager::~ConfigManager()
{
    finialize();
}

bool ConfigManager::finialize(void)
{
    try
    {
        /*freeConfigurationMemory(m_neList);
        freeConfigurationMemory(m_serverGroupList);
        freeConfigurationMemory(m_autonomousList);
        freeConfigurationMemory(m_configurationList);
        freeConfigurationMemory(m_neConfigurations);*/
    }
    catch (...)
    {
        cout << "Exception: ConfigManager::finialize(void)" << endl;
    }

    return true;
}

bool ConfigManager::setCommandDelay(string &command, int &delay)
{
    map<string, int> *ptr = NULL;
    ptr = &m_commandDelays;
    map<string, int>::iterator delayIter;

    if (delay == 0)
    {
        ptr->erase(command);
        return true;
    }
    else
    {
        (*ptr)[command] = delay;
        return true;
    }

    return false;
}

bool ConfigManager::getCommandDelay(string &command, int &delay)
{
    try
    {
        map<string, int>::const_iterator iter = m_commandDelays.find(command);
        if (iter != m_commandDelays.end())
        {
            delay = (*iter).second;
            return true;
        }
        delay = 0;
    }

    catch (...)
    {
        cout << "Exception: ConfigManager::getCommandDelay" << endl;
    }

    return false;
}


bool ConfigManager::getAttribute(string key, string name, list<string> &value, string type)
{
    map<string, PropertiesList> *ptr = NULL;
    list<string>::const_iterator responseListCounter;

    if (type == "<SERVERGROUP>")
        ptr = &m_serverGroupList;
    else if (type == "<NE>")
        ptr = &m_neList;
    else if (type == "<AUTONOMOUS>")
        ptr = &m_autonomousList;
    else if (type == "<CONFIGPATH>")
        ptr = &m_neConfigurations;
    else if (type == "<CONFIGURATION>")
        ptr = &m_configurationList;
    else
        return false;

    if (ptr == NULL)
        return false;

    toUpper(name);
    map<string, PropertiesList>::const_iterator propIter = ptr->find(key);

    if (propIter != ptr->end())
    {
        map<AttributeName, AttributeValueList>::const_iterator attrIter = propIter->second.find(name);
        if (attrIter != propIter->second.end())
        {
            for (responseListCounter = ((*attrIter).second).begin(); responseListCounter != ((*attrIter).second).end(); responseListCounter++)
                value.push_back((*responseListCounter));
        }
    }

    return true;
}

bool ConfigManager::getAttributes(string key, map<AttributeName, AttributeValueList> &properties, string type)
{
    map<string, PropertiesList> *ptr = NULL;

    if (type == "<SERVERGROUP>")
        ptr = &m_serverGroupList;
    else if (type == "<NE>")
        ptr = &m_neList;
    else if (type == "<AUTONOMOUS>")
        ptr = &m_autonomousList;
    else if (type == "<CONFIGPATH>")
        ptr = &m_neConfigurations;
    else if (type == "<CONFIGURATION>")
        ptr = &m_configurationList;
    else
        return false;

    if (ptr == NULL)
        return false;

    map<string, PropertiesList>::const_iterator propIter = ptr->find(key);
    if (propIter != ptr->end())
    {
        properties = propIter->second;
        return true;
    }

    return false;
}

bool ConfigManager::getNEConfigurationFiles(map<string, string> &nelist)
{
    bool success = false;
    map<string, PropertiesList>::iterator propIter;
    map<AttributeName, AttributeValueList>::iterator attrIter;
    list<string>::iterator responseListIter;

    for (propIter = m_neConfigurations.begin(); propIter != m_neConfigurations.end(); propIter++)
    {
        attrIter = ((*propIter).second).find("<PATH>");
        if (attrIter != ((*propIter).second).end())
        {
            for (responseListIter = ((*attrIter).second).begin(); responseListIter != ((*attrIter).second).end(); responseListIter++)
            {
                success = true;
                nelist.insert(pair<string, string> (((*propIter).first), (*responseListIter)));
            }
        }
    }

    return success;
}

bool ConfigManager::getTcpConfigurations(map<string, PropertiesList> &neConfig)
{
    bool success = false;
    map<string, PropertiesList>::iterator counter;
    map<AttributeName, AttributeValueList>::iterator counter2;
    list<string>::iterator responseListCounter;

    neConfig = m_neList;


    return success;
}


bool ConfigManager::setAttributes(string key, map<AttributeName, AttributeValueList> &properties, string type)
{
    map<string, PropertiesList> *ptr = NULL;
    map<string, PropertiesList>::iterator counter;
    list<string>::iterator responseListCounter;
    map<AttributeName, AttributeValueList>::iterator counter2;
    map<AttributeName, AttributeValueList>::iterator counter3;


    if (type == "<SERVERGROUP>")
        ptr = &m_serverGroupList;
    else if (type == "<NE>")
        ptr = &m_neList;
    else if (type == "<AUTONOMOUS>")
        ptr = &m_autonomousList;
    else if (type == "<CONFIGPATH>")
        ptr = &m_neConfigurations;
    else if (type == "<CONFIGURATION>")
        ptr = &m_configurationList;

    else
        return false;

    if (ptr == NULL)
        return false;

    counter = ptr->find(key);
    if (counter != ptr->end())
    {

        for (counter2 = properties.begin(); counter2 != properties.end(); counter2++)
        {
            string name = (*counter2).first;
            counter3 = ((*counter).second).find(name);




            //nice output of data structures
            // cout<<"attributeName:"<<name<<endl;
            //for(list<string>::iterator debugIter = ((*counter3).second).begin();debugIter!=((*counter3).second).end(); debugIter++)
            // cout<<(*debugIter)<<endl;

            // cout<<"counter3.first: "<<(*counter3).first<<" counter3.second.begin"<< (*((*counter3).second).begin())<<endl;
            //end output



            if (counter3 != (((*counter).second).end()))
            {

                ((*counter).second).erase(counter3);
                ((*counter).second).insert(pair<string, AttributeValueList> (name, (*counter2).second));
            }
        }

    }
    else
        ptr->insert(pair<string, PropertiesList> (key, properties));


    return true;
}


bool ConfigManager::cleanupConfiguartion(void)
{
    // cleanup memory for all configurations

    // TODO

    return true;
}


bool ConfigManager::getAttribute(string &tid, string type, string attributeName, list<string> &attributeValueList)
{
    map<string, PropertiesList>* ptr;
    map<string, PropertiesList>::iterator counter;
    map<AttributeName, AttributeValueList>* ptr2;
    map<AttributeName, AttributeValueList>::iterator counter2;
    list<string>::iterator responseListCounter;


    if (type == "<SERVERGROUP>")
        ptr = &m_serverGroupList;
    else if (type == "<NE>")
        ptr = &m_neList;
    else if (type == "<AUTONOMOUS>")
        ptr = &m_autonomousList;
    else if (type == "<CONFIGPATH>")
        ptr = &m_neConfigurations;
    else if (type == "<CONFIGURATION>")
        ptr = &m_configurationList;

    bool success = false;

    if (ptr == NULL)
        return false;

    counter = ptr->find(tid);
    if (counter != ptr->end())
    {
        for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
        {

            string source = (*counter2).first;
            toUpper(source);
            toUpper(attributeName);

            if ((*counter2).first == attributeName)
            {
                for (responseListCounter = ((*counter2).second).begin(); responseListCounter != ((*counter2).second).end(); responseListCounter++)
                    attributeValueList.push_back((*responseListCounter));

                break;

            }
        }
    }


    return true;
}

bool ConfigManager::getAutonomousMessages(string &tid, string port, string &supportedType, ScheduleQueue &scheduleQueue, list<string> &atids)
{
    map<string, PropertiesList>::iterator counter;
    map<AttributeName, AttributeValueList>::iterator counter2;
    list<string>::iterator responseListCounter;
    list<string>::iterator atidsIter;
    string tempStr;

    if (atids.empty() == false) //there are arguments to start scheduler
    {
        for (counter = m_autonomousList.begin(); counter != m_autonomousList.end(); counter++)
        {
            atidsIter = atids.begin();
            tempStr = (*counter).first;
            toUpper(tempStr);
            while (atidsIter != atids.end() && (*atidsIter) != tempStr)
            {
                //cout<<(*atidsIter)<<endl;
                atidsIter++;
            }

            //if(atidsIter!=atids.end())
            //cout<<(*atidsIter)<<endl;
            if (atidsIter != atids.end())
            {
                tempStr = (*counter).first;
                toUpper(tempStr);
                //cout<<"atidsIter:" <<(*atidsIter)<< " counter: "<<tempStr<<endl;
                if ((*atidsIter) == tempStr )
                {
                    if (((*counter).first) == supportedType)
                    {
                        ScheduleEntry *scheduleEntry = new ScheduleEntry;
                        scheduleEntry->tid = ((*counter).first);
                        scheduleEntry->neTid = tid;
                        scheduleEntry->port = port;
                        scheduleEntry->mseconds = 0;
                        scheduleEntry->launchSchedule = true;

                        for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
                        {
                            // only add a schedule entry for those that are supported
                            // for a specified NE
                            for (responseListCounter = ((*counter2).second).begin(); responseListCounter != ((*counter2).second).end(); responseListCounter++)
                            {

                                if (((*counter2).first) == string("<REPORT>"))
                                    scheduleEntry->response = scheduleEntry->response + '\n' + (*responseListCounter);
                                else if (((*counter2).first) == string("<SCHEDULE>"))
                                    scheduleEntry->schedulePeriod = atoi(((*responseListCounter).c_str()));
                                else if (((*counter2).first) == string("<HEADER>"))
                                    scheduleEntry->header = (*responseListCounter);
                                else if (((*counter2).first) == string("<TYPE>"))
                                    scheduleEntry->type = (*responseListCounter);
                                else if (((*counter2).first) == string("<INITIALDELAY>"))
                                    scheduleEntry->initialDelay = atoi(((*responseListCounter).c_str()));
                            }
                        }
                        scheduleQueue.push_back(scheduleEntry);
                    }
                }
            }
        }
    }




    else  // there are no arguments to the start scheduler command
    {
        for (counter = m_autonomousList.begin(); counter != m_autonomousList.end(); counter++)
        {
            if ((*counter).first == supportedType)
            {
                ScheduleEntry *scheduleEntry = new ScheduleEntry;
                scheduleEntry->tid = ((*counter).first);
                scheduleEntry->neTid = tid;
                scheduleEntry->port = port;
                scheduleEntry->mseconds = 0;
                scheduleEntry->launchSchedule = true;

                for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
                {

                    // only add a schedule entry for those that are supported
                    // for a specified NE

                    for (responseListCounter = ((*counter2).second).begin(); responseListCounter != ((*counter2).second).end(); responseListCounter++)
                    {

                        if (((*counter2).first) == string("<REPORT>"))
                            scheduleEntry->response = scheduleEntry->response + '\n' + (*responseListCounter);
                        else if (((*counter2).first) == string("<SCHEDULE>"))
                            scheduleEntry->schedulePeriod = atoi(((*responseListCounter).c_str()));
                        else if (((*counter2).first) == string("<HEADER>"))
                            scheduleEntry->header = (*responseListCounter);
                        else if (((*counter2).first) == string("<TYPE>"))
                            scheduleEntry->type = (*responseListCounter);
                        else if (((*counter2).first) == string("<INITIALDELAY>"))
                            scheduleEntry->initialDelay = atoi(((*responseListCounter).c_str()));

                    }

                }
                scheduleQueue.push_back(scheduleEntry);
            }
        }
    }
    return true;
}

bool ConfigManager::freeConfigurationMemory(map<string, PropertiesList>& configData)
{

    bool success = false;
    map<string, PropertiesList>::iterator counter;
    map<AttributeName, AttributeValueList>::iterator iter2;
    list<string>::iterator responseListCounter;
    AttributeValueList *responseList = NULL;
    map<AttributeName, AttributeValueList> *propertyList = NULL;

    for (counter = configData.begin(); counter != configData.end(); counter++)
    {
        for (iter2 = ((*counter).second).begin();iter2 != ((*counter).second).end(); iter2++)
        {
            for (responseListCounter = ((*iter2).second).begin(); responseListCounter != ((*iter2).second).end(); responseListCounter++)
            {
                ((*iter2).second).erase(responseListCounter);
            }
            ((*iter2).second).clear();
            configData.erase(counter);

        }

    }

    return true;
}


void ConfigManager::toUpper(string &name)
{
    int len = name.length();
    int counter = 0;
    for (counter = 0; counter < len;counter++)
    {
        char character = name[counter];
        name[counter] = toupper(character);
    }


}
