/*++*************************************************************************
$Header: /R5/UHCpCl/h/defines.h 3     2/15/02 12:52p Sr $

This is a part of the UH Communications Aps UHCpCl library

Copyright (c) 1995-1996  UH Communications Aps, Denmark
All rights reserved.

Module Name:

    defines

Abstract:

    This file contains various defines that substitutes the ones used in
    ms windows when compiling UHCpCl under unix$$$

Revision History:
    $Log: /R5/UHCpCl/h/defines.h $
 * 
 * 3     2/15/02 12:52p Sr
 * PR:5.0951
 * 
 * 2     6-11-01 14:35 Uh
 * p:ruling out ftime
 * 
 * 1     2-03-99 20:47 Uffe Harksen
 * R 5.0 moved from PVCS
 * 
 * 1     2-03-99 20:43 Uffe Harksen
 * The basic UHCpCl Library R. 5.0
 * 
 *    Rev 1.1   Jul 18 1997 18:50:18   Uffe Harksen
 * P:400.120
 * 
 *    Rev 1.0   Apr 12 1996 11:16:30   Uffe Harksen
 *  
*************************************************************************--*/

#ifndef __defines_h
#define __defines_h

#include <synch.h>
#include <sys/timeb.h>
#ifndef _timeb
#define _timeb timeb
#endif 
#ifndef _ftime
#define _ftime ftime_r
extern "C" {
int ftime(struct _timeb * timeptr);
int ftime_r(struct _timeb * timeptr);
}
#endif

#define PASCAL  //not relevant in unix
#define _DLLMOD // ditto
#define _CRTIMP  // ditto
#define __stdcall // ditto
#define _cdecl   // ditto
#define __cdecl // ditto
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#ifndef BASETYPES
#define BASETYPES
typedef long LONG;
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef void * LPVOID;
typedef char *PSZ;
#endif  /* !BASETYPES */
 
#define MAX_PATH          260
#define _HEAPOK		(-2) 
 
#ifndef FALSE
#define FALSE               0
#endif
 
#ifndef TRUE
#define TRUE                1
#endif
 
#ifndef IN
#define IN
#endif
 
#ifndef OUT
#define OUT
#endif


typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
 
#ifndef NOMINMAX
 
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
 
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif //NOMINMAX
 

#if defined(SOLARIS) && !defined(PTHREADS)
/* begin Solaris threads */
#ifndef CRITICAL_SECTION 
#define CRITICAL_SECTION mutex_t
#endif

#define THREADHANDLE thread_t

#ifndef SEMAPHORE
#define SEMAPHORE sema_t
#endif

#ifndef EVENT 
#define EVENT cond_t
#endif

#endif
/* end Solaris threads */

//#if (defined(SOLARIS) || defined(LINUX)) && defined(PTHREADS)
#ifdef PTHREADS
/* begin POSIX threads */
//PTHREADS not defined here when compiling OMGIDL
#ifndef CRITICAL_SECTION
#define CRITICAL_SECTION pthread_mutex_t
#endif

#define THREADHANDLE pthread_t

#ifndef SEMAPHORE
#define SEMAPHORE sem_t
#endif

#ifndef EVENT
#define EVENT pthread_cond_t
#endif

#endif /* end POSIX threads */



#ifndef LPSECURITY_ATTRIBUTES
#  define LPSECURITY_ATTRIBUTES void*
#endif
#ifndef EVENT
#  define EVENT cond_t
#endif
#ifndef EVENTHANDLE
#  define EVENTHANDLE void*
#endif
#ifndef THREADHANDLE
#  define THREADHANDLE thread_t
#endif

#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strincmp
#define strincmp strncasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#if defined(LINUX)
#define SOCKLENTYPE socklen_t*
#else
#define SOCKLENTYPE 
#endif


#define Sleep(x) sleep(x/1000)  //Sleep is mswin specific sleep for millisecs

#endif 

