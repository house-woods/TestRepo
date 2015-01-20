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

#include <iostream>
#include <stdio.h>
#include <ospace/helper.h> 
//#include "TL1SimSocketAcceptor.h"
#include "TL1ConnectionManager.h"
#include "TL1CommandResolver.h"
#include "XmlParser.h"
#include "ConfigManager.h"
#include "AutonomousScheduler.h"


char f;

void displayProperties(PropertiesList &properties);

int main(int argc, char* argv[])
{
    cout << "Starting up ... " << endl;
    for (int i = 0;i < argc;i++)
    {
        if (string (argv[i]) == "-d")
        {
            cout << "Press enter to start simulation: " << endl;
            f = getchar();
        }
    }

    string configFilename;

    if ((argc > 3) || (argc < 2))
    {
        cout << "usage: " << argv[0] << " <config filename>" << endl;
        exit(0);

    }
    else
    {
        configFilename = argv[1];
    }


    cout << "Loading master configuration file " << configFilename << " ...  " << endl;
    XmlParser parser;
    string invalidXMLTag;
    bool validateXML = parser.readConfigFile(configFilename, invalidXMLTag);
    if (validateXML == false)
    {
        if (invalidXMLTag == "")
            cout << "configuration file cannot be found or is in an invalid XML format." << endl;
        else
            cout << "Problem found with XML format, TAG: " << invalidXMLTag << endl;
        exit(0);
    }
    cout << "done" << endl;

    cout << "Parsing master configuration file " << configFilename << "... ";
    parser.parseXML();
    invalidXMLTag = parser.parseNEConfigurationFiles();
    if (invalidXMLTag != "valid")
    {
        cout << "\nPlease check paths to the configurations files (or file format.)" << endl;
        if (invalidXMLTag != "")
            cout << "Problem with TAG " << invalidXMLTag << endl;

        exit(0);
    }
    cout << "done" << endl;

    string tid;
    PropertiesList neProperties;
    PropertiesList sgProperties;
    PropertiesList propertyList;
    string neType = "<NE>";
    string sgType = "<SERVERGROUP>";

    ConfigManager* configManager = ConfigManager::getInstance();


    AutonomousScheduler* autonomous = AutonomousScheduler::getInstance();



    TL1CommandResolver* commandResolver = TL1CommandResolver::getInstance();



    cout << "Starting up socket listeners ...  " << endl << endl;
    TL1ConnectionManager *manager = TL1ConnectionManager::getInstance();
    manager->startConnections();

    cout << "Initializing scheduler ... ";
    //Creates autonomous queue with all alarms for on demand requests.
    //The queue for the scheduler is initialized each time the scheduler
    // started and may be smaller if not all alarms are turned on.
    if (autonomous->firstInitialize() == false)
    {
        cout << "Problem initializing queue." << endl;
        (0);
    }


    cout << endl << endl;
    cout << "8888P d8 .d88b. .d88b.    8b  8 8888 " << endl;
    cout << "  dP   8 8P  Y8 8P  Y8    8Yb 8 8www " << endl;
    cout << " dP    8 8b  d8 8b  d8    8 dY8 8    " << endl;
    cout << "dP     8 `Y88P' `Y88P'    8  d8 8888 " << endl;
    cout << endl;
    cout << ".d88b. e                 8       w              " << endl;
    cout << "YPwww. w 8d8b.d8b. 8   8 8 .d88 w8ww .d8b. 8d8b " << endl;
    cout << "    d8 8 8P Y8P Y8 8b d8 8 8  8  8   8' .8 8P   " << endl;
    cout << "`Y88P' 8 8   8   8 `Y8P8 8 `Y88  Y8P `Y8P' 8    " << endl;
    cout << "+----------------------------------------------+" << endl;
    cout << "+ Ready to accept socket connections ...       +" << endl;
    cout << "+----------------------------------------------+" << endl;
    cout << "+ For best results:                            +" << endl;
    cout << "+  -Use EMS or SockTest to connect             +" << endl;
    cout << "+  -Avoid telnet as Local echo is un-supported +" << endl;
    cout << "+  -Type HELP for a list of supported commands +" << endl;
    cout << "+ Useful hints:                                +" << endl;
    cout << "+  -Autonomous messages can be generated based +" << endl;
    cout << "+   on a received command. To do this ensure   +" << endl;
    cout << "+   that the <SUPPORTEDAUTONOMOUS> structure   +" << endl;
    cout << "+   is added to the message and the aid of the +" << endl;
    cout << "+   autonomous message is added to this        +" << endl;
    cout << "+   structure; the aid should also be in the   +" << endl;
    cout << "+   <SUPPORTEDAUTONOMOUS> structure for the NE.+" << endl;
    cout << "+  -Type: Command Autonomous Auto to turn on   +" << endl;
    cout << "+   this feature and Command Autonomous Manual +" << endl;
    cout << "+   to turn the feature off.                   +" << endl;
    cout << "+----------------------------------------------+" << endl;
    cout << endl;
    cout << "Enter command: " << endl;
    //  char buffer[2048];
    char character;
    string commandBuffer;
    string origBuffer;
    string response;
    int counter;

    bool validCommand = false;
    bool resolveCommand = false;

    printf("> ");
    while ( (character = getchar()) )
    {
        resolveCommand = false;

        if (character != '\n')
        {
            commandBuffer = commandBuffer + character;
            origBuffer = origBuffer + character;
            continue;
        }

        ConfigManager::toUpper(commandBuffer);
        if (commandBuffer == "QUIT")
        {
            break;
        }
        else if (commandResolver->resolveUserCommand(commandBuffer, response, origBuffer))
        {
            cout << response << endl;
        }
        else
        {
            cout << "unknown command" << endl;
        }

        printf("> ");
        commandBuffer = "";
        origBuffer = "";

    }

    manager->stopConnections();

    delete manager;
    delete autonomous;
    delete configManager;

    /*****/
    // MAKE SURE ALL MEMORY IS CLEARED
    /*****/


}

void displayProperties(PropertiesList &properties)
{
    PropertiesList::iterator counter;
    AttributeValueList::iterator counter2;

    cout << endl;

    for (counter = properties.begin(); counter != properties.end(); counter++)
    {
        cout << (*counter).first << endl;

        for (counter2 = ((*counter).second).begin();counter2 != ((*counter).second).end(); counter2++)
        {
            cout << (*counter2) << endl;
        }

    }

}
