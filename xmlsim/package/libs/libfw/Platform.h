/**********************************************************************************************
* COPYRIGHT 2001 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Platform.h
*
* Description: 
*	   Holds platform dependent definitions.
*
**********************************************************************************************/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

namespace EMS_FW
{

// Any Microsoft Windows 32-bit platform
#ifdef WIN32
	// Define a Windows constant for exporting and importing
	// DLL functions. This allows header files to be shared
	// by the DLL and associated container EXE.
	#ifdef _CONSOLE
		// Define a constant to easily identify imported functions.
		#define OBJECT_IMPORT_EXPORT _declspec(dllimport)
	#endif

	// If in a DLL project
	#ifdef _USRDLL
		// Define a constant to easily identify exported functions.
		#define OBJECT_IMPORT_EXPORT _declspec(dllexport)
	#endif
#elif EMSUNIX
	#define OBJECT_IMPORT_EXPORT
#endif

}
#endif	// _NBIADMIN_H_
