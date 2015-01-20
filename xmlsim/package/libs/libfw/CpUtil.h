/*************************************************************************
* COPYRIGHT 2009 Tellabs Operations, Inc. 
* ALL RIGHTS RESERVED 
* No Part of this document may be reproduced without 
* the written permission of Tellabs Operations, Inc., 
* 1415 W Diehl Rd., Naperville, IL 60563
*************************************************************************/

#ifndef CpUtil_h
#define CpUtil_h 1

#include <string>
#include <map>

using namespace std;

class CpUtil
{
public:

    static string convertIfNameToIfIndex(const string& ifName, const int stableCount);

    static string convertTSLparamsToIfIndex(const string& moduleAID, const string& portId, const string& facType, const string& channel);
    static string convertTSLparamsToIfIndex(int shelf, int slot, int port, const string& facType, int channel);

    static bool convertIfIndexToTypeAndNumbers(const string& ifIndex, string& type, int& shelf, int& slot, int& port);

    static string convertDottedDecimalToHexidecimal(const string& dottedDec);
    static string convertHexidecimalToDottedDecimal(const string& hexStr);

    // ported from client codes
    static string convertIfIndexToSSP(const string& ifIndex);
    static string convertLabelToTimeslot(const string& signalType, const string& cpLabel);
    static string convertInterfaceToAID(const string& signalType, const string& ingressInterface);
};

#endif

