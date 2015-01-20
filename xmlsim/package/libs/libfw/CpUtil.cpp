
/*************************************************************************
* COPYRIGHT 2002 - 2008 Tellabs Operations, Inc.
* ALL RIGHTS RESERVED
* No Part of this document may be reproduced without
* the written permission of Tellabs Operations, Inc.,
* 1415 W Diehl Rd., Naperville, IL 60563
*************************************************************************/

#include "CpUtil.h"
#include "sysErrLog.h"
#include "StringParser.h"
#include "BaseRDbTypes.h"
#include "EmsMoValueT.h"
#include <stdlib.h>


using namespace ELogger;
using namespace std;

extern const char* sourceName;
const char* className = "CpUtil";

//******************************************************************************
// This function, introduced in FP7.0, returns a dotted-decimal ifIndex for the
// given AID or identifier.
// I got this from the Control Plane Low Level SRS for FP4.3 Rev D
//
string CpUtil::convertIfNameToIfIndex(const string& ifName, const int stableCount)
{
    const char* methodName = "convertIfNameToIfIndex";
    string ifIndex;
    char buffer[8];
    
    if (ifName.compare(0,5,"SINTF") == 0)  // If it is SINTF stable (loopback) interface,
    {
        if (ifName.compare(5,2,"-1") == 0)  // If SINTF-1,
        {
            ifIndex = "137.3.17.";
            if (stableCount > 127)    // A little safety precaution
                ifIndex.append("0");
            else
            {
                sprintf(buffer, "%d", stableCount);      // Conforming, more portable than itoa
                ifIndex.append(buffer);
            }
        }
        else if (ifName.compare(5,2,"-2") == 0)  // If SINTF-2,
        {
            // For now we will assume NP=3 and stableCount provides the Node number
            ifIndex = "146.0.";
            unsigned int seg3 = stableCount >> 3;  // Grab the node num and shift it down 3 bits
            sprintf(buffer, "%d", (seg3 & 0x7f));
            ifIndex.append(buffer);
            ifIndex.append(".");
            unsigned int seg4 = stableCount << 5;  // Grab the node num and shift it up 5 bits
            seg4 += 3;   // Add the partition number
            sprintf(buffer, "%d", (seg4 & 0xff));
            ifIndex.append(buffer);
        }
    }
    else if (ifName == "SW-3")
    {
        ifIndex = "137.3.16.140";
    }
    else if (ifName.compare(0,4,"OSCX") == 0)
    {
        if (ifName.compare(5,3,"A-2") == 0)
            ifIndex = "137.3.16.148";
        else if (ifName.compare(5,3,"B-2") == 0)
            ifIndex = "137.3.16.149";
        else if (ifName.compare(5,3,"C-2") == 0)
            ifIndex = "137.3.16.152";
        else if (ifName.compare(5,3,"D-2") == 0)
            ifIndex = "137.3.16.153";
        else if (ifName.compare(5,3,"E-2") == 0)
            ifIndex = "137.3.16.162";
        else if (ifName.compare(5,3,"F-2") == 0)
            ifIndex = "137.3.16.200";
        else if (ifName.compare(5,3,"G-2") == 0)
            ifIndex = "137.3.16.164";
        else if (ifName.compare(5,3,"H-2") == 0)
            ifIndex = "137.3.16.128";
        else
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifName, %s, has an unexpected value.", ifName.c_str());
    }
    else if (ifName.compare(0,3,"PPP") == 0)
    {
        // PPP are an IPv4 Numbered interface with 3-bit Type=100 + 5-bit Subtype=01001 +
        //     7-bit shelf + 5-bit slot + 5-bit port + 7-bit port specific (1=PPPL, 2=PPPS)
        ifIndex = "137.";
        StringParser parts(ifName, "-");
        if (parts.getLength() == 4)
        {
            unsigned int seg2 = atoi(parts.getTokenbyIndex(1).c_str()) << 1;  // Grab the shelf and shift it up 1
            sprintf(buffer, "%d", seg2);
            ifIndex.append(buffer);
            ifIndex.append(".");
            unsigned int seg3 = atoi(parts.getTokenbyIndex(2).c_str()) << 5;  // Grab the slot and shift it up 5 bits
            unsigned int seg4 = atoi(parts.getTokenbyIndex(3).c_str());   // Grab the port
            seg3 = (seg3 + seg4) >> 1;
            sprintf(buffer, "%d", seg3);
            ifIndex.append(buffer);
            if (parts.getTokenbyIndex(0) == "PPPL" || parts.getTokenbyIndex(0) == "PPPMS")
            {
                if (seg4 & 1 == 1)  // I believe this is a good shortcut
                    ifIndex.append(".129");
                else
                    ifIndex.append(".1");
            }
            else  // this must be a PPPS or PPPRS
            {
                if (seg4 & 1 == 1)  // I believe this is a good shortcut
                    ifIndex.append(".130");
                else
                    ifIndex.append(".2");
            }
        }
        else
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifName, %s, has an unexpected value.", ifName.c_str());
    }
    else if (ifName.compare(0,3,"OTS") == 0)
    {
        // OTS IfIndex is type=001 + 7-bit shelf + 5-bit slot + 5-bit port + 12-bit zero
        StringParser parts(ifName, "-");
        if (parts.getLength() == 4)
        {
            unsigned int seg1 = 32;    // Start with type 00100000
            int shelf = atoi(parts.getTokenbyIndex(1).c_str());
            seg1 += shelf >> 2;  // Some bits of shelf go in segment 1
            sprintf(buffer, "%d", seg1);
            ifIndex.append(buffer);
            ifIndex.append(".");
            unsigned int seg2 = shelf << 5;  // Seg 2 has couple bits of shelf making room for slot
            seg2 = (seg2 + atoi(parts.getTokenbyIndex(2).c_str())) << 1;  // Assume ports never take all 5 bits
            sprintf(buffer, "%d", (seg2 & 255));
            ifIndex.append(buffer);
            ifIndex.append(".");
            unsigned int seg3 = atoi(parts.getTokenbyIndex(3).c_str()) << 4;   // Grab the port and shift 4
            sprintf(buffer, "%d", seg3);
            ifIndex.append(buffer);
            ifIndex.append(".0");
        }
        else
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifName, %s, has an unexpected value.", ifName.c_str());
    }
    else  // This should be an OCn, STMn or client OTU
    {
        // OCn/STMn Unnumbered IfIndex is type=000 + 7-bit shelf + 5-bit slot + 5-bit port + 12-bit zero
        StringParser parts(ifName, "-");
        if (parts.getLength() == 4)
        {
            int shelf = atoi(parts.getTokenbyIndex(1).c_str());
            unsigned int seg1 = shelf >> 2;  // Some bits of shelf go in segment 1
            sprintf(buffer, "%d", seg1);
            ifIndex.append(buffer);
            ifIndex.append(".");
            unsigned int seg2 = shelf << 5;  // Seg 2 has couple bits of shelf making room for slot
            seg2 = (seg2 + atoi(parts.getTokenbyIndex(2).c_str())) << 1;  // Assume ports never take all 5 bits
            sprintf(buffer, "%d", (seg2 & 255));
            ifIndex.append(buffer);
            ifIndex.append(".");
            unsigned int seg3 = atoi(parts.getTokenbyIndex(3).c_str()) << 4;   // Grab the port and shift 4
            sprintf(buffer, "%d", seg3);
            ifIndex.append(buffer);
            ifIndex.append(".0");
        }
        else
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifName, %s, has an unexpected value.", ifName.c_str());
    }
    return ifIndex;
}

//******************************************************************************
// This function, introduced in FP8.3, returns a dotted-decimal ifIndex for the
// given TSL parameters.
// I got this from the Control Plane Low Level SRS for FP6.0.2 Rev BP4
//
string CpUtil::convertTSLparamsToIfIndex(const string& moduleAID, const string& portId, const string& facType, const string& channel)
{
    const char* methodName = "convertTSLparamsToIfIndex";
    
    StringParser parts(moduleAID, "-");
    if (parts.getLength() > 2)  // I expect at least 3 or 4 parts like RCMM-1-3, OMD-1-13-2, etc.
    {
        int shelf = atoi(parts.getTokenbyIndex(1).c_str());
        int slot = atoi(parts.getTokenbyIndex(2).c_str());
        int port = atoi(portId.c_str());
        int chan = atoi(channel.c_str());
        return CpUtil::convertTSLparamsToIfIndex(shelf, slot, port, facType, chan);
    }
    else
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                 "The module AID, %s, has an unexpected value.", moduleAID.c_str());
    return "";
}

//******************************************************************************
// This function, introduced in FP8.3, returns a dotted-decimal ifIndex for the
// given TSL parameters.
// I got this from the Control Plane Low Level SRS for FP6.0.2 Rev BP4
//
string CpUtil::convertTSLparamsToIfIndex(int shelf, int slot, int port, const string& facType, int chan)
{
    const char* methodName = "convertTSLparamsToIfIndex";
    string ifIndex;
    char buffer[8];
    
    int portSpec = 2;
    if (facType == "OCH")  // if on the network/multiplexer side of the TSL
        portSpec = 1;

    unsigned int seg1 = 32;    // Start with type 00100000
    seg1 += shelf >> 2;  // Some bits of shelf go in segment 1
    sprintf(buffer, "%d", seg1);
    ifIndex.append(buffer);
    ifIndex.append(".");
    unsigned int seg2 = shelf << 5;  // Seg 2 has couple bits of shelf making room for slot
    seg2 = (seg2 + slot) << 1;
    if (port > 15)
        seg2 += 1;
    sprintf(buffer, "%d", (seg2 & 255));
    ifIndex.append(buffer);
    ifIndex.append(".");
    unsigned int seg3 = port << 4;   // Grab the port and shift 4
    seg3 += chan >> 3;               // plus top 4 bits of channel
    sprintf(buffer, "%d", (seg3 & 255));
    ifIndex.append(buffer);
    ifIndex.append(".");
    unsigned int seg4 = chan << 5;   // Grab the channel and shift 5
    seg4 += portSpec;
    sprintf(buffer, "%d", (seg4 & 255));
    ifIndex.append(buffer);
    return ifIndex;
}

//******************************************************************************
// This function, introduced in FP7.0, returns info about an ifIndex to validate
// it or to try to determine the IfName or Resource.  You will probably need a
// database query to figure out exactly what it is though.
// I got this from the Control Plane Low Level SRS for FP4.3 Rev D
//
bool CpUtil::convertIfIndexToTypeAndNumbers(const string& ifIndex, string& type, int& shelf, int& slot, int& port)
{
    const char* methodName = "convertIfIndexToTypeAndNumbers";
    StringParser p(ifIndex, ".");
    if (p.getLength() != 4)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                 "The ifIndex, %s, has an unexpected value. Expecting 'x.x.x.x' form.", ifIndex.c_str());
        return false;
    }
    
    // Make it into a 32-bit integer
    unsigned int ifIndexInt = (atoi(p.getTokenbyIndex(0).c_str()) << 24) | 
                              (atoi(p.getTokenbyIndex(1).c_str()) << 16) | 
                              (atoi(p.getTokenbyIndex(2).c_str()) << 8) | 
                              (atoi(p.getTokenbyIndex(3).c_str()));
                              
    if (ifIndex.compare(0,9,"137.3.17.") == 0)
    {
        type = "SINTF-1";
        shelf = 0;
        slot = 0;
        port = ifIndexInt & 0x7f;
        return true;
    }
    else if (ifIndex.compare(0,9,"137.3.16.") == 0)
    {
        type = "OSCX";
        shelf = 0;
        slot = 0;   
        // I know this can be a bunch of other specific port types but no one needs to distinguish them right now.
        int portType = ifIndexInt & 0x7f;  // bottom 7 bits
        if (portType == 12)
        {
            type = "SW-3";
            port = 0;
        }
        else if (portType == 20)
            port = 1;   // OSCX-A-2
        else if (portType == 21)
            port = 2;   // OSCX-B-2
        else if (portType == 24)
            port = 3;   // OSCX-C-2
        else if (portType == 25)
            port = 4;   // OSCX-D-2
        else if (portType == 34)
            port = 5;   // OSCX-E-2
        else if (portType == 35)
            port = 6;   // OSCX-F-2
        else if (portType == 36)
            port = 7;   // OSCX-G-2
        else if (portType == 37)
            port = 8;   // OSCX-H-2
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifIndex, %s [%x], is a Router type we do not recognize. Port type value = %d",
                                     ifIndex.c_str(), ifIndexInt, (ifIndexInt & 0x7f));
            return false;
        }
        return true;
    }
    else if (ifIndex.compare(0,4,"137.") == 0)  
    {
        if ((ifIndexInt & 0x7f) == 1)
            type = "PPPL";   
        else if ((ifIndexInt & 0x7f) == 2)
            type = "PPPS";  
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifIndex, %s [%x], has an unexpected value. port type value (%x) out of range",
                                     ifIndex.c_str(), ifIndexInt, (ifIndexInt & 0x7f));
            return false;
        } 
        shelf = (ifIndexInt >> 17) & 0x1f;
        slot = (ifIndexInt >> 12) & 0x1f;
        port = (ifIndexInt >> 7) & 0x1f;
        if (shelf == 0 || slot == 0 ||  port == 0)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifIndex, %s, has an unexpected value. shelf, slot, or port value is zero", ifIndex.c_str());
            return false;
        } 
        return true;
    }
    else if (ifIndex.compare(0,4,"146.") == 0)  
    {
        type = "SINTF-2";
        shelf = 0;
        slot = (ifIndexInt >> 5) & 0x3ff;  // Node number
        port = ifIndexInt & 0x1f;  // partition number
        return true;
    }
    else // Better turn out to be an OCn/STMn type
    {
        int iType = (ifIndexInt & 0xe0000000) >> 29;   // Type is the top 3 bits
        if (iType == 0 && (ifIndexInt & 0x00000fff) == 0)  // This is unnumbered transport (OCn, STMn or OTUk)
        {
            type = "Transport";   
            shelf = (ifIndexInt >> 22) & 0x7f;
            slot = (ifIndexInt >> 17) & 0x1f;
            port = (ifIndexInt >> 12) & 0x1f;
            if (shelf == 0 || slot == 0 ||  port == 0)
            {
                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                         "The ifIndex, %s, has an unexpected value. shelf, slot, or port value is zero", ifIndex.c_str());
                return false;
            } 
            return true;
        }
        else if (iType == 1)  // This is an unnumbered OTS, OCH or ODUk transponder link
        {
            int iPortSpecific = (ifIndexInt & 0x0000001f);   // bottom 5 bits
            if (iPortSpecific == 0)
                type = "OTS";
            else if (iPortSpecific == 1)
                type = "OCH";
            else if (iPortSpecific == 2)
                type = "OCH-P";
            else
            {
                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                         "The ifIndex, %s, has an unexpected value. the port specific value is %d", ifIndex.c_str(), iPortSpecific);
                return false;
            } 
            shelf = (ifIndexInt >> 22) & 0x7f;
            slot = (ifIndexInt >> 17) & 0x1f;
            port = (ifIndexInt >> 12) & 0x1f;
            // channel = (ifIndexInt >> 5) & 0x7f;
            if (shelf == 0 || slot == 0 ||  port == 0)
            {
                ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                         "The ifIndex, %s, has an unexpected value. shelf, slot, or port value is zero", ifIndex.c_str());
                return false;
            } 
            return true;
        }
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                     "The ifIndex, %s, seems to be an unimplemented or reserved type", ifIndex.c_str());
            return false;
        }
    }
    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                             "The ifIndex, %s, has an unexpected value. Fell out of all cases", ifIndex.c_str());

    return false;
}

//******************************************************************************
//  converts something like 0.132.16.0 to 00841000
string CpUtil::convertDottedDecimalToHexidecimal(const string& dottedDec)
{
    const char* methodName = "convertDottedDecimalToHexidecimal";
    StringParser p(dottedDec, ".");
    if (p.getLength() != 4)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                 "The input, %s, has an unexpected value. Expecting 'x.x.x.x' form.", dottedDec.c_str());
        return "";
    }
    
    // Make it into a 32-bit integer
    unsigned int intVal = (atoi(p.getTokenbyIndex(0).c_str()) << 24) | 
                          (atoi(p.getTokenbyIndex(1).c_str()) << 16) | 
                          (atoi(p.getTokenbyIndex(2).c_str()) << 8) | 
                          (atoi(p.getTokenbyIndex(3).c_str()));
    char buffer[16];
    sprintf(buffer, "%08X", intVal);
                              
    return buffer;
}

//******************************************************************************
//  converts something like 01641000 to 1.100.16.0
string CpUtil::convertHexidecimalToDottedDecimal(const string& hexStr)
{
    const char* methodName = "convertHexidecimalToDottedDecimal";
    if (hexStr.length() != 8 || hexStr.find_first_not_of("0123456789ABCDEFabcdef") != string::npos)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                 "The input, %s, has an unexpected value. Expecting eight hexidecimal digits.", hexStr.c_str());
        return "";
    }
    
    // Make it into a 32-bit integer
    unsigned long intVal = strtoul(hexStr.c_str(), NULL, 16);
    char buffer[16];
    sprintf(buffer, "%d", (intVal >> 24) & 0xff);
    string ifIndex(buffer);
    ifIndex.append(".");
    sprintf(buffer, "%d", (intVal >> 16) & 0xff);
    ifIndex.append(buffer);
    ifIndex.append(".");
    sprintf(buffer, "%d", (intVal >> 8) & 0xff);
    ifIndex.append(buffer);
    ifIndex.append(".");
    sprintf(buffer, "%d", (intVal) & 0xff);
    ifIndex.append(buffer);
                              
    return ifIndex;
}

//******************************************************************************
// convert CP ifIndex to AID
// this function is ported from client function EntityManagementHelper.translateIfIndexToSSP
string CpUtil::convertIfIndexToSSP(const string& ifIndex)
{
    const char* methodName = "convertIfIndexToSSP";

    StringParser p(ifIndex, ".");
    if (p.getLength() != 4)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MINOR,
                                 "The ifIndex, %s, has an unexpected value. Expecting 'x.x.x.x' form.", ifIndex.c_str());
        return false;
    }
    
    // Make it into a 32-bit integer
    unsigned int ifIndexNo = (atoi(p.getTokenbyIndex(0).c_str()) << 24) | 
                             (atoi(p.getTokenbyIndex(1).c_str()) << 16) | 
                             (atoi(p.getTokenbyIndex(2).c_str()) << 8) | 
                             (atoi(p.getTokenbyIndex(3).c_str()));
    
    int ifType = (ifIndexNo >> 29) & 7;
    if (ifType == 0)  // If unnumbered
    {
        string aid;
        int shelfNo = (ifIndexNo >> 22) & 0x7F;            
        int slotNo = (ifIndexNo >> 17) & 0x1F;            
        int portNo = (ifIndexNo >> 12) & 0x1F;            
        int portSpec = ifIndexNo & 0xFFF; 
        if (portSpec == 0)
            aid = "TP-";
        else if (portSpec == 1)
            aid = "PPPL-";
        else if (portSpec == 2)
            aid = "PPPS-";
        else 
            aid = "SPM-";
        char buffer [33];
        sprintf(buffer, "%d", shelfNo);
        aid.append(buffer);
        aid.append("-");
        sprintf(buffer, "%d", slotNo);
        aid.append(buffer);
        aid.append("-");
        sprintf(buffer, "%d", portNo);
        aid.append(buffer);
        if (portSpec > 2)
        {
            aid.append("-");
            sprintf(buffer, "%d", portSpec);
            aid.append(buffer);
        }
        return aid;
    }
    else if (ifType == 1)  // else if unnumbered OTS or OCH
    {
        string aid;
        int shelfNo = (ifIndexNo >> 22) & 0x7F;            
        int slotNo = (ifIndexNo >> 17) & 0x1F;            
        int portNo = (ifIndexNo >> 12) & 0x1F;            
        int chanNo = (ifIndexNo >> 5) & 0x7F;            
        int portSpec = ifIndexNo & 0x1F; 
        if (portSpec == 0)
            aid = "OTS-";
        else if (portSpec == 1)
            aid = "OCH-";
        else if (portSpec == 2)
            aid = "OCH-P-";
        else 
            aid = "TSL-";
        char buffer [33];
        sprintf(buffer, "%d", shelfNo);
        aid.append(buffer);
        aid.append("-");
        sprintf(buffer, "%d", slotNo);
        aid.append(buffer);
        aid.append("-");
        sprintf(buffer, "%d", portNo);
        aid.append(buffer);
        if (portSpec == 1 || chanNo > 0)
        {
            aid.append("-");
            sprintf(buffer, "%d", chanNo);
            aid.append(buffer);
        }
        return aid;
    }
    else if (ifType == 4)  // else if IPV4 numbered interfaces
    {
        int subType = (ifIndexNo >> 24) & 0x1F; 
        if (subType == 9)
        {
            int shelfNo = (ifIndexNo >> 17) & 0x7F;            
            int slotNo = (ifIndexNo >> 12) & 0x1F;            
            int portNo = (ifIndexNo >> 7) & 0x1F;            
            int portSpec = ifIndexNo & 0x7F;
            string aid;
            if (portSpec <= 2)
            {
                if (portSpec == 1)
                    aid = "PPPL-";
                else if (portSpec == 2)
                    aid = "PPPS-";
                else
                    aid = "PORT-";
                char buffer [33];
                sprintf(buffer, "%d", shelfNo);
                aid.append(buffer);
                aid.append("-");
                sprintf(buffer, "%d", slotNo);
                aid.append(buffer);
                aid.append("-");
                sprintf(buffer, "%d", portNo);
                aid.append(buffer);
                return aid;
            }
            else if (shelfNo == 1 && slotNo == 17)
            {
                if (portNo == 2)
                    return "SINTF-1";
                else
                {
                    if (portSpec == 12)
                        return "SW-3";
                    else if (portSpec == 20)
                        return "OSCX-A-2";
                    else if (portSpec == 21)
                        return "OSCX-B-2";
                    else if (portSpec == 24)
                        return "OSCX-C-2";
                    else if (portSpec == 25)
                        return "OSCX-D-2";
                    else if (portSpec == 37)
                        return "OSCX-E-2";
                    else if (portSpec == 38)
                        return "OSCX-F-2";
                    else if (portSpec == 39)
                        return "OSCX-G-2";
                    else if (portSpec == 40)
                        return "OSCX-H-2";
                    else if (portSpec == 16)
                        return "RT-1";
                    else if (portSpec == 17)
                        return "RT-2";
                    else if (portSpec == 35)
                        return "RT-3";
                    else if (portSpec == 36)
                        return "RT-4";
                    else
                    {
                        char buffer [33];
                        sprintf(buffer, "%d", portSpec);
                        string rtnVal = "UNKNOWN PS ";
                        rtnVal.append(buffer);
                        return rtnVal;
                    }
                }
            }
            return "NUM UNKNOWN";
        }
        else if (subType == 18)  // SINTF-2
        {
            int nodeNo = (ifIndexNo >> 5) & 0x3FF;            
            int partition = ifIndexNo & 0x1F;
            string aid = "SINTF-2 NODE-";
            char buffer [33];
            sprintf(buffer, "%d", partition);
            aid.append(buffer);
            aid.append("-");
            sprintf(buffer, "%d", nodeNo);
            aid.append(buffer);
            return aid;
        }
        else if (subType == 16)  // BL
        {
            int blNo = (ifIndexNo >> 15) & 0x1FF;            
            int nodeNo = (ifIndexNo >> 5) & 0x3FF;            
            int partition = ifIndexNo & 0x1F;
            string aid("BL-");
            char buffer [33];
            sprintf(buffer, "%d", partition);
            aid.append(buffer);
            aid.append("-");
            sprintf(buffer, "%d", nodeNo);
            aid.append(buffer);
            aid.append("-");
            sprintf(buffer, "%d", nodeNo);
            aid.append(buffer);
            return aid;
        }
        else
            return "NUM UNKNOWN";
    }
    else if (ifType == 7)  // abstract
    {
        return "ABSTRACTNODE";
    }
    else
        return "UNKNOWN";
}

//******************************************************************************
// convert CP Label to time slot number
// this function is ported from client function EntityManagementHelper.translateLabelToTimeslot
string CpUtil::convertLabelToTimeslot(const string& signalType, const string& cpLabel)
{
    int nTS = 0;
    unsigned int nLabel = strtol(cpLabel.c_str(),NULL, 16);
    if (signalType.compare(0,3,"VC4")==0)
    {
        nTS = (short) ((nLabel >> 16) & 0xFFFF);
    }
    else if (signalType.compare("VC3")==0 || signalType.compare("STS1")==0 )
    {
        int uRange = (nLabel >> 12) & 0xF;
        int sRange = (nLabel >> 16) & 0xFFFF;
        nTS = ((sRange - 1) * 3 + 1) + uRange - 1;
    }
    else if (signalType.compare(0,3,"STS")==0)
    {
        nTS = (((nLabel >> 16) & 0xFFFF) - 1) * 3 + 1;
    }
    else if (signalType.compare(0,3,"ODU")==0)
    {
        if ((nLabel >> 29 & 0x7) > 0)  // An OCH/OCH-L label should have 1 for DWDM
        {
            short chanSpacing = (short) (nLabel >> 25 & 0xF);
            short nVal = (short) (nLabel & 0xFFFF);
            if (chanSpacing == 1)
            {
                nTS = nVal + 15;
            }
            else if (chanSpacing == 2)
            {
                if ((nVal % 2) == 1)    // odd values represent chans 45 - 88
                    nTS = (nVal + 29) / 2 + 44;
                else  // even values represent chans 1 - 44
                    nTS = (nVal + 30) / 2;
            }
        }
        else  // Otherwise, assume ODU type label
        {
            if (nLabel == 1 || nLabel == 2 || nLabel == 0x0010)
                nTS = 1;  // Unmultiplexed
        }
    }
    char buffer [33];
    sprintf(buffer, "%d", nTS);
    string ts = buffer;
    return ts;
}

//******************************************************************************
// convert CP interface to AID
// this function is ported from client function EntityManagementHelper.translateInterfaceToAID
string CpUtil::convertInterfaceToAID(const string& signalType, const string& ingressInterface)
{
    string aid;
    StringParser parts(ingressInterface, "-");
    if (parts.getLength() == 2)
    {
        string ifIndex = convertHexidecimalToDottedDecimal(parts.getTokenbyIndex(0));
        string label = parts.getTokenbyIndex(1);    
        string ifAID = convertIfIndexToSSP(ifIndex);

        if (signalType.compare(0,3,"ODU")==0 && ifAID.compare(0,3,"TP-")==0)
        {
            if (signalType.compare(0,4,"ODU2")==0)
                aid = "OTU2";
            else
                aid = "OTU3";
            aid.append(ifAID.substr(2));
        }
        else if (ifAID.compare(0,4,"OTS-")==0)
        {
            string ts = convertLabelToTimeslot(signalType, label);
            aid = "OCH-L";
            aid.append(ifAID.substr(3));
            aid.append("-");
            aid.append(ts);
        }
        else if (signalType.compare(0,2,"VC")==0 || signalType.compare(0,3,"STS")==0)
        {
            string ts = convertLabelToTimeslot(signalType, label);
            aid = signalType;
            aid.append(ifAID.substr(2));
            aid.append("-");
            aid.append(ts);
        }
        else
        {
            aid = ifAID;
        }
    }
    return aid;
}

