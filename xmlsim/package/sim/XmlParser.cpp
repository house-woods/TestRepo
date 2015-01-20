/***********************************************************************************************
* COPYRIGHT $Date:   June 13 2002 15:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* * Name: Socket Acceptor
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

#include "XmlParser.h"
#include "BaseException.h"
#include <stdio.h>



bool XmlParser::readConfigFile(string fileName, string &invalidXMLTag)
{
    list<string>::iterator iteratorXml;
    char line[2048];
    int counter = 0;
    string tag;
    string response;

    ifstream* configFile = NULL;
    map<AttributeName, AttributeValueList> *ptr;


    ConfigManager* configManager = ConfigManager::getInstance();

    bool tagExists = false;

    configFile = new ifstream();
    configFile->open(fileName.c_str(), ios::in);
    configFile->setf(ios::skipws);

    try
    {

        if ((*configFile) == false)
        {
            //cout<<"Throw1\n";
            throw BaseException();
        }

        iteratorXml = xmlConfigList.end();
        iteratorXml--;

        while ( (*configFile) )
        {

            configFile->getline(line, 2048);
            int stringLength = strlen(line);
            int xmlLineStart = 0;

            for (counter = 0; counter < stringLength; counter++)
            {

                if ( (line[counter] != ' ') && (line[counter] != '\t') )
                {
                    xmlLineStart = counter;

                    /* if (line[counter]  == '*')
                     {
                         line[counter] = ' ';
                         firstAstrieskFound = true;
                     }
                     else*/
                    break;
                }
                /* else if (line[counter]  == '*')
                 {
                     line[counter] = ' ';
                     firstAstrieskFound = true;
                 }*/
                else if ((line[counter] != ' ') && (line[counter] != '\t'))
                {

                    break;
                }

            }


            int lastNonSpaceChar = 0;
            bool setEOL = false;

            for (counter = xmlLineStart; counter < stringLength; counter++)
            {
                // remove spaces after the tag
                if ((line[counter] == '>'))
                {
                    setEOL = false;
                    line[counter + 1] = '\0';
                    break;
                }


                // ignore comments
                else if ((line[counter] == '/') && (line[counter + 1] == '/'))
                {
                    //   setEOL = false;
                    line[counter] = '\0';
                    break;
                }

                else if ((line[counter] != ' ') && (line[counter] != '\t'))
                {
                    setEOL = true;
                    lastNonSpaceChar = counter;
                }
            }

            if (setEOL == true)
            {
                line[lastNonSpaceChar + 1] = '\0';
            }

            string textLine = &line[xmlLineStart];
            // change [SPC] to a space character
            int pos3 = -1;
            pos3 = textLine.find("[SPC]");
            string part1, part2;
            while (pos3 != -1)
            {
                part1 = textLine.substr(0, pos3);
                part2 = textLine.substr(pos3 + 5);
                textLine = part1 + " " + part2;
                pos3 = textLine.find("[SPC]");
            }

            if ( (textLine.find("<") != -1) && (textLine.find(">") != -1) )
                ConfigManager::toUpper(textLine);

            xmlConfigList.push_back(textLine);



        }

        iteratorXml++;
        if (validateXmlFormat(invalidXMLTag, iteratorXml) == false)
        {
            invalidXMLTag = invalidXMLTag + " in file " + fileName;
            configFile->close();
            //cout << "Throw2\n";
            throw BaseException();
        }



    }
    catch (...)
    {

        delete configFile;
        configFile = NULL;
        return false;

    }

    configFile->close();
    delete configFile;
    configFile = NULL;

    return true;
}

bool XmlParser::parseXML(void)
{

    bool setKey = false;
    bool typeIdentified = false;
    bool success = true;
    bool parsingNEFile = false;
    bool supportedAutonomous = false;

    string type;
    string key;
    string attributeName;
    string attributeValue;
    PropertiesList* ptr = NULL;
    AttributeValueList* attributeValueList = NULL;

    ConfigManager* configManager = ConfigManager::getInstance();

    list<string>::iterator iteratorXml;
    for (iteratorXml = xmlConfigList.begin(); iteratorXml != xmlConfigList.end(); iteratorXml++)
    {
        if ( ((*iteratorXml) == "<NE>") || ((*iteratorXml) == "<CONFIGPATH>") || ((*iteratorXml) == "<SERVERGROUP>") ||
             ((*iteratorXml) == "<AUTONOMOUS>") || ((*iteratorXml) == "<CONFIGURATION>") )
        {

            if ((*iteratorXml) == "<CONFIGURATION>")
                parsingNEFile = true;
            else
                parsingNEFile = false;

            ptr = new PropertiesList();
            type = (*iteratorXml);
            typeIdentified = true;
            continue;
        }
        else if ( ((*iteratorXml) == "</NE>") || ((*iteratorXml) == "</CONFIGPATH>") || ((*iteratorXml) == "</SERVERGROUP>") ||
                  ((*iteratorXml) == "</AUTONOMOUS>") || ((*iteratorXml) == "</CONFIGURATION>") )
        {
            //cout<<"key: "<<key<<" type: " <<type<<endl;
            configManager->setAttributes(key, *ptr, type);

            freeXMLMemory(ptr);


            typeIdentified = false;
            continue;
        }


        if (typeIdentified == false)
        {
            // success = false;
            continue;
        }

        if (setKey)
        {
            key = (*iteratorXml);
            setKey = false;
            continue;
        }

        if ( ((*iteratorXml) == "<SUPPORTEDAUTONOMOUS>") && (parsingNEFile == true))
        {
            supportedAutonomous = true;
            //  continue;
        }
        else if ( ((*iteratorXml) == "</SUPPORTEDAUTONOMOUS>") && (parsingNEFile == true))
        {
            supportedAutonomous = false;
            //  continue;
        }

        if ( ((*iteratorXml) == "<TID>") || ((*iteratorXml) == "<AID>") )
        {
            setKey = true;
            continue;
        }

        else if ( (((*iteratorXml).find("<") != -1) && ((*iteratorXml).find("</") == -1)) && (supportedAutonomous == false))
        {
            attributeName = (*iteratorXml);
            //cout<<"attributeName " <<attributeName<<endl;
            PropertiesList::iterator iter = ptr->find(attributeName);
            if (iter == ptr->end())
            {
                //        if(parsingNEFile)
                //  cout<<"adding attributeName " <<attributeName<<" for type "<<type<<endl;

                attributeValueList = new AttributeValueList();
                ptr->insert(pair<string, AttributeValueList>(attributeName, *attributeValueList));
            }
            continue;
        }
        else if ( (((*iteratorXml).find("</") == -1) && (!attributeName.empty()) && (ptr != NULL)) || ((supportedAutonomous == true) && (parsingNEFile == true)))
        {
            attributeValue = (*iteratorXml);

            PropertiesList::iterator iter = ptr->find(attributeName);
            if (iter != ptr->end())
            {
                //if(parsingNEFile)
                //    cout<<attributeValue<<endl;


                //  attributeValueList = new AttributeValueList();
                AttributeValueList *attrList = &((*iter).second);
                if (parsingNEFile)
                    attrList->push_back(attributeValue);
                else
                    attrList->push_back(attributeValue);
                // ptr->insert(attributeName, *attributeValueList);

                attributeValueList = NULL;
            }
            if (parsingNEFile == true)
                ;
            // cout<<"--------------------------------------\n-------------------\n"<<attributeValue<<endl;
            // else
            // {
            //   ((*iter).second).push_back(attributeValue);
            //}

        }


    }



    return success;
}

string XmlParser::parseNEConfigurationFiles(void)
{
    map<string, string> list;
    map<string, string>::iterator counter;
    string invalidTag;
    ConfigManager* configManager = ConfigManager::getInstance();
    bool success = configManager->getNEConfigurationFiles(list);

    // causes a crash
    // clearXMLList();

    if (success)
    {
        cout << "\nValidating NE Config files" << endl;
        for (counter = list.begin(); counter != list.end(); counter++)
        {
            //cout<<"Current file: "<<(*counter).second<<endl;
            cout << ".";
            if (readConfigFile((*counter).second, invalidTag) == false)
            {
                return invalidTag;
            }
        }
        cout << "\nParsing XML...\n";
        parseXML();
        cout << "Done parsing XML.\n";
    }


    return "valid";
}

bool XmlParser::freeXMLMemory(PropertiesList* ptr)
{
    try
    {
        map<AttributeName, AttributeValueList>::iterator propertyIterator;
        AttributeValueList* responseListCounter;
        propertyIterator = ptr->begin();
        responseListCounter = &((*propertyIterator).second);
        if (responseListCounter != NULL)
            delete responseListCounter;

        // causes a crash
        //  delete ptr;
        ptr = NULL;
    }
    catch (...)
    {
        ptr = NULL;
        return false;
    }

    return true;
}

bool XmlParser::clearXMLList(void)
{
    list<string>::iterator iter;
    for (iter = xmlConfigList.begin(); iter != xmlConfigList.end(); iter++)
        xmlConfigList.erase((iter));

    return true;
}

bool XmlParser::validateXmlFormat(string &invalidTag, list<string>::iterator savedPos)
{
    list<string>::iterator iteratorXml;
    list<string>::iterator iteratorXml2;
    bool validFormat = true;
    string attributeName, endTag, beginTag;

    if (xmlConfigList.empty() == true)
    {
        savedPos = xmlConfigList.begin();
    }

    for (iteratorXml = savedPos; iteratorXml != xmlConfigList.end(); iteratorXml++)
    {
        // validate headers
        if ( ((*iteratorXml).find("<?") != -1) && ((*iteratorXml).find("?>") != -1) )
            continue;

        // checks for bad closing tag
        if ( ((*iteratorXml).find("<") != -1) && ((*iteratorXml).find("</") == -1) )
        {
            attributeName = (*iteratorXml);

            endTag = "</" + attributeName.substr(1);
            for (iteratorXml2 = savedPos; iteratorXml2 != xmlConfigList.end();iteratorXml2++)
            {
                if ((*iteratorXml2).find(endTag) != -1)
                {
                    validFormat = true;
                    break;
                }
                else
                {
                    invalidTag = endTag;
                    validFormat = false;
                }
            }
        }

        if (validFormat == false)
            break;

        // checks for closing tag without an opening tag
        if ( (*iteratorXml).find("</") != -1)
        {
            attributeName = (*iteratorXml);
            beginTag = "<" + attributeName.substr(2);
            for (iteratorXml2 = iteratorXml; iteratorXml2 != savedPos; iteratorXml2--)
            {
                if ((*iteratorXml2).find(beginTag) != -1)
                {
                    validFormat = true;
                    break;
                }
                else
                {
                    invalidTag = (*iteratorXml);
                    validFormat = false;
                }
            }
        }

        if (validFormat == false)
            break;

        if (validFormat == false)
            break;

    }

    return validFormat;
}
