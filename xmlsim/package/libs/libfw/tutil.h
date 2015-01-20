/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:48:38  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Tutil
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   tutil.h  $
* Version: $Revision:   1.6  $
* Description: 
* Implement Some Base operations.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/tutil.h_v  $
* 
*    Rev 1.6   Oct 27 1999 14:48:38   dtchalak
* Query Interface updates.
* 
*    Rev 1.5.1.0   Sep 20 1999 16:51:54   dtchalak
* Just to create a branch.
* 
*    Rev 1.5   02 Sep 1999 15:52:40   dtchalak
* New interface support added.
* 
*    Rev 1.4   21 Jul 1999 10:37:16   dtchalak
* Multiple Ne support included.
* 
*    Rev 1.3   23 Jun 1999 16:47:42   dtchalak
*  
* 
*    Rev 1.2   14 Jun 1999 16:27:32   dtchalak
* Updates related to new CM.idl and other details.
* 
*    Rev 1.1   04 May 1999 19:15:06   dtchalak
* Comments added.
* 
*    Rev 1.0   05 Apr 1999 13:51:46   bacuff
*  
* 
*    Rev 1.1   04 Feb 1999 15:18:54   dtchalak
* ready for IP-5
* 
*    Rev 1.0   22 Jan 1999 13:49:36   dtchalak
*  
* 
*    Rev 1.1   Nov 09 1998 17:03:02   dtchalak
*  
* 
*    Rev 1.0   Nov 05 1998 11:52:38   dtchalak
*  
**********************************************************************************************/



#ifndef _TUTIL_H_
#define _TUTIL_H_

#include <string>
#include <list>
#include <map>
#include <iostream>
#include <sstream>
#include <ospace/thread.h>
#include <ems.hh>

using namespace std;

class MoAttributePo;

namespace EMS_FW
{

// Base utility class used for data conversion. Specific implementation of
// this class supporting different protocols(CMIP...) need to be registered
// in this class.
class Tutil
{
public:

    static void registerUtil(Tutil* util);
    // Register specific implementation of utility class.

    static bool extractAttribute(const string& name, string& atrName);
    // Extract attribute name from a string containing moc.attrName


    static bool extractMoc(const string& name, string& moc);
    // Extract Managed Object Class Name from a string containing moc.attrName

    static bool timeToLong(const char* timestr, long& ltime);
    // Convert string representation of the agent time to long.

    static bool longToTime(const long& ltime, string& timestr);
    // Convert long representation of the agent time to string.

    static bool extractKeyValue(const char* delim, const string& line,
                                string& key, string& value);
    // Extract key Value from a line containing key=value.

    static bool extractMoi(const char* moi, const char* delim, string& result);
    // Extract pure Moi from a agent specific string.

    static string convertInput(int size, const char* source, const char *desc, ...);
    // Helper function similar to printf.

    static string getParentMoi(const string& moi);
    // Extract parent Moi from specified Moi.

    static string getNeId(const string& moi);
    // Extract Network Element Name from specified Moi.

    static string getUniqueId();
    // Return unique string.

    static string getUniqueNeId();
    // Return unique string for the creation of NeId.

    static void fillAttribSequence(const list<MoAttributePo*>& attrList,
                                   EMS::Attributes& results);

    static void fillAttribList(const EMS::Attributes& attrib,
                               list<MoAttributePo*>& attrList);

    static void deleteAttributeList(list<MoAttributePo*>& attrList);

    static void deleteAttribute(const string& attrName, list<MoAttributePo*>& attrList);

    static string getEmsDelimeter();

    static string getClassName();

    static string trim(const string& str);

    static bool keyInside(const string& data, const string& key);
    // Return true if key is contained into the data

    static const MoAttributePo* getAttribute(const string& name, const list<MoAttributePo*>& attrList);
    // return pointer to the attribute inside of the list or NULL

    static bool isNEOnDemandMode(const string& neId);
    // return true if NE is in On Demand Discovery mode

    static void setNEOnDemandMode(const string& neId, bool mode);
    // set the On Demand Discovery mode of the NE

    static const char EMS_DELIM;

    static string numberToString (unsigned long number);

    static string doubleToString (double number);

    static string getUsername (void);

    static string getHostname (void);

    static string getIPAddress (void);

    static bool parseFile (const string &filename, map<string, string>&nameValuePairs);

    static string getEnvVariableValue(const string &variableName);

    static bool encryptRijndael (const char* plainText, char *encryptedTextBuffer);

    static bool decryptRijndael (const char* encryptedText, char *plainTextBuffer);

    static bool encryptRijndaelWithKey (const char* plainText, char *encryptedTextBuffer, const string& key = "zifheers8592#(.@");

    static bool decryptRijndaelWithKey (const char* encryptedText, char *plainTextBuffer, const string& key = "zifheers8592#(.@");

    static string encryptRijndaelEx (const string& plainText, const string& key = "zifheers8592#(.@");

    static string encryptRijndaelExWithPadding (const string& plainText, const string& key = "zifheers8592#(.@");

    static string encryptRijndael32Ex (const string& plainText, const string& key = "zifheers8592#(.@kkiheers1120#(.@");

    static string decryptRijndaelEx (const string& hexEncryptedText, const string& key = "zifheers8592#(.@");

    static string decryptRijndael32Ex (const string& hexEncryptedText, const string& key = "zifheers8592#(.@kkiheers1120#(.@");

    static void renameFile (const string &oldfilename, const string &newfilename);

    static void removeFile (const string &filename);

    static void appendCommandLineArgs (string &basicCommandLine, list<string> &argList);

    static void toUpperCase (string &);

    static void toLowerCase (string &);

    static void coreFileBackup ();

    static string padBack (const string &stringToPad, char padCharacter, int totalCharsInResult);

    static void getReadablePW (char* userPassword);

    static void getReadablePW (string& userPassword);

    static void getHashedPW (const string& password, string& hash);

protected:





    virtual bool _timeToLong(const char* timestr, long& ltime) = 0;
    // Pure virtual function used as a base interface for converting time
    //to long for specific protocol implementation.

    virtual bool _longToTime(const long& ltime, string& timestr) = 0;
    // Pure virtual function used as a base interface for converting time
    //to string for specific protocol implementation.

    virtual string _getParentMoi(const string& moi) = 0;
    // Pure virtual function used as a base interface for extracting
    // parent moi from a specified moi.

    virtual bool _extractMoi(const char* moi, const char* delim, string& result);

    static bool _defaultExtractMoi(const char* moi, const char* delim, string& result);

    static Tutil* _instance;
    // Pointer to specific protocol dependend implementation of Tutil class.

    static int _counter;
    // Store number of getUniqueId invokations from the server start up.

    typedef osi_sem_lock<osi_mutex_semaphore> CounterLock;

    static osi_mutex _counterMutex;


    // Guard for seting of _counter.
private:
    static bool getLine (ifstream *&file, string& line);

    static char charToUpperCase (char element);

    static char charToLowerCase (char element);

};
};


#endif
