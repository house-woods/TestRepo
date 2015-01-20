
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:26  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: BaseNameResolver
* Type: C++ 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseNameResolver.cpp  $
* Version: $Revision:   1.6  $
* Description: 
* Resolve Names. 
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/BaseNameResolver.cpv  $
   
      Rev 1.6   Oct 27 1999 14:46:26   dtchalak
   Query Interface updates.
   
      Rev 1.5.1.0   Sep 20 1999 16:46:04   dtchalak
   Just to create a branch.
   
      Rev 1.5   02 Sep 1999 15:53:14   dtchalak
   New interface support added.
   
      Rev 1.4   21 Jul 1999 10:37:52   dtchalak
   Multiple Ne support included.
   
      Rev 1.3   14 Jun 1999 16:28:08   dtchalak
   Updates related to new CM.idl and other details.
   
      Rev 1.2   21 May 1999 09:56:34   dtchalak
    
   
      Rev 1.1   04 May 1999 19:15:40   dtchalak
   Comments added.
**********************************************************************************************/

#include "BaseNameResolver.h"
#include "FileReader.h"
#include "tutil.h"
#include "sysErrLog.h"

extern const char* sourceName;

//#define MAPDELIM "="

using namespace ELogger;

using namespace EMS_FW;

using namespace std;

const string baseNameResolverName = "BaseNameResolver";


const string& BaseNameResolver::getClassName() const
{
    return baseNameResolverName;
}

bool BaseNameResolver::initialize(const string& fName)
{
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "BaseNameResolver", "initialize", __LINE__, TRACE3, "Method Begin file=%s", fName.c_str());
    FileReader fr(fName.c_str());
    if (fr.bad())
    {
        ErrorLogger::
        logVarError(sourceName, getClassName().c_str(), "initialize", __LINE__,
                    CRITICAL, "Unable to open->%s", fName.c_str());
        return false;
    }
    string tmp;
    string key;
    string value;
    while (fr.getLine(tmp))
    {
        if (!Tutil::extractKeyValue(MAPDELIM.c_str(), tmp, key, value))
        {
            ErrorLogger::
            logVarError(sourceName, getClassName().c_str(), "initialize", __LINE__,
                        CRITICAL, "Unable to extract Key Value from ->%s", tmp.c_str());
            continue;
        }
        _nameMap[key.c_str()] = value.c_str();
    }
    if (ErrorLogger::isTraceListOn())
        ErrorLogger::logVarError(sourceName, "BaseNameResolver", "initialize", __LINE__, TRACE3, "Method finished OK file=%s", fName.c_str());
    return true;
}

string BaseNameResolver::resolveName(const string& moc) const
{
    maptype::const_iterator p = _nameMap.find(moc);
    if (p == _nameMap.end()) // name is not in a map
        throw UnresolvedNameEx(moc);
    return string((*p).second);
}


bool BaseNameResolver::checkMap(const list<string>& mapList, string& missingStrings) const
{
    list<string>::const_iterator p;
    missingStrings = "MissingStrings in a Map->";
    bool res = true;
    for (p = mapList.begin(); p != mapList.end(); p++)
    {
        try
        {
            resolveName(*p);
        }
        catch (UnresolvedNameEx& name)
        {
            missingStrings += *p;
            missingStrings += " ";
            res = false;
        }
    }
    return res;
}


void BaseNameResolver::print() const
{
    maptype::const_iterator p = _nameMap.begin();
    string key;
    string value;
    while (p != _nameMap.end())
    {
        key = string((*p).first);
        value = string((*p).second);
        ErrorLogger::logVarError(sourceName, "BaseNameResolver", "print", __LINE__, TRACE3, "Key [%s]  Value [%s]", key.c_str(), value.c_str());
        p++;
    }
}


void BaseNameResolver::first()
{
    _p = _nameMap.begin();
}

bool BaseNameResolver::more()
{
    if (_p != _nameMap.end())
        return true;
    return false;
}

void BaseNameResolver::next(string& key , string& value)
{
    key = string((*_p).first);
    value = string((*_p).second);
    _p++;
}

