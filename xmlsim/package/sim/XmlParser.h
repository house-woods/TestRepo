#include <iostream>
#include <fstream>
#include <ospace/thread.h>
#include <ospace/helper.h>
#include <map>
#include <list>
#include "ConfigManager.h"
using namespace std;

class XmlParser
{
public:
    XmlParser()
    { }
    ;
    ~XmlParser()
    { }
    ;
    bool parseXML(void);
    bool readConfigFile(string filename, string &invalidXMLTag);
    string parseNEConfigurationFiles(void);
    bool validateXmlFormat(string &invalidTag, list<string>::iterator savedPos);

private:
    bool clearXMLList();

protected:
    bool freeXMLMemory(PropertiesList* ptr);

private:

    list<string> xmlConfigList;
};

