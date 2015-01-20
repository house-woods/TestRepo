/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:44  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: EmsUnixDefines
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EMSUnixDefines.h  $
* Version: $Revision:   1.2  $
* Description: 
* Defines that are needed to maintain the portable code between Unix and NT
* 
* Revision History:
*
*
**********************************************************************************************/

#ifdef WIN32

void Ems_static_init()
{
    // nothing to init for now
}


void Ems_static_cleanup()
{
    // nothing to clean for now
}

#endif

#ifdef EMSUNIX 
// here are all implementations required for unix build

#include "EmsUnixDefines.h"
#include <string>
#include <ospace/file.h>
#include <ospace/time.h>
#include <ospace/unix.h>
#include <ospace/thread.h>


void Ems_static_init()
{
    osi_init_file_toolkit();
    osi_init_time_toolkit();
    osi_init_unix_toolkit();
    osi_init_thread_toolkit();
    osi_init_network_toolkit();
}

void Ems_static_cleanup()
{
    osi_exit_file_toolkit();
    osi_exit_time_toolkit();
    osi_exit_unix_toolkit();
    osi_exit_thread_toolkit();
    osi_exit_network_toolkit();
}

int GetShortPathName(const char* name, char* shortName, int len)
{  // this implementation is just duplicating the  name to shortName
    int strLen = strlen(name);
    if (strLen > len)
        return len; // name is bigger than the required len
    strcpy(shortName, name);
    return strLen;
}

bool GetUserName(char* name, unsigned int* len)
{
    // We need to set a default value for the name
    // Assumption is that the buffer has been allocated and a write won't
    // cause us to blow up
    strcpy(name, "EMS");
    // If the calls are successful, then this value will get over-written
    if (0 != getlogin_r(name, *len))
    {
        return false; /* user name not found */

    }
    if (name == NULL)
    {
        return false; /* user name not found */
    }
    if (strlen(name) == 0)
    {
        return false; /* empty user name*/
    }

    *len = strlen(name) + 1;
    return true; /* SUCCESS */
}


char * _ltoa ( long value , char * string , int radix )
{
    sprintf(string, "%lu", value);
    return string;
}

char * _ultoa ( unsigned long value , char * string , int radix )
{
    sprintf(string, "%lu", value);
    return string;
}

char * itoa ( int value , char * string , int radix )
{
    sprintf(string, "%d", value);
    return string;
}

char* _gvct ( double value, int digits, char* buffer)
{
    sprintf(buffer, "%f", value);
    return buffer;
}

#endif
