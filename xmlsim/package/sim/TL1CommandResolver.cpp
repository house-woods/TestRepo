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


#include "TL1CommandResolver.h"
#include "TL1ConnectionManager.h"
#include "ConfigManager.h"
#include "AutonomousScheduler.h"
#include "TL1ConnectionManager.h"
#include "XmlParser.h"
#include <map>
#include <string>
#include <list>
#include <stdio.h>

using namespace std;

TL1CommandResolver* TL1CommandResolver::m_commandResolver = NULL;
bool _commandAutonomous = true;
int scheduleDalay = 1000;

TL1CommandResolver* TL1CommandResolver::getInstance()
{
    if (TL1CommandResolver::m_commandResolver == NULL)
        TL1CommandResolver::m_commandResolver = new TL1CommandResolver();

    return TL1CommandResolver::m_commandResolver ;

}


bool TL1CommandResolver::resolveCommand(string &configTid, string &command, list<string> &response)
{
    ConfigManager* configManager = ConfigManager::getInstance();

    string type = "<CONFIGURATION>";

    configManager->getAttribute(configTid, type, command, response);

    if (response.empty())
        return false;

    return true;
}

bool TL1CommandResolver::resolveUserCommand(string &command, string &response, string &origCommand)
{
    TL1ConnectionManager *manager = TL1ConnectionManager::getInstance();
    list<string> atids;
    try
    {
        if (command == "START SCHEDULER")
        {
            AutonomousScheduler* scheduler = AutonomousScheduler::getInstance();

            if (scheduler->start(atids) == true)
                response = "ok\n";
            else
                response = "fail\n";

        }

        else if (command.find("START SCHEDULER") != -1)
        {
            AutonomousScheduler* scheduler = AutonomousScheduler::getInstance();
            int pos1, pos2;

            if (command[14] != '\n') //there are arguments or spaces at end of command
            {
                if (command.find_first_not_of(" ", 16) == ' ')
                {
                    //there are no arguments, but extra spaces
                    response = "invalid command";

                }
                else
                { //there are arguments
                    pos1 = 16;
                    pos2 = command.find_first_of(" \n", 17);
                    string step;

                    while (pos2 != -1)
                    {
                        atids.push_back(command.substr(pos1, pos2 - pos1));
                        pos1 = pos2 + 1;
                        pos2 = command.find_first_of(" ", pos1);
                    }
                    atids.push_back(command.substr(pos1, command.length() - pos1));
                }
            }

            if (scheduler->start(atids) == true)
                response = "ok\n";
            else
                response = "fail\n";

        }
        else if (command == "COMMAND AUTONOMOUS AUTO")
        {
            _commandAutonomous = true;
            response = "ok\n";

        }
        else if (command == "COMMAND AUTONOMOUS MANUAL")
        {
            _commandAutonomous = false;
            response = "ok\n";

        }
        else if (command == "STOP SCHEDULER" || command == "S")
        {
            AutonomousScheduler* scheduler = AutonomousScheduler::getInstance();

            if (scheduler->stop() == true)
                response = "ok\n";
            else
                response = "fail\n";

        }
        else if (command.find("GENERATE") != -1)
        {
            int firstPos = command.find_first_of(" ");
            int endPos = command.find_last_of(" ");
            int length = endPos - firstPos;

            if ((firstPos == -1) && (endPos == -1))
                return false;

            // extract netid
            string neTid = command.substr(firstPos + 1, length - 1);

            // extract message tid
            string messageTid = command.substr(endPos + 1);
            if (messageTid.empty())
                return false;

            AutonomousScheduler* scheduler = AutonomousScheduler::getInstance();
            if (scheduler->onDemandRequest(neTid, messageTid) == true)
                response = "ok\n";
            else
                response = "fail\n";

        }
        else if (command.find("COMMUNICATION") != -1)
        {
            int firstPos = command.find_first_of(" ");
            int endPos = command.find_last_of(" ");
            int length = endPos - firstPos;

            if ((firstPos == -1) && (endPos == -1))
                return false;

            // extract netid
            string neTid = command.substr(firstPos + 1, length - 1);

            // extract message tid
            string messageTid = command.substr(endPos + 1);
            if (messageTid.empty())
                return false;

            if (manager->setCommunicationState(neTid, messageTid) == true)
                response = "ok\n";
            else
                response = "fail\n";

        }
        else if (command.find("UPDATE CONFIGURATION") != -1)
        {
            int firstPos = origCommand.find_last_of(" ");
            if (firstPos == -1)
                return false;

            string filename = origCommand.substr(firstPos + 1);
            string invalidXml;

            XmlParser parser;
            bool validateXML = parser.readConfigFile(filename, invalidXml);
            if (validateXML == false)
            {
                response = "invalid XML format! Problem Tag: " + invalidXml + "\n";
                return true;
            }
            validateXML = parser.parseXML();
            if (validateXML == false)
            {
                response = "XML parser failed!\n";
                return true;
            }

            TL1ConnectionManager *manager = TL1ConnectionManager::getInstance();
            manager->startConnections();

            response = "\nScheduler related changes, will require the scheduler\nto be manually stopped and restarted, for this change to take affect.\n";
        }
        else if (command.find("DELAY") != -1)
        {
            int firstSpace = command.find_first_of(" ");
            int endSpace = command.find_last_of(" ");
            int length = endSpace - firstSpace;
            if ((firstSpace == -1) && (endSpace == -1))
                return false;

            //delay everything
            if (firstSpace == endSpace)
            {
                string delay = command.substr(firstSpace + 1, command.length() - firstSpace - 1);
                scheduleDalay = atoi(delay.c_str());
            }

            //delay specific command
            else
            {
                string tl1command = command.substr(firstSpace + 1, endSpace - firstSpace - 1);

                firstSpace = endSpace;
                string delay = command.substr(firstSpace + 1, command.length() - firstSpace - 1);
                int intDelay = atoi(delay.c_str());
                ConfigManager *configManager = ConfigManager::getInstance();
                if (!configManager->setCommandDelay(tl1command, intDelay))
                    return false;

                //scheduleDalay = atoi(delay.c_str());
            }

            response = "ok\n";


        }

        else if (command == "HELP")
        {
            char buffer[5000];
            sprintf(buffer, "\nQUIT {stop the simulator}\n\nCOMMAND AUTONOMOUS AUTO {will allow the simulator to generate automatic autonomous messages in response to a command} \n\nCOMMAND AUTONOMOUS MANUAL {turn off automatic autonomous responses}\n\nSTART SCHEDULER <AUTONOMOUS TID 1> <AUTONOMOUS TID 2> ... {start the autonomous message scheduler with given autonomous message types turned on. if none given, all are turned on}\n\nSTOP SCHEDULER {stop the autonomous message scheduler}\n\nGENERATE <NE TID> <AUTONOMOUS TID> {on-demand generate an autonomous message}\n\nUPDATE CONFIGURATION <CONFIG XML FILE> {update the existing ne configuration with new configuartion data} \n\nCOMMUNICATION <NE TID> <ENABLE|DISABLE> {force a connection to respond with a DENY or a COMLD} \n\nDELAY <milliseconds> {add a response time delay for all NEs when the autonomous scheduler is running} \n\nDELAY <TL1 COMMAND> <milliseconds> {delay the responses to the given TL1 command}\n\n");
            response = buffer;
        }
        else
            response = "invalid command!\n";
    }
    catch (...)
    {
        return false;
    }

    return true;
}

