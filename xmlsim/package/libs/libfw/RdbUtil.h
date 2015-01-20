/*****************************************************************************************
* COPYRIGHT 2004 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd, Naperville, IL 60563 U.S.A.
*
* Rev 1.0  12/33/2004  lzou
* the memeory representation of REF_ENTITY table
*****************************************************************************************/



#ifndef _RdbUtil_H__
#define _RdbUtil_H__

#include <string>
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif


namespace EMS_FW
{

class RdbUtil
{
public:

    static string getEntityIdandType (const string& neId, const string& aid, string& entityType);

    static string getEntityId (const string& neId, const string& aid);

    static string get7120EntityIdandType (const string &neId, const string &aid, string &entityType);

    static string getNGXEntityId (const string &neId, const string &aid);

    static string getEntityIdandType (const string& neId, const string& aidType,
                                      const string& shelfNo, const string& slotNo,
                                      const string& portNo, const string& chNo,
                                      string& entityType);

    static string getEntityId (const string& neId, const string& aidType,
                               const string& shelfNo, const string& slotNo,
                               const string& portNo, const string& chNo, const string& levelNo = "");

    static string getEntityTypeFromAid(const string& aid);

    static string getAidTypeFromId(const string& entityId);
    static string getAidTypeFromId(const string& entityId, const string& neid);

    static string getEntityTypeFromId(const string& entityId);
    static string getEntityTypeFromId(const string& entityId, const string& neid);


    static bool parseEntityId (const string& entityId, const string& neId, string& aidType,
                               string& shelfNo, string& slotNo,
                               string& portNo, string& chNo, string& levelNo = string(""));

    static bool parseEntityIdNumbers (const string& entityId, string& shelfNo, string& slotNo,
                                      string& portNo, string& chNo, string& levelNo = string(""));

    static string getAidAndType (const string& id, string& entityType, const string& neid = "");

    static string getAidFromId (const string& id);

    static string getInternalType (const string& id);

    static string getAidTypeAndComponents(const string& aid, string& shelfNo, string& slotNo,
                                          string& portNo, string& chNo, string& levelNo = string(""));

    static bool checkIfEntityExists(const string& table, const string& idColName, const string& id);
    static string getNeType(const string& neId);
    static string getShelfType(const string& neId, const string& shelfNo);
    static bool isCPE(const string& neId);
    static bool is7305NE(const string& neId);
    static bool is7100E(const string& neId);
    static bool isCPENE(const string& neType);
    static void getRcmmAidbySide(const string& neId, const string& side, string& aid);

    static string getNePreferenceNav(const string& neId);

    static string getEntityIdandType (const string& neId, const string& aidType,
                                      const string& shelfNo, const string& slotNo,
                                      const string& portNo, const string& chNo, const string& levelNo,
                                      string& entityType);
    static void retrieveDefaultProvisionAttributes(const string& attrName, const string& attrSubType, string& attrValue);
private:


    RdbUtil()
    {}

    ~RdbUtil()
    {}


}
; // end of class RdbUtil

}
; // end of namespace EMS_FW

#endif
