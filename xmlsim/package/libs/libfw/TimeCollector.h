

/***********************************************************************************************
* COPYRIGHT $Date:   July 23 2000 14:39:38  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TimeCollector 
* Type: H
* Originator: $Author:  
* File: $Workfile:  
* Version: $Revision:   
* Description: 
* Utility class used to collect time reports for debugging purposes.
* 
* Revision History:
*
* $Log:  
*
* File Dependencies: 
* 
************************************************************************************************/


#ifndef _TIMECOLLECTOR_H_
#define _TIMECOLLECTOR_H_

#include <string>
#include <map>
#include <ospace/time.h>
#include <ospace/file.h>
#include <ospace/thread.h>

using namespace std;

namespace EMS_FW
{

typedef osi_sem_lock<osi_mutex_semaphore> MessageLock;			

class TimeCollector
{
public:

	static void initialize (bool precisionProfilingEnabled, const string &profileLogFilename="");
	static void initializeDefaultProfile (bool precisionProfilingEnabled, const string &profileLogFilename="");
	static void finalize (void);
	static void finalize (const string& profileLogFilename);

	static void start(const string& key, const string& profileLogFilename="DEFAULT");
	static void end(const string& key, const string& profileLogFilename="DEFAULT");
	static void collect(const string& key);
	static void suspend(const string& key);

private:

	struct CollectTime
	{
		unsigned int startTime;
		unsigned int suspendedTime;
		unsigned int suspendTime;
		osi_stopwatch stopWatch;
	};

	static bool _precisionProfilingEnabled;
	static map<string, bool> _precisionProfilingEnabledMap;

	static osi_file *_profileLog;

	static map<string, osi_file*> _profileLogMap;

	typedef map<string, CollectTime, less<string> > timeMapType;

	static timeMapType _timeMap;

	static void printReport(const string& key,CollectTime ct, const string& profileLogFilename);

	static osi_mutex _messageMutex;

};

};

#endif


