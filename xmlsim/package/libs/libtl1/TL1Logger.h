/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: TL1 Connection
* Type: C++
* Originator: $Author:   aaali  $
* File: $Workfile:   TL1Logger.h
* Version: $Revision:   1.0  $
* Description:
* Header for class resposible for logging tl1 commands and responses in a tid-based log file
*
* Revision History:
*
* $Log:  $
************************************************************************************/

#ifndef _TL1LOGGER_H_
#define _TL1LOGGER_H_

#include <string>
#include <list>
#include "BaseException.h"
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
//#else
#include <ospace/file.h>
//#endif

#define NOOFMESSAGES 20

namespace EMS_FW
{
class TL1Logger
{
public:
    // call this to continue logging and wrapping in your desired file
    TL1Logger (const string &filePath, unsigned long masSize = NOOFMESSAGES);
    // call this to log in a file of the format prefix + current_time if wraparound = false
    TL1Logger (const string &filePrefix, unsigned long maxSize = NOOFMESSAGES, bool wrapAround = true, int numberOfFiles = 5);
    // call this to let the logger construct a file based on the tid and current_time
    TL1Logger (const string &logDirectory, const string &hostname, const string &tid, unsigned long maxSize = NOOFMESSAGES, bool wrapAround = true, int numberOfFiles = 5, bool deleteOnExit = false);
    // Destructore - closes the file
    ~TL1Logger (void);
    // log message to the log file for the particular tid;
    void logMessage (const string &messageType, const string &message, const string& conName);
    // call to set the flag which controls whether to delete log files on exit
    void setDeleteOnExit (bool deleteOnExit)
    {
        _deleteOnExit = deleteOnExit;
    }

private:
    // if set to true, wraps around the initially define log file otherwise creates a new one
    bool _wrapAround;
    // number of files to create for one Ne before the oldest one is overwritten
    int _maxNumberOfFiles;
    // number of files to create for one Ne before the oldest one is overwritten
    string _logDirectory;
    // tid of the NE for which logging is taking place
    string _tid;
    // host on which NE exists
    string _hostname;
    // maximum size of each log file
    unsigned long _maxSize;
    // path of the current log file
    string _logFilePath;
    // number of messages written so far
    unsigned long _messageCount;
    // list of log files that have been created so far for this NE
    list <string> _logFileList;
    // set to true to delete all files when the logger is deleted i.e. NE removed
    bool _deleteOnExit;
    // get and return stringified time
    void getTime(char* buffer);
    // handle for the current log file
    osi_file*  _logFileHandle;
    // returns true if directoryName exists
    bool directoryExists (const string &directoryName);
    // tries to create a directory returning true on success
    bool createDirectory (const string &directoryName);
    // generates filenpath based on directory, tid and current time
    string generateFileName (const string &directory, const string &preFix);
    // removes the file for the passed filename
    bool removeFile (const string &filename);
    // creates the file and returns the handle
    osi_file *createFile (const string &filename);
    // closes the file associated with the passwd handle
    void closeFile (osi_file *fileHandle);
}; // end class declaration
}; // end namespace
#endif // _TL1LOGGER_H_




