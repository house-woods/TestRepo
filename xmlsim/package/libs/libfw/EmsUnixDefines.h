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


#ifndef _EMSUNIXDEFINES_H_
#define _EMSUNIXDEFINES_H_

void Ems_static_init();

void Ems_static_cleanup();

#ifdef EMSUNIX
// here are all definitions required for unix build

#include <unistd.h>  // needed for some definitions like gethostname()

#include<stdio.h> 
#include<stdarg.h> 


#define _vsnprintf vsnprintf 



#define _snprintf snprintf


#define _chdir(x)  chdir(x)

#define _stricmp strcasecmp

#define stricmp strcasecmp

#define _strdup strdup

#define _itoa itoa
#define __itoa itoa

#define _ltoa ltoa


int GetShortPathName(const char* name, char* shortName, int len);

bool GetUserName(char* name, unsigned int* len);

char * _ltoa ( long value , char * string , int radix ) ; 

char * _ultoa ( unsigned long value , char * string , int radix ) ; 

char * itoa ( int value , char * string , int radix ) ; 

char* _gvct ( double value, int digits, char* buffer);


#endif

#endif
