
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:44  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: EmsProcess
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EmsProcess.h  $
* Version: $Revision:   1.2  $
* Description: 
* Encapsulates the process management API
* 
* Revision History:
*
*
**********************************************************************************************/


#ifndef _EMSPROCESS_H_
#define _EMSPROCESS_H_



#if defined(WIN32)

#include <windows.h>

#else

#include <ospace/unix/process.h>
#include <ospace/unix.h>

#endif


#include <string>
#include <stdlib.h>
#include <ospace/file.h>
//#include "ProxyLogger.h"

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

namespace EMS_FW
{

class EmsProcess
{
public:

	EmsProcess( bool use_fork1 = true );

	~EmsProcess();

	 // Create process and block waiting the process to finish if waitToFinish = true.
	 // If visible is true the process run in its own console otherwise is runing as a daemon
	 bool createProcess(const string& execDir,const string& cmd, const string& args, bool visible = false, bool waitToFinish = false,
	 		    const string& startConsoleCmd = "", int writeHandle = NULL);
	 

	 bool terminateProcess();

	 static bool setEnvorinmentVariable(const string& name,const string& value);

	 static bool getEnvironmentVariable(const string& name, string& value);

	 unsigned int getProcessId() const; 

	 bool connectToProcess(unsigned int pid);

	 // return false if process is not started yet
	 bool getProcessTime(string& time) const; 

	 static bool getProcessTimeByPID(unsigned int pid, string& time); 

	 static bool isProcessRunning(unsigned int pid);

	 static EmsProcess* getProcess(unsigned int pid);
	 
	 static bool system(const string& cmd, bool inSeparateThread=false);


private:


#if defined(WIN32)
	unsigned int _pid;
	HANDLE m_proxyHandler;
#else
	osi_process* _process;
	bool 		 _fork_alone;
#endif

	 struct _SystemThreadArgType {
	   _SystemThreadArgType(const char* command, osi_process_status* procStatus) :
		   _command(command), _procStatus(procStatus)
		  {}
	   const char* _command;
		 osi_process_status* _procStatus;
	 };
   static void* EmsProcess::_systemThreadFunc(void* arg);
};

};

#endif
