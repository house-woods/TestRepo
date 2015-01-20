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
* File: $Workfile:   TL1Logger.cpp
* Version: $Revision:   1.0  $
* Description:
* Class resposible for logging tl1 commands and responses in a tid-based log file
*
* Revision History:
*
* $Log:  $
************************************************************************************/

#include "TL1Logger.h"
#include <ospace/time/time.h>
#include <ospace/file.h>
#include <ospace/file/dir.h>
#include <algorithm>
#include <functional>
#include "EMSDefines.h"
#include "sysErrLog.h"


using namespace EMS_FW;
using namespace ELogger;


TL1Logger::TL1Logger (const string &filePath, unsigned long masSize)
{}

TL1Logger::TL1Logger (const string &filePrefix, unsigned long maxSize, bool wrapAround, int numberOfFiles)
{}

TL1Logger::TL1Logger (const string &logDirectory, const string &hostname, const string &tid, unsigned long maxSize, bool wrapAround, int maxNumberOfFiles, bool deleteOnExit)
{
    _logDirectory = logDirectory;
    _tid = tid;
    _maxSize = maxSize; // in KB
    _wrapAround = wrapAround;
    // if directory does not exist and we fail to create it then default to pwd
    if (!directoryExists (_logDirectory))
        if (!createDirectory (_logDirectory))
            _logDirectory = ".";
    _logFilePath = generateFileName (_logDirectory, _tid);
    // if a backup or '.old' file exists already, remove it
    if (osi_file_system::exists(_logFilePath + ".old"))
        osi_file_system::remove
        (_logFilePath + ".old");
    // if a file with the newly generated filename exists, back it up as .old
    if (osi_file_system::exists( _logFilePath))
        osi_file_system::rename( _logFilePath, _logFilePath + ".old");
    // create the log file
    _logFileHandle = createFile (_logFilePath);
}

TL1Logger::~TL1Logger (void)
{
    // close the current log file
    closeFile (_logFileHandle);
}

void TL1Logger::logMessage(const string& messageType, const string& message, const string& conName)
{
    if ((_logFileHandle == NULL) || !(_logFileHandle->good()))
        return ;

    int sizeInBytes = _logFileHandle->tell();
    if (_wrapAround)
    {
        // convert to KB and compare
        if ((sizeInBytes / 1024) >= _maxSize)
        {
            _logFileHandle->resize_to(sizeInBytes - 8);
            _logFileHandle->rewind();
        }
        else
        {
            if (sizeInBytes >= 8)
                _logFileHandle->seek(sizeInBytes - 8, SEEK_SET);
        }
    }
    else
    {
        if ((sizeInBytes / 1024) >= _maxSize)
        {
            closeFile (_logFileHandle);
            removeFile (_logFilePath);
            _logFileHandle = createFile (_logFilePath);
        }
    }

    char timeBuffer[128];
    getTime(timeBuffer);

    // message is formatted and dumped to file here
    string header = "\n[" + messageType + ":" + conName + "@" + timeBuffer + "]\n";
    string footer = "\n[/" + messageType + "]\n";
    _logFileHandle->write(header.c_str(), header.size());
    string duplicateMessage ;
    ErrorLogger::maskPassword(message, duplicateMessage);
    _logFileHandle->write(duplicateMessage.c_str(), duplicateMessage.size());
    _logFileHandle->write(footer.c_str(), footer.size());

    if (_wrapAround)
    {
        _logFileHandle->write ("\n*WRAP*\n", 8);
    }
}

void TL1Logger::getTime(char* buffer)
{
    struct tm* datetime;
    struct tm result;
    time_t current_time;
    time(&current_time);
    datetime = localtime_r(&current_time, &result);
    strftime(buffer, 128, "%y-%m-%d %X", datetime);
}

bool TL1Logger::directoryExists (const string &directoryName)
{
    return ( osi_directory::exists (directoryName.c_str()) );
}

bool TL1Logger::createDirectory (const string &directoryName)
{
    osi_directory::create (directoryName.c_str());
    return ( osi_directory::exists(directoryName.c_str()) );
}

string TL1Logger::generateFileName (const string &directory, const string &preFix)
{
    string prefix = "TL1";

    string suffix = ".log";

    string filename = directory + EMSDEF::SLASH + EMSDEF::SLASH + prefix + "-" + _tid + suffix;

    return filename;
}

osi_file *TL1Logger::createFile (const string &filename)
{
    osi_file *fileHandle = new osi_file(filename, osi_open_control::open_always , O_RDWR);
    if (!fileHandle->good())
    {
        delete fileHandle;
        fileHandle = NULL;
    }
    return fileHandle;
}

void TL1Logger::closeFile (osi_file *fileHandle)
{
    if (fileHandle)
    {
        fileHandle->close();
        delete fileHandle;
        fileHandle = NULL;
    }
}

bool TL1Logger::removeFile (const string &filename)
{
    osi_file_system::remove
    (filename);
    return !(osi_file_system::exists (filename));
}
