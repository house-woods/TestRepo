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


#include "AutonomousScheduler.h"
#include "TL1ConnectionManager.h"


#include <ospace/time.h>
#include <ospace/helper.h>
#include <ace/Service_Object.h>

#define MAX_SCHEDULES  5000

osi_thread* AutonomousScheduler::_scheduleThread = NULL;
AutonomousScheduler* AutonomousScheduler::m_autonomous = NULL;
ScheduleQueue AutonomousScheduler::m_scheduleQueue;
ScheduleQueue AutonomousScheduler::onDemandQueue;
ScheduleQueue::iterator AutonomousScheduler::iter;
list<int> AutonomousScheduler::m_schedulePeriod;

extern int scheduleDalay;

AutonomousScheduler* AutonomousScheduler::getInstance()
{
    if (AutonomousScheduler::m_autonomous == NULL)
        AutonomousScheduler::m_autonomous = new AutonomousScheduler();

    return AutonomousScheduler::m_autonomous ;

}

AutonomousScheduler::AutonomousScheduler()
{
    _scheduleThread = NULL;
}

AutonomousScheduler::~AutonomousScheduler()
{

    try
    {
        finialize();
    }
    catch (...)
    {
        cout << "Exception in AutonomousScheduler::finialize()" << endl;
    }

}

bool AutonomousScheduler::firstInitialize()
{
    const string hostName = "localhost";
    ScheduleEntry* item = NULL;
    map<string, PropertiesList> neList;
    map<string, PropertiesList>::iterator counter;
    map<AttributeName, AttributeValueList>::iterator counter2;
    list<string>::iterator responseListCounter;
    list<string> supportAutonomousList;
    list<string> atids; //blank here
    // initialize the schedule queue


    m_scheduleQueue.clear();


    ConfigManager* configManager = ConfigManager::getInstance();
    configManager->getTcpConfigurations(neList);

    //cout << "Initialize Autonomous Scheduler" << endl;

    bool supportedAutonomousSet = false;
    string supportType;
    string tid;
    string port;
    string ip;

    for (counter = neList.begin(); counter != neList.end(); counter++)
    {
        tid = ((*counter).first);
        supportedAutonomousSet = false;
        for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
        {
            if ((*counter2).first == "<SUPPORTEDAUTONOMOUS>")
            {
                supportedAutonomousSet = true;
            }
            else if ((*counter2).first == "<PORT>")
            {
                responseListCounter = (((*counter2).second).begin());
                port = (*responseListCounter);
            }
            else if ((*counter2).first == "<IP>")
            {
                responseListCounter = (((*counter2).second).begin());
                ip = (*responseListCounter);
            }

            if (supportedAutonomousSet == true)
            {

                //cycles through set autonomous types
                for (responseListCounter = ((*counter2).second).begin(); responseListCounter != ((*counter2).second).end(); responseListCounter++)
                {
                    supportType = (*responseListCounter);
                    configManager->getAutonomousMessages(tid, ip.append(port), supportType, m_scheduleQueue, atids);
                }
                break;
            }
        }


    }

    onDemandQueue = m_scheduleQueue;

    //cout << endl << "the schedule queue consists of the following: " << endl << endl;


    iter = m_scheduleQueue.begin();

    int m_lastPeriod = 200000;
    int m_firstPeriod = 0;
    int period = 0;
    int maxCounter = 0;

    int m_schedule[MAX_SCHEDULES];
    for (iter = m_scheduleQueue.begin();iter != m_scheduleQueue.end();iter++)
    {
        item = (*iter);

        //nice output of schedule queue

        //cout << "NE Tid " << item->neTid << endl;
        //cout << "Tid: " << item->tid << endl;
        //cout << "Port: " << item->port << endl;
        //cout << "Response: " << item->response << endl;
        //cout << "Header: " << item->header << endl << endl;
        //cout << "Initial Delay: " << item->initialDelay << endl;
        //cout << "Period: " << item->schedulePeriod << endl << endl;





        // period = item->schedulePeriod;

        // ensure no duplcates
        /*bool duplicate = false;
        for(int counter2=0; counter2<maxCounter; counter2++)
        {
            if(m_schedule[counter2] == period)
            {
                duplicate = true;
                break;
            }
        }

        if (duplicate == false)
        {
        m_schedule[maxCounter] = period;
        maxCounter++;
        }*/
    }

    // sort list
    bool sort = true;
    int lastCounter = 0;
    int temp;


    /*  while (sort)
      {

    sort = false;
    lastCounter = 0;
         for (int counter2=1; counter2<maxCounter; counter2++)
    {
     if (m_schedule[lastCounter] > m_schedule[counter2])
     {
        temp = m_schedule[lastCounter];
        m_schedule[lastCounter] = m_schedule[counter2];
        m_schedule[counter2] = temp;
        sort = true;
     }
     
     lastCounter++;

    }
      
      }
      
      for (int counter2=0; counter2<maxCounter; counter2++)
        m_schedulePeriod.push_back(m_schedule[counter2]);
      */

    return true;
}

bool AutonomousScheduler::initialize(list<string> &atids)
{
    const string hostName = "localhost";
    ScheduleEntry* item = NULL;
    map<string, PropertiesList> neList;
    map<string, PropertiesList>::iterator counter;
    map<AttributeName, AttributeValueList>::iterator counter2;
    list<string>::iterator responseListCounter;
    list<string> supportAutonomousList;

    // initialize the schedule queue


    m_scheduleQueue.clear();


    ConfigManager* configManager = ConfigManager::getInstance();
    configManager->getTcpConfigurations(neList);

    //cout << "Initialize Autonomous Scheduler" << endl;

    bool supportedAutonomousSet = false;
    string supportType;
    string tid;
    string port;
    string ip;

    for (counter = neList.begin(); counter != neList.end(); counter++)
    {
        tid = ((*counter).first);
        supportedAutonomousSet = false;
        for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
        {
            if ((*counter2).first == "<SUPPORTEDAUTONOMOUS>")
            {
                supportedAutonomousSet = true;
            }
            else if ((*counter2).first == "<PORT>")
            {
                responseListCounter = (((*counter2).second).begin());
                port = (*responseListCounter);
            }
            else if ((*counter2).first == "<IP>")
            {
                responseListCounter = (((*counter2).second).begin());
                ip = (*responseListCounter);
            }

            if (supportedAutonomousSet == true)
            {

                //cycles through set autonomous types
                for (responseListCounter = ((*counter2).second).begin(); responseListCounter != ((*counter2).second).end(); responseListCounter++)
                {
                    supportType = (*responseListCounter);
                    configManager->getAutonomousMessages(tid, ip.append(port), supportType, m_scheduleQueue, atids);
                }
                break;
            }
        }


    }

    //cout << endl << "the schedule queue consists of the following: " << endl << endl;


    //iter = m_scheduleQueue.begin();

    //int m_lastPeriod = 200000;
    //int m_firstPeriod = 0;
    //int period = 0;
    //int maxCounter = 0;

    //    int m_schedule[MAX_SCHEDULES];
    //for (iter = m_scheduleQueue.begin();iter != m_scheduleQueue.end();iter++)
    //{
    //item = (*iter);
    //cout << "NE Tid " << item->neTid << endl;
    //cout << "Tid: " << item->tid << endl;
    //cout << "Port: " << item->port << endl;
    //cout << "Response: " << item->response << endl;
    //cout << "Header: " << item->header << endl << endl;
    //cout << "Initial Delay: " << item->initialDelay << endl;
    //cout << "Period: " << item->schedulePeriod << endl << endl;

    // period = item->schedulePeriod;

    // ensure no duplcates
    //bool duplicate = false;
    //for(int counter2=0; counter2<maxCounter; counter2++)
    //{
    //if(m_schedule[counter2] == period)
    //  {
    //  duplicate = true;
    //    break;
    //  }
    //}

    //if (duplicate == false)
    //{
    // m_schedule[maxCounter] = period;
    //maxCounter++;
    //  }
    //}

    // sort list
    bool sort = true;
    int lastCounter = 0;
    int temp;

    /*  while (sort)
      {

    sort = false;
    lastCounter = 0;
         for (int counter2=1; counter2<maxCounter; counter2++)
    {
     if (m_schedule[lastCounter] > m_schedule[counter2])
     {
        temp = m_schedule[lastCounter];
        m_schedule[lastCounter] = m_schedule[counter2];
        m_schedule[counter2] = temp;
        sort = true;
     }
     
     lastCounter++;

    }
      
      }
      
      for (int counter2=0; counter2<maxCounter; counter2++)
        m_schedulePeriod.push_back(m_schedule[counter2]);
      */

    return true;
}

bool AutonomousScheduler::onDemandRequest(string neTid, string tid)
{
    bool found = false;
    ScheduleEntry* item = NULL;
    string message;
    string dateString;
    string timeString;
    string itemNeTid;
    string messageTid;


    for (iter = onDemandQueue.begin();iter != onDemandQueue.end();iter++)
    {
        item = (*iter);
        itemNeTid = item->neTid;
        ConfigManager::toUpper(itemNeTid);
        messageTid = item->tid;
        ConfigManager::toUpper(messageTid);

        if ( (itemNeTid == neTid) && (messageTid == tid) )
        {
            found = true;
            break;
        }
    }

    if (found)
        found = compileResponse(item);


    return found;
}

bool AutonomousScheduler::finialize(void)
{
    try
    {
        if (_scheduleThread == NULL)
            return true;

        _scheduleThread->terminate();
        while (_scheduleThread->exists())
        {

            ACE_OS::sleep(10);

        }
        delete _scheduleThread;
        _scheduleThread = NULL;
    }
    catch (...)
    {
        if (_scheduleThread)
            delete _scheduleThread;

        _scheduleThread = NULL;
        return false;
    }
    return true;
}

bool AutonomousScheduler::start(list<string> &atids)
{

    if (_scheduleThread == NULL)
    {
        // start the scheduler thread
        try
        {
            AutonomousScheduler *scheduler = AutonomousScheduler::getInstance();
            bool success = scheduler->initialize(atids);
            if (success == false)
                return success;
            _scheduleThread = new osi_thread(AutonomousScheduler::scheduleLoop, (void*) this, 0, true);
        }
        catch (osi_thread_toolkit_error& err)
        {
            cout << endl << "could not start scheduler: " << err.code();
            return false;
        }

    }

    return true;
}

bool AutonomousScheduler::stop(void)
{

    finialize();

    return true;
}

bool AutonomousScheduler::compileResponse(ScheduleEntry *entry)
{
    string header;
    string dateString;
    string timeString;

    // send message
    string port = entry->port;

    // add date to the header
    osi_date todayDate = osi_date::today();
    osi_time todayTime = osi_time::now();

    TL1ConnectionManager* manager = TL1ConnectionManager::getInstance();

    TL1SimSocketAcceptor::GetTimeInAgentFormat(todayDate, todayTime, dateString, timeString);

    header = entry->neTid + " " + dateString + " " + timeString + "\r\n";

    if (timeString.length() == 8)
    {
        timeString[2] = '-';
        timeString[5] = '-';
    }

    int pos = timeString.find_last_of("-");
    string timeStringNoSeconds = timeString.substr(0, pos);


    const string &response = entry->response;
    // split up responses based on semicolons
    int startOfOneEvent = 0;
    string oneEvent;
    bool lastRunThrough = false;

    while (true)
    {
        // each event should end with a semicolon
        int endOfOneEvent = response.find (";", startOfOneEvent);
        if (endOfOneEvent == string::npos)
            lastRunThrough = true;

        if (!lastRunThrough)
            // extract out a single event
            oneEvent = response.substr (startOfOneEvent, endOfOneEvent - startOfOneEvent);
        else
            oneEvent = response.substr(startOfOneEvent);

        startOfOneEvent = endOfOneEvent + 1;

        // replace tags
        int pos = oneEvent.find ("[HEADER]");
        if (pos != string::npos)
            oneEvent.replace (pos, 8, header);

        pos = 0;
        while ((pos = oneEvent.find("[DATE]", pos)) != string::npos)
            // chop off the year
            oneEvent.replace (pos, 6, dateString.substr (3, 5));

        pos = 0;
        while ((pos = oneEvent.find("[TIME]", pos)) != string::npos)
            oneEvent.replace (pos, 6, timeString);

        pos = 0;
        while ((pos = oneEvent.find("[TIME-NO-SECONDS]", pos)) != string::npos)
            oneEvent.replace (pos, 17, timeStringNoSeconds);

        oneEvent += "\r\n;" ;

        try
        {
            manager->sendAutonomous(port, oneEvent);
        }
        catch (...)
        {
            cout << "exception caught sending autonomous" << endl;
            return false;
        }

        if (lastRunThrough)
            break;
    }


    return true;
}

void* AutonomousScheduler::scheduleLoop(void* arg)
{

    list<int>::iterator iter2;
    int period;
    iter2 = m_schedulePeriod.begin();
    int lastSchedulePeriod = 0;
    int delayValue = 0;
    char buffer[100];
    string periodString;
    string message = "sample";
    string port;
    string dateString, timeString;
    string header;
    bool initialize = true;

    char testBuffer[100];

    AutonomousScheduler* autonomous = AutonomousScheduler::getInstance();


    osi_time_and_date todayTime = osi_time_and_date::now();
    osi_time_period todayTimePeriod = todayTime.utc_time_and_date();
    double seconds = todayTimePeriod.to_seconds();
    double microseconds = seconds * 1000;

    try
    {

        while (1)
        {
            // I keep track of the time by adding the delay value,in a loop,
            // to the microseconds. When I used to use osi time I noticed a problem
            // could occur if other applications are calling ospace time. This
            // would over a period of time, affect the schedule intervals which would knock an alarm
            //and clear schedule out-of-sync.

            for (iter = m_scheduleQueue.begin(); iter != m_scheduleQueue.end(); iter++)
            {
                ScheduleEntry *entry = (*iter);
                if ( (entry->launchSchedule == true) || (initialize == true) )
                {
                    double scheduleTime = 0;
                    period = entry->schedulePeriod;
                    scheduleTime = microseconds + period;


                    // adds an inital delay to some schedules (alarm clears)
                    // this will ensure alarm and clears will not occur at the same time.
                    if (initialize == true)
                    {
                        scheduleTime = scheduleTime + entry->initialDelay;
                    }

                    entry->mseconds = scheduleTime;

                    entry->launchSchedule = false;

                }
            }

            if (initialize)
                cout << "Scheduler Delay has been set to: " << scheduleDalay << endl;

            // the lowest possible value I can sleep for
            osi_this_thread::milli_sleep(scheduleDalay);

            microseconds = microseconds + scheduleDalay;

            string cur;
            for (iter = m_scheduleQueue.begin(); iter != m_scheduleQueue.end(); iter++)
            {
                ScheduleEntry *entry = (*iter);
                if (autonomous != NULL)
                {
                    if ( (entry->mseconds <= microseconds) && (entry->launchSchedule == false) )
                    {
                        entry->launchSchedule = true;
                        cur = entry->tid;
                        //cout<< cur << " for " <<entry->neTid<< " sent." <<endl;
                        autonomous->compileResponse(entry);

                    }
                }
            }
            // this flag is used to enforce a delay for clears
            initialize = false;

        }

    }
    catch (...)
    {
        cout << "exception occurred in scheduler - please stop then restart scheduler" << endl;
    }

    return 0;
}
