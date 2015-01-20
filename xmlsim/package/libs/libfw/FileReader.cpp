
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:47:04  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: FileReader 
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   FileReader.cpp  $
* Version: $Revision:   1.6  $
* Description: 
* Read from file and parse line by line name=value sequence
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/FileReader.cpv  $
   
      Rev 1.6   Oct 27 1999 14:47:04   dtchalak
   Query Interface updates.
   
      Rev 1.5.1.0   Sep 20 1999 16:48:02   dtchalak
   Just to create a branch.
   
      Rev 1.5   02 Sep 1999 15:54:08   dtchalak
   New interface support added.
   
      Rev 1.4   21 Jul 1999 10:38:22   dtchalak
   Multiple Ne support included.
   
      Rev 1.3   14 Jun 1999 16:28:34   dtchalak
   Updates related to new CM.idl and other details.
   
      Rev 1.2   21 May 1999 09:56:44   dtchalak
    
   
      Rev 1.0   05 Apr 1999 13:50:24   bacuff
    
   
      Rev 1.1   04 Feb 1999 15:16:18   dtchalak
   ready for IP-5
   
      Rev 1.0   22 Jan 1999 13:48:42   dtchalak
    
**********************************************************************************************/

#include "FileReader.h"
using namespace std;

using namespace EMS_FW;

FileReader::FileReader()
{ }

FileReader::FileReader(const char* name)
{
    f.open(name, ios::nocreate);
    f.setf(ios::skipws);
}

FileReader::~FileReader()
{
    f.close();
}

bool FileReader::bad()
{
    return ((f) ? false : true);
}

bool FileReader::getLine(string& line)
{
    char cline[1024];

    // get the next non-blank/non-comment line
    while (f.getline(cline, 1024))
    {
        // if (f.eof())  // end of file
        //  return false;
        string tmp = cline;
        int len = tmp.length();
        if (len == 0)
            continue;  // skip blank line
        if (tmp.find("#") != string::npos)
            continue;  // skip comment line

        int last = tmp.find_last_not_of(" ", len);
        if ((last < 0) || (last > len))
        {
            line = tmp; // no white spaces at the end
            return true;
        }
        line = tmp.substr(0, last + 1);
        if (line.length() == 0)
            continue; // nothing just white space
        else
            return true;
    }

    return false;
}

// go to beginning of file
void FileReader::begin()
{
    f.seekg(0, ios::beg);
}

bool FileReader::eof()
{
    return f.eof();
}
