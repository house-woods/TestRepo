/***********************************************************************************************
* COPYRIGHT Oct 27 1999 - 2004 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: TTime.h
* Type: Include File 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   ttime.h  $
* Version: $Revision:   1.3  $
* Description: 
* TTime class which allows comparison and conversion of time
* 
*
************************************************************************************************/

#ifndef _TTIME_H
#define _TTIME_H

using namespace std;
#include <string>
#include <ospace/time.h>

// Max size for a byte array (string) to store Agent Format Time
// which can be in the following formats: (a) YYYYMMDDHHMMSS.UUZ (b) YYYYMMDDHHMMSS.UZ
// (c) YYYYMMDDHHMMSS.U OR (d) YYYYMMDDHHMMSS.U(+/-)HHSS
#define AGENT_TIME_LENGTH 23

typedef struct timeZoneInfo
{
    string name;
    int hourOffsetFromGMT;
    int minOffsetFromGMT;

    int startDayRank;
    int startWeekDay;
    int startMonth;
    int startHours;
    int startMinutes;

    int endDayRank;
    int endWeekDay;
    int endMonth;
    int endHours;
    int endMinutes;
}
TimeZoneInfo;

namespace EMS_FW
{

class TTime
{
public:
    enum PeriodType { days, hours, mints, secs, millisecs };

public:

    TTime();
    static const char* CompareTime(const char* eventTime, const char* clearTime, const char* acknowledgeTime);
    //returns greater of the three strings
    static const char* CompareTime(const char* eventTime, const char* clearTime);
    //returns greater of two strings and NULL if both strings are equal
    static bool isGreater(const char* eventTime, const char* clearTime);
    //returns true if eventTime is greater than or equal to clear time
    static bool isGreaterYear(const char* eventTime, const char* clearTime);
    //returns true if eventYear is greater than or equal to clear Year
    static bool isExpiredDate(const char* date);
    //returns true if date is greater than current Date.  Otherwise, return false.
    static bool isValid(const char* cTime);
    //returns true if the time is valid
    static int GetWeek(const char* eventTime);
    static bool GetOsiTimeAndDate(const char* ctime, osi_time_and_date& timeDate);
    static void GetTimeForDisplay(const char* time, char* str);
    //This gets UTC time, converts to current local time and then change the format
    //Passed String should have at least size of 23
    static void GetCurrentAgentTime(char* str);
    //This gets current local time, convert to UTC and then change the format
    //Passed String should have at least size of 19
    static void GetTimeInAgentFormat(osi_date today, osi_time Timenow, char* str);
    //Passed String should have at least size of 19
    static void ConvertTimeFromDisplayToAgentFormat(const char* displayTime, char* str);
    //This function converts time in Display format to that in UTC and then agent format
    //Passed String should have at least size of 19
    static void ConvertLocalTimeToAgentFormat(const char* time, char* str);
    //This function converts local time to that in UTC and then agent format
    static bool extractKeyValue(const char* delim, const string& line,
                                string& key, string& value);
    // Added by Shangping
    static bool ConvertFromAgentTimeToSimplifiedGMT(const char* agentTime, char* sGMT);
    // This function converts agent time to the simplified GMT time that client expects.
    // The format for client time is "yyyymmddhhmmss".

    static void periodAddedDate(const char* eventTime, int period, char* str, PeriodType periodType);

    static const int TTime::getDays(const char* eventTime, const char* clearTime);

    static const int TTime::getSeconds(const char* eventTime, const char* clearTime);

    static const int TTime::getSeconds(const char* startTime);

    // returns the differrence in years of the two time inputs
    static const int GetYears(const char* endTime, const char* startTime);

    // get the date of the time input formating as mm/dd/yyyy
    static const string GetDate(const char* eventTime);

    static string GetUTCDateTime (const string &date, const string &time, const string &timeZoneIndex, const string &DSPSetting, const string &format = "");
    static string ConvertTL1DateTimeToAgentTimeFormat (const string &date, const string &time);
    static string FormatTime(string& inputTime);
    static string getCurrentTimeGMTString ();
    static osi_time_and_date getCurrentTimeGMT ();

};

};

#endif // _TTIME_H
