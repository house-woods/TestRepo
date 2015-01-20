/***********************************************************************************************
* COPYRIGHT 1999 - 2007 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: Tutil
* Type: C++
* Originator: dtchalak
* File: tutil.cpp
* Version:
* Description:
* Implement Some Base operations.
*
* Revision History:
*
**********************************************************************************************/

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <ospace/helper.h>
#include <netdb.h>
#include "Rijndael.h"

using namespace std;

#include "EmsUnixDefines.h"
#include "tutil.h"

#include "sysErrLog.h"
#include "BaseException.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "MoAttributefactory.h"
#include "MoAttributePo.h"
#include "EmsTypes.h"
#include "EMSDefines.h"
#include "Server_i.h"
#include "EmsUnixDefines.h"
#include "ace/Env_Value_T.h"
#include "SystemConfigMgr.h"
#include "Cryptor.h"

#include "ConfigNames.h"
#include "RDbFactory.h"
#include "RDbMgr.h"


extern const char* sourceName;

#define ATTRIB_DELIM '.'

using namespace EMS_FW;

using namespace ELogger;      
using namespace RDb;

Tutil* Tutil::_instance = 0;

int Tutil::_counter = 0;

const char Tutil::EMS_DELIM = '@';

osi_mutex Tutil::_counterMutex;


string Tutil::getClassName()
{
    return "Tutil";
}

void Tutil::registerUtil(Tutil* util)
{
    _instance = util;
}


string Tutil::trim(const string& str)
{
    string separators(" ");
    size_t start = str.find_first_not_of(separators);
    if (start == osi_npos)
        return ""; // this is empty string
    int stop = str.find_last_not_of(separators);
    if (stop == osi_npos)
        return ""; // this is empty string
    return str.substr(start, stop - start + 1);
}


bool Tutil::extractAttribute(const string& name, string& atrName)
{
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractAttribute", TRACE3, "Method Begin for attribute=%s", name.c_str());
    if (name.size() == 0)
    {
        // invalid name
        ErrorLogger::logError(sourceName, getClassName().c_str(), "extractAttribute",
                              __LINE__, CRITICAL,
                              "Invalid Name ???");
        return false;
    }
    int pos = name.find(ATTRIB_DELIM);
    if (pos < 1)  // .AttributeName is not allowed
    {
        // name is not valid should be MOC.AttributeName
        atrName = name;
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractAttribute", __LINE__, TRACE3, "Method finished extracted attrib=%s", atrName.c_str());
        return true;
    }
    if (((unsigned)(pos + 1)) >= name.size())
    {
        // name is not valid should be MOC.AttributeName
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractAttribute",
                                 __LINE__, CRITICAL,
                                 "Invalid Attribute %s pos = %d", name.c_str(), pos);
        return false;
    }
    atrName = name.substr(pos + 1);
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractAttribute", __LINE__, TRACE3, "Method finished extracted attrib=%s", atrName.c_str());
    return true;
}


bool Tutil::extractMoc(const string& name, string& moc)
{
    if (name.size() == 0)
    {
        // invalid name
        ErrorLogger::logError(sourceName, getClassName().c_str(), "extractMoc",
                              __LINE__, CRITICAL,
                              "Invalid Name ???");
        return false;
    }
    int pos = name.find(ATTRIB_DELIM);
    if (pos < 1) // .AttributeName is not allowed
    {
        // name is not valid should be MOC.AttributeName
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractAttribute",
                                 __LINE__, TRACE1,
                                 "Warning: Attribute %s is not using MOC.NAME convention; MOC has been set to an empty string", name.c_str());
        moc = "";
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractMoc", __LINE__, TRACE3, "Method finished moc=%s", moc.c_str());
        return true;
    }
    if (((unsigned)(pos + 1)) >= name.size())
    {
        // name is not valid should be MOC.AttributeName
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractMoc",
                                 __LINE__, CRITICAL,
                                 "Invalid Attribute %s pos = %d", name.c_str(), pos);
        return false;
    }
    moc = name.substr(0, pos);
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractMoc", __LINE__, TRACE3, "Method finished moc=%s", moc.c_str());
    return true;
}


bool Tutil::extractKeyValue(const char* delim, const string& line,
                            string& key, string& value)
{
    /*  if(ErrorLogger::isTraceListOn())
          ErrorLogger::logVarError(sourceName,getClassName().c_str(),"extractKeyValue",__LINE__,TRACE3,"Method Begin line=%s",line.c_str());*/
    if (delim == NULL)
    {
        // invalid delimeter
        ErrorLogger::logError(sourceName, getClassName().c_str(), "extractKeyValue",
                              __LINE__, CRITICAL,
                              "NULL Delimeter ???");
        return false;
    }
    if (line.size() == 0)
    {
        // line with zero size
        ErrorLogger::logError(sourceName, getClassName().c_str(), "extractKeyValue",
                              __LINE__, MINOR,
                              "Zero size Line ???");
        return false;
    }
    int pos = line.find(delim);
    if (pos < 1) // Invalid Line
    {
        // line is not valid unable to find delimeter should be Key=Value
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractKeyValue",
                                 __LINE__, MINOR,
                                 "Invalid Line no delimeter %s", line.c_str());
        return false;
    }
    key = line.substr(0, pos);
    if (((unsigned)(pos + 1)) >= line.size())
    {
        // empty value
        value = "";
    }
    else
    {
        value = line.substr(pos + 1);
    }
    /*  if(ErrorLogger::isTraceListOn())
          ErrorLogger::logVarError(sourceName,getClassName().c_str(),"extractKeyValue",__LINE__,TRACE3,"Method finished key=%s value=%s",key.c_str(),value.c_str());*/
    return true;
}

bool Tutil::_defaultExtractMoi(const char* moi, const char* delim, string& result)
{

    const char* tmp = strstr(moi, delim);
    if (tmp == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "extractMoi",
                                 __LINE__, MINOR,
                                 "Unable to extract Moi from %s delim=%s", moi, delim);
        return false;
    }
    string tmpstr = tmp;
    tmpstr = trim(tmpstr);
    int nPos = tmpstr.size() - 1;
    if ((tmpstr.at(nPos) == '}') || (tmpstr.at(nPos) == ']') || (tmpstr.at(nPos) == ')'))
    {
        result = tmpstr.substr(0, tmpstr.size() - 1);
    }
    else
    {
        result = tmpstr;
    }
    return true;
}

bool Tutil::extractMoi(const char* moi, const char* delim, string& result)
{
    if (_instance)
        return _instance->_extractMoi(moi, delim, result);
    return _defaultExtractMoi(moi, delim, result);
}

bool Tutil::_extractMoi(const char* moi, const char* delim, string& result)
{
    return _defaultExtractMoi(moi, delim, result);
}


bool Tutil::timeToLong(const char* timestr, long& ltime)
{
    if (_instance)
        return _instance->_timeToLong(timestr, ltime);
    throw BaseException("Specific Tutil  is not registered", getClassName().c_str(), "timeToLong");
    return false;
}


bool Tutil::longToTime(const long& ltime, string& timestr)
{
    if (_instance)
        return _instance->_longToTime(ltime, timestr);
    throw BaseException("Specific Tutil  is not registered", getClassName().c_str(), "longToTime");
    return false;
}

string Tutil::convertInput(int size, const char* source, const char *desc, ...)
{
    char* res = (char*) malloc(size);
    va_list ap;
    va_start(ap, desc);

    _vsnprintf(res, size, desc, ap);
    string r(source);
    r += " ";
    r += res;
    free(res);
    return r;
}


string Tutil::getParentMoi(const string& moi)
{
    if (_instance)
        return _instance->_getParentMoi(moi);
    throw BaseException("Specific Tutil  is not registered", getClassName().c_str(), "getParentMoi");
    return "";
}

string Tutil::getNeId(const string& moi)
{

    size_t pos = moi.find_first_of(EMS_DELIM);
    string baseElement;
    if (pos == osi_npos)
    {
        // Root Object -> Managed Element
        return moi;
    }
    else
        return moi.substr(0, pos);
}

string Tutil::getUniqueId()
{
    CounterLock lock1(_counterMutex);
    time_t ltime;
    time(&ltime);
    struct tm result;
    struct tm* t = gmtime_r(&ltime, &result);
    long lt = mktime(t);
    char buffer[64];
    ltoa(lt, buffer, 10);
    string res = buffer;
    itoa(_counter, buffer, 10);
    res += buffer;
    _counter++; //only one thread will increment counter
    return res;
}

// This function is implemented to generate unique ID with lesser digits with just the seconds in contrast
// to the previous function getUniqueId() which appends the counter. This was done due to the problem
// encountered in the DSET EFD name which used the NeID as it's name and had a restriction in it's name
// integer value of 3 billion.

//change made to test the size of the NeId for the new Dset IFD the integer value should be in the range of 268 435 455 or 0xFFFFFFF for the DSET MIT
//ykouame 07-04-02 MR #85467 this is a for test purpuse only

string Tutil::getUniqueNeId()
{
    CounterLock lock1(_counterMutex);
    time_t ltime;
    time(&ltime);
    struct tm result;
    struct tm* t = gmtime_r(&ltime, &result);
    long lt = mktime(t);
    char buffer[64];
    char neIdFirstDigit = '1';

    ltoa(lt, buffer, 10);
    string res = buffer;
    string res2 = neIdFirstDigit + res.substr(2, 8);

    return res2;

}

string Tutil::getEmsDelimeter()
{
    string delim = "1";
    delim[0] = EMS_DELIM;
    return delim;
}


void Tutil::fillAttribSequence(const list<MoAttributePo*>& attrList,
                               EMS::Attributes& results)
{
    list<MoAttributePo*>::const_iterator p;
    MoValue* value = NULL;
    int count = 0;
    results.length(attrList.size());
    for (p = attrList.begin(); p != attrList.end(); p++)
    {
        // fill sequence attributes
        EMS::EmsAttribute attr;
        const string& name = (*p)->getName();
        if (name == EMSDEF::ATTRIBUTEHOLDERTYPE)
        {
            // this is attribute wrapper around EMS::Attribute use it directly
            attr = ((EmsAttributeHolderType*) (*p)->getValue())->getValue();
        }
        else
        {
            // convert
            attr.attName = name.c_str();
            value = (*p)->getValue();
            if (value == NULL)
            {
                ErrorLogger::logVarError(sourceName, getClassName().c_str(), "fillAttribSequence",
                                         __LINE__, CRITICAL,
                                         "null value for %s", name.c_str());
                continue;
            }
            value->toCorba(attr.value);
        }
        results[count] = attr;
        count++;
    }
    results.length(count);
}

void Tutil::fillAttribList(const EMS::Attributes& attrib,
                           list<MoAttributePo*>& attrList)
{
    ErrorLogger::logError(sourceName, getClassName().c_str(), "fillAttribList", TRACE4, "Method started");
    long seqlength = attrib.length();
    MoAttributeFactory* factory = MoAttributeFactory::instance();
    EMS::EmsAttribute* attr;
    for (long count = 0; count < seqlength; count++)
    {
        // fill attributes
        attr = (EMS::EmsAttribute *) (&(attrib) [count] );
        const char* atrName = attr->attName;
        if (atrName == NULL)
        {
            ErrorLogger::logVarError(sourceName, getClassName().c_str(), "fillAttribList",
                                     __LINE__, CRITICAL,
                                     "Null Name for count=%d", count);
            // never suppose to happen
            throw BaseException("Null Name for attribute", getClassName().c_str(), "fillAttribList");
        }
        MoValue* value;
        value = factory->getMoValue(atrName);
        if (value == NULL)
        {
            string desc = convertInput(MAX_DESCRIPTION_SIZE, sourceName,
                                       "Unable to  extract MoValue for Name=%s", atrName);
            ErrorLogger::logError(sourceName, getClassName().c_str(), "fillAttributtes",
                                  __LINE__, CRITICAL, desc.c_str());
            throw BaseException(desc, getClassName().c_str(), "fillAttribList");
        }
        value->fromCorba(attr->value);
        MoAttributePo* attrib = new MoAttributePo(atrName, value);
        attrList.push_back(attrib);
    }
    ErrorLogger::logError(sourceName, getClassName().c_str(), "fillAttribList", TRACE4, "Method finished");
}


void Tutil::deleteAttributeList(list<MoAttributePo*>& attrList)
{

    for (list<MoAttributePo*>::iterator p = attrList.begin(); p != attrList.end(); p++)
    {
        delete *p;
    }

    attrList.clear();
}

void Tutil::deleteAttribute(const string& attrName, list<MoAttributePo*>& attrList)
{
    list<MoAttributePo*>::iterator itr;
    for (itr = attrList.begin(); itr != attrList.end(); itr++)
    {
        if ((*itr)->getName() == attrName)
        {
            delete *itr;
            attrList.erase(itr);
        }
    }
}

const MoAttributePo* Tutil::getAttribute(const string& name, const list<MoAttributePo*>& attrList)
{
    list<MoAttributePo*>::const_iterator p;
    for (p = attrList.begin(); p != attrList.end(); p++)
    {
        if ((*p)->getName() == name)
            return *p; // attribute fould
    }
    return NULL;
}

bool Tutil::isNEOnDemandMode(const string& neId)
{
    /* CmNetworkElement* ne = (CmNetworkElement*) Server_i::instance()->locateNetworkElement(neId);
      if (ne)
        return ne->isOnDemandMode();*/

    return false;
}

void Tutil::setNEOnDemandMode(const string& neId, bool mode)
{
    /* CmNetworkElement* ne = (CmNetworkElement*) Server_i::instance()->locateNetworkElement(neId);
      if (!ne)
      {
        string tmp = "Tutil cannot locate NetworkElement " + neId + " .";
        throw BaseException(tmp,getClassName().c_str(),"setNEOnDemandMode");
      }
     
      ne->setOnDemandMode(mode);
      if (mode)
        ne->setConfigValue(ConfigNames::OnDemandSupportedKey, ConfigNames::True);
      else
        ne->setConfigValue(ConfigNames::OnDemandSupportedKey, ConfigNames::False);*/
}

bool Tutil::keyInside(const string& data, const string& key)
{
    size_t pos = data.find_first_of(key);
    if (pos == osi_npos)
    {
        // no key found
        return false;
    }
    else
        return true;
}

string Tutil::doubleToString (double number)
{
    stringstream temp;
    temp << number;
    return temp.str();
}

string Tutil::numberToString (unsigned long number)
{
    stringstream temp;
    temp << number;
    return temp.str();
}

string Tutil::getUsername (void)
{
    char temp[128];
    unsigned int bufferSize = unsigned(128);
    GetUserName(temp, &bufferSize);
    string userName(temp);
    return userName;
}

string Tutil::getHostname (void)
{
    // this will return the interface for the default Ethernet adapters
    string hostname;
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());

    try
    {
        string sql = "select config_value from system_config where config_name = :1 and  config_type = :2";
        dbMgr->setSQL (sql);
        dbMgr->bindStringParam (1, ConfigNames::NamingServiceHostNameKey);
        dbMgr->bindStringParam (2, "GLOBAL");
        auto_ptr<ResultSet> rs(dbMgr->executeQuery());
        if (rs->moveNext())
        {
            hostname=rs->getString(1);
        }
    } catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getHostname", __LINE__, MAJOR,
                       "Could not get IP address from DB because of %s", ex.getDescription().c_str());
    }
    ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getHostname", TRACE1, "Obtained Hostname = %s ", hostname.c_str());

    char hostname_UpperCase[255];
    unsigned int i = 0;
    for (;i < hostname.length();i++)
        hostname_UpperCase[i] = toupper (hostname[i]);
    hostname_UpperCase[i] = 0;
    string capitaliizedHostName (hostname_UpperCase);
    return capitaliizedHostName;
}

string Tutil::getIPAddress (void)
{
    // this will return the interface for the default Ethernet adapters
    string ipAddress;
    auto_ptr<RDbMgr> dbMgr(RDbFactory::getDbMgr());

    try
    {
        string sql = "select config_value from system_config where config_name = :1 and  config_type = :2";
        dbMgr->setSQL (sql);
        dbMgr->bindStringParam (1, "NAMINGSERVICE_HOSTIP");
        dbMgr->bindStringParam (2, "GLOBAL");
        auto_ptr<ResultSet> rs(dbMgr->executeQuery());
        if (rs->moveNext())
        {
            ipAddress= rs->getString(1);
        }
    } catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "getIpAddress", __LINE__, MAJOR,
                       "Could not get IP address from DB because of %s", ex.getDescription().c_str());
    }
    ErrorLogger::logVarError(sourceName, "getIpAddress", TRACE1, "Obtained IP address = %s ", ipAddress.c_str());

    return ipAddress;
}

bool Tutil::parseFile (const string &filename, map<string, string>& nameValuePairs )
{
    ifstream* file = new ifstream();
    file->open(filename.c_str()); //,ios::nocreate);
    file->setf(ios::skipws);
    if ((*file) == false)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "parseFile", __LINE__, MAJOR, "File %s does not exist", filename.c_str());
        return false;
    }

    string line;
    while (getLine(file, line))
    {
        if (line.length() == 0)
            continue;
        if (line.find("#") == 0)
            continue;

        string key = "";
        string value = "";
        if (line.size() > 0)
        {
            int pos = line.find("=");
            if (pos >= 1)
            {
                key = line.substr(0, pos);
                if (((unsigned)(pos + 1)) < line.size())
                    value = line.substr(pos + 1);
                nameValuePairs.insert (pair<string, string> (key, value));
            }
        }
    }
    return true;
}

bool Tutil::getLine (ifstream *&file, string& line)
{
    if (!(*file))
        return false;
    char cline[2048];
    file->getline(cline, 2048);
    string tmp = cline;
    int len = tmp.length();
    int last = tmp.find_last_not_of(" ", len);
    if ((last < 0) || (last > len))
    {
        line = tmp; // no white spaces at the end
        return true;
    }
    line = tmp.substr(0, last + 1);
    return true;
}


string Tutil::getEnvVariableValue (const string &variableName)
{
    ACE_Env_Value<string> env;
    env.open (variableName.c_str(), "");
    return (env);
}

bool Tutil::encryptRijndael (const char *plainTextBuffer, char *encryptedTextBuffer)
{
    if (plainTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "encryptRijndael", __LINE__, MAJOR, "No text supplied to encrypt");
        return false;
    }
    if (encryptedTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "encryptRijndael", __LINE__, MAJOR, "Could not encrypt text [%s]; Destination buffer is invalid", plainTextBuffer);
        return false;
    }
    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey("abcdefghabcdefgh", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
        oRijndael.EncryptBlock(plainTextBuffer, encryptedTextBuffer);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "encryptRijndael", __LINE__, MAJOR, "Could not encrypt text [%s]", plainTextBuffer);
        return false;
    }

    return true;
}

bool Tutil::encryptRijndaelWithKey (const char *plainTextBuffer, char *encryptedTextBuffer, const string& key)
{
    if (plainTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "encryptRijndael", __LINE__, MAJOR, "No text supplied to encrypt");
        return false;
    }
    if (encryptedTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "encryptRijndael", __LINE__, MAJOR, "Could not encrypt text [%s]; Destination buffer is invalid", plainTextBuffer);
        return false;
    }
    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
        oRijndael.EncryptBlock(plainTextBuffer, encryptedTextBuffer);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "encryptRijndael", __LINE__, MAJOR, "Could not encrypt text [%s]", plainTextBuffer);
        return false;
    }

    return true;
}

bool Tutil::decryptRijndael (const char* encryptedTextBuffer, char *plainTextBuffer)
{
    if (encryptedTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "decryptRijndael", __LINE__, MAJOR, "No text supplied to decrypt");
        return false;
    }
    if (plainTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "decryptRijndael", __LINE__, MAJOR, "Could not decrypt text [%s]; Destination buffer is invalid", encryptedTextBuffer);
        return false;
    }
    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey("abcdefghabcdefgh", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
        oRijndael.DecryptBlock(encryptedTextBuffer, plainTextBuffer);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "decryptRijndael", __LINE__, MAJOR, "Could not decrypt text [%s]", encryptedTextBuffer);
        return false;
    }

    return true;
}

bool Tutil::decryptRijndaelWithKey (const char* encryptedTextBuffer, char *plainTextBuffer, const string& key)
{
    if (encryptedTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "decryptRijndael", __LINE__, MAJOR, "No text supplied to decrypt");
        return false;
    }
    if (plainTextBuffer == NULL)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "decryptRijndael", __LINE__, MAJOR, "Could not decrypt text [%s]; Destination buffer is invalid", encryptedTextBuffer);
        return false;
    }
    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
        oRijndael.DecryptBlock(encryptedTextBuffer, plainTextBuffer);
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "decryptRijndael", __LINE__, MAJOR, "Could not decrypt text [%s]", encryptedTextBuffer);
        return false;
    }

    return true;
}


// encrypt an ASCII string containing no NULs and no longer than blockSize characters
// and return the ASCII hex representation of the binary result.
// TODO: handle strings of arbitrary length and content.
string Tutil::encryptRijndaelEx (const string& plainText, const string& key)
{
    const int blockSize = 16;
    const string initialChain(blockSize, '\0');
    const char hexChars[] = "0123456789ABCDEF";

    string hexEncryptedText;

    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), initialChain.data(), key.size(), initialChain.size());

        string encryptedText;
        encryptedText.resize(blockSize);
        oRijndael.EncryptBlock(string(plainText).append(blockSize, '\0').data(), const_cast<char *>(encryptedText.data()));

        hexEncryptedText.reserve(2 * blockSize);
        for (int i = 0; i < blockSize; i++)
        {
            hexEncryptedText.push_back(hexChars[(encryptedText[i] >> 4) & 0x0f]);
            hexEncryptedText.push_back(hexChars[ encryptedText[i] & 0x0f]);
        }
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, getClassName().c_str(), "encryptRijndael_ex", __LINE__, MAJOR, "Encryption failure");
        hexEncryptedText = "00112233445566778899AABBCCDDEEFF";
    }

    return hexEncryptedText;
}

// handle password less than 16 chars
string Tutil::encryptRijndaelExWithPadding (const string& plainText, const string& key)
{

    char textPassword[32];
    unsigned short blockSize = 16;
    short lenPass = plainText.length();
    for ( int idex = 0;idex<lenPass;idex++ )
    {
        textPassword[idex]=plainText[idex];
    }

    if (lenPass>=16)
    {
        blockSize =32;
    }
    if ( lenPass<blockSize )
    {
        short diff = blockSize-lenPass;
        for (int index = lenPass;index<blockSize;index++ )
        {
            textPassword[index]=diff;
        }
    }
    if (blockSize == 16)
         return Tutil::encryptRijndaelEx (textPassword);
    else
         return Tutil::encryptRijndael32Ex(textPassword);
}

string Tutil::encryptRijndael32Ex (const string& plainText, const string& key)
{
    const int blockSize = 32;
    const string initialChain(blockSize, '\0');
    const char hexChars[] = "0123456789ABCDEF0123456789ABCDEF";

    string hexEncryptedText;

    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), initialChain.data(), key.size(), initialChain.size());

        string encryptedText;
        encryptedText.resize(blockSize);
        int plainTextSize = plainText.size();
        oRijndael.EncryptBlock(string(plainText).append((32-plainTextSize), '\0').append(blockSize, '\0').data(), const_cast<char *>(encryptedText.data()));

        hexEncryptedText.reserve(2 * blockSize);
        for (int i = 0; i < blockSize; i++)
        {
            hexEncryptedText.push_back(hexChars[(encryptedText[i] >> 4) & 0x0f]);
            hexEncryptedText.push_back(hexChars[ encryptedText[i] & 0x0f]);
        }
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, getClassName().c_str(), "encryptRijndael_ex", __LINE__, MAJOR, "Encryption failure");
        hexEncryptedText = "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF";
    }

    return hexEncryptedText;
}

// decrypt the output of the above function
string Tutil::decryptRijndael32Ex (const string& hexEncryptedText, const string& key)
{
    const int blockSize = 32;
    const string initialChain(blockSize, '\0');

    string plainText;

    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), initialChain.data(), key.size(), initialChain.size());

        string encryptedText;
        encryptedText.reserve(blockSize);
        for ( int i = 0; i < 2 * blockSize; i += 2)
        {
            char hi = hexEncryptedText[i];
            char lo = hexEncryptedText[i + 1];
            encryptedText.push_back( (char) ( ( ((hi >= '0' && hi <= '9') ? (hi - '0') : (hi - 'A' + 10)) << 4) |
                                              ((lo >= '0' && lo <= '9') ? (lo - '0') : (lo - 'A' + 10)) ) );
        }

        plainText.resize(blockSize);
        oRijndael.DecryptBlock(encryptedText.data(), const_cast<char *>(plainText.data()));

        if (plainText.find_last_not_of('\0') == string::npos)
        {
            plainText.resize(0);
        }
        else
        {
            plainText.resize(plainText.find_last_not_of('\0') + 1);
        }
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, getClassName().c_str(), "decryptRijndael_ex", __LINE__, MAJOR, "Decryption failure");
        plainText = "bad\003string";
    }

    return plainText;
}

// decrypt the output of the above function
string Tutil::decryptRijndaelEx (const string& hexEncryptedText, const string& key)
{
    const int blockSize = 16;
    const string initialChain(blockSize, '\0');

    string plainText;

    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), initialChain.data(), key.size(), initialChain.size());

        string encryptedText;
        encryptedText.reserve(blockSize);
        for ( int i = 0; i < 2 * blockSize; i += 2)
        {
            char hi = hexEncryptedText[i];
            char lo = hexEncryptedText[i + 1];
            encryptedText.push_back( (char) ( ( ((hi >= '0' && hi <= '9') ? (hi - '0') : (hi - 'A' + 10)) << 4) |
                                              ((lo >= '0' && lo <= '9') ? (lo - '0') : (lo - 'A' + 10)) ) );
        }

        plainText.resize(blockSize);
        oRijndael.DecryptBlock(encryptedText.data(), const_cast<char *>(plainText.data()));

        if (plainText.find_last_not_of('\0') == string::npos)
        {
            plainText.resize(0);
        }
        else
        {
            plainText.resize(plainText.find_last_not_of('\0') + 1);
        }
    }
    catch (...)
    {
        ErrorLogger::logError(sourceName, getClassName().c_str(), "decryptRijndael_ex", __LINE__, MAJOR, "Decryption failure");
        plainText = "bad\003string";
    }

    return plainText;
}

void Tutil::getReadablePW (char* userPassword)
{
     char pChar=userPassword[0];
     int indexP=0;

     for (;(pChar!='\0' && indexP<=31);indexP++)
     {
        pChar = userPassword[indexP];
        if (pChar<33 || pChar >126)
        {
           userPassword[indexP] = '\0';
           break;
        }
     }
}

void Tutil::getReadablePW (string& userPassword)
{
     char pChar=userPassword[0];
     int indexP=0;

     for (;(pChar!='\0' && indexP<=31);indexP++)
     {
        pChar = userPassword[indexP];
        if (pChar<33 || pChar >126)
        {
           userPassword.resize(indexP);
           break;
        }
     }

}


void Tutil::getHashedPW (const string& password, string& hash)
{
     string rawPassword;
     string hashedPwd = password;
     Tutil::toUpperCase(hashedPwd);
     rawPassword = Tutil::decryptRijndaelEx(hashedPwd);

     // Need to remove the trace log after debugging
     ErrorLogger::logVarError(sourceName, "Tutil", "getHashedPW",TRACE5, "Validate for hashed password %s", 
         hashedPwd.c_str());

     ErrorLogger::logVarError(sourceName, "Tutil", "getHashedPW",TRACE5, "Validate for password %s", 
         rawPassword.c_str());

     Tutil::getReadablePW(rawPassword);
     ErrorLogger::logVarError(sourceName, "Tutil", "getHashedPW",TRACE5, "Validate again for password %s", 
         rawPassword.c_str());

    hash = Cryptor::encrypt(rawPassword, Cryptor::md5Method);

}

void Tutil::renameFile (const string &oldfilename, const string &newfilename)
{
    if (osi_file_system::exists( newfilename))
        osi_file_system::remove
        ( newfilename );

    if (osi_file_system::exists( oldfilename))
        osi_file_system::rename( oldfilename, newfilename );

}

void Tutil::appendCommandLineArgs (string &basicCommandLine, list<string> &argList)
{
    for (list<string>::iterator iter = argList.begin(); iter != argList.end();iter++)
    {
        string arg = (*iter);
        basicCommandLine.append (" ");
        basicCommandLine.append (arg);
        basicCommandLine.append (" ");
    }
}

char Tutil::charToUpperCase (char element)
{
    if ((element >= 'a') && (element <= 'z'))
        element -= 32;
    return element;
}

char Tutil::charToLowerCase (char element)
{
    if ((element >= 'A') && (element <= 'Z'))
        element += 32;
    return element;
}

void Tutil::toUpperCase (string &myString)
{
    transform (myString.begin(), myString.end(), myString.begin(), charToUpperCase);
}

void Tutil::toLowerCase (string &myString)
{
    transform (myString.begin(), myString.end(), myString.begin(), charToLowerCase);
}

void Tutil::removeFile (const string &filename)
{
    if (osi_file_system::exists (filename))
        osi_file_system::remove
        (filename);
}

void Tutil::coreFileBackup()
{
    ErrorLogger::logVarError(sourceName, "Tutil", "coreFileBackup", TRACE3, "Try and back up the core files here");
    try
    {
        string _baseDirectory = SystemConfigMgr::instance()->getConfig ("GLOBAL", "", "BaseDirectory");
        string coreDirectory = _baseDirectory + EMSDEF::SLASH + string("corefiles");
        osi_directory dir(coreDirectory);
        const string dotOne = string(".1");
        const string dotTwo = string(".2");
        vector<string> fNames = dir.filenames(); // get a list of all the files in the folder
        vector<string> fNamesDotOne; // List of filenames ending in .1
        vector<string>::iterator i;
        vector<string>::iterator j;
        for (i = fNames.begin(); i != fNames.end(); i++)
        {
            osi_path path = coreDirectory + EMSDEF::SLASH + *i;
            if (path.extension() == "1")
            {
                fNamesDotOne.push_back(path);
            }
        }
        for (i = fNames.begin(); i != fNames.end(); i++)
        {
            osi_path path = coreDirectory + EMSDEF::SLASH + *i;
            bool dotOneExists = false;
            for (j = fNamesDotOne.begin(); j != fNamesDotOne.end(); j++)
            {
                if (*j == (path + dotOne))
                {
                    dotOneExists = true;
                    break;
                }
            }
            if (dotOneExists)
            {
                Tutil::renameFile(path + dotOne, path + dotTwo); // back up .1 to .2
            }
            if (path.extension() != "1" && path.extension() != "2" &&
                (path.extension() == "exe" || path.extension() == "Naming_Service" || path.extension() == "Notify_Service"))
            {
                Tutil::renameFile(path, path + dotOne); // back up fresh core file
            }
        }
        ErrorLogger::logVarError(sourceName, "Tutil", "coreFileBackup", TRACE3, "Core file backup complete");
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, "Tutil", "coreFileBackup", TRACE3, "Core file back up unsuccessful; %s", ex.getDescription().c_str());
    }
    catch (osi_file_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, "Tutil", "coreFileBackup", TRACE3, "Core file backup unsuccessful; %s", ex.description(ex.code()));
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "Tutil", "coreFileBackup", TRACE3, "Core file backup unsuccessful; Unknown error");
    }

}

string Tutil::padBack (const string &stringToPad, char padCharacter, int totalCharsInResult)
{
    int padLength = totalCharsInResult - stringToPad.length();
    if (padLength <= 0)
        return stringToPad;
    string paddedString;
    paddedString.append (padLength, padCharacter);
    paddedString.append (stringToPad);
    return paddedString;
}
