#include "RDbFactory.h"
//#include<memory>
#include "RdbUtil.h"
#include "RdbRefEntityTable.h"
#include "sysErrLog.h"
#include "StringParser.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include <ospace/helper.h>
#include "tutil.h"

#include <stdio.h>
#include <stdlib.h>

using namespace ELogger;
using namespace EMS_FW;
using namespace std;
using namespace RDb;
extern const char* sourceName;
static const char* objectName = "RdbUtil";



//******************************************************************************
// This form of get7120EntityId tries to break down the AID as generically as
// possible to avoid too much future modification.  The subcomponents are all
// converted to fixed length numeric values and concatenated to for the Id.
//
string RdbUtil::get7120EntityIdandType (const string &neId, const string &aid, string &entityType)
{
    ErrorLogger::logVarError(sourceName, objectName, "get7120EntityIdandType", __LINE__,
                             TRACE1, "Attempting to convert AID [%s]", aid.c_str());
    if (aid.empty())
    {
        ErrorLogger::logVarError(sourceName, objectName, "get7120EntityIdandType", __LINE__,
                                 MINOR, "Blank AID supplied; returning blank id");
        return "";
    }

    string aidtype, shelf, row, position, facility, channel;

    osi_tokenizer t ("-", false, "", "\n", "", false);
    const vector<string> &tokens = t.tokenize (aid);

    if (tokens.size())
    {
        // first token is the aid-type
        vector<string>::const_iterator i = tokens.begin();
        aidtype = (*i);
        i++;
        while (i != tokens.end())
        {
            string token = (*i);
            switch (token.c_str()[0])
            {
            case 'S':
                shelf = token.substr (1);
                break;
            case 'R':
                // if shelf has not been parsed yet, then this is part of the AIDtype
                if (shelf.empty())
                    aidtype += "-" + token;
                else
                    row = token.substr (1);
                break;
            case 'P':
                if (shelf.empty())
                    aidtype += "-" + token;
                else
                    position = token.substr (1);
                break;
            case 'F':
                if (shelf.empty())
                    aidtype += "-" + token;
                else
                    facility = token.substr (1);
                break;
            default:
                // if all qualifiers are empty, then we haven't gotten to the shelf etc yet
                if ( (shelf.empty()) && (row.empty()) && (position.empty()) && (facility.empty()) )
                    aidtype += "-" + token;
                else
                {
                    if ((token.c_str()[0] <= 'Z') && (token.c_str()[0] >= 'A'))
                        channel = Tutil::numberToString (token.c_str()[0] - 'A' + 1);
                    else
                        channel = token;
                }
            }
            i++;
        }
    }

    string id = getEntityIdandType(neId, aidtype, shelf, row + position, facility, channel, entityType);

    ErrorLogger::logVarError(sourceName, objectName, "get7120EntityIdandType", __LINE__,
                             TRACE1, "Converted  AID [%s] to  id [%s]  entity-type [%s]", aid.c_str(), id.c_str(), entityType.c_str());

    return id;
}

//******************************************************************************
// getNGXEntityId converts a 5500 NGX AID to a unique numerical form
//
// Forms are like:  SYS-NE-COM, SH1-NE-COM, SYS-IP-n, SHn-SNMP-n, SH1-ENV-n,
//                  SHx-INT-n, SHx-EXT-n, SHx-STU-n,
//                  SH1-SLOT-11
//                  SH1-12-4
//                  SH1-nn-nn-nn-nn-nn
//
string RdbUtil::getNGXEntityId (const string &neId, const string &aid)
{
    const char * methodName = "getNGXEntityId";
    ErrorLogger::logVarError(sourceName, objectName, methodName,
                             TRACE3, "Attempting to convert AID [%s]", aid.c_str());
    if (aid.empty())
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__,
                                 MINOR, "Blank AID supplied; returning blank id");
        return "";
    }

    osi_tokenizer t ("-", false, "", "\n", "", false);
    const vector<string> &tokens = t.tokenize (aid);

    if (tokens.empty() || aid.find("-NE-COM") != string::npos)
        return neId;

    string entityId(neId);
    entityId.append("999");   // A place holder for the Entity type just in case.

    try
    {
        for (vector<string>::const_iterator tIter = tokens.begin(); tIter != tokens.end(); ++tIter)
        {
            if ((*tIter).compare(0, 2, "SH") == 0)
            {
                const string& shelf = (*tIter).substr(2);
                if (shelf.length() == 1)
                    entityId.append("0");
                entityId.append(shelf);
            }
            else if ((*tIter) == "SYS") // Found SYS instead of SHx
            {
                entityId.append("00");
            }
            else if ((*tIter).at(0) > '9' || (*tIter).at(0) < '0')  // Some other non-numeric
            {
                if ((*tIter) == "ENV")  // These are probably in the slot position
                    entityId.append("50");
                else if ((*tIter) == "IP")
                    entityId.append("51");
                else if ((*tIter) == "SNMP")
                    entityId.append("52");
                else if ((*tIter) == "ULSDCC")
                    entityId.append("53");
                else if ((*tIter) == "LLSDCC")
                    entityId.append("54");
                else if ((*tIter) == "INT")
                    entityId.append("61");
                else if ((*tIter) == "EXT")
                    entityId.append("62");
                else if ((*tIter) == "STU")
                    entityId.append("63");
                else if ((*tIter) != "SLOT")
                    ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MINOR,
                                             "Unexpected literal in AID = %s", aid.c_str());
            }
            else
            {
                if ((*tIter).length() == 1)
                    entityId.append("0");
                entityId.append((*tIter));
            }
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, methodName, __LINE__, MAJOR,
                                 "Unexpected exception caught converting AID = %s (ID = %s so far)", aid.c_str(), entityId.c_str());
    }

    ErrorLogger::logVarError(sourceName, objectName, methodName,
                             TRACE3, "Converted  AID [%s] to  id [%s] ", aid.c_str(), entityId.c_str());

    return entityId;
}

//******************************************************************************
// This form of getEntityId tries to break down the AID as generically as
// possible to avoid too much future modification.  The subcomponents are all
// converted to fixed length numeric values and concatenated to for the Id.
//
// The basic form of an AID is:
//    Type-Shelf-slot-(port|subslot)-(channel|STS)
//
// The type can contain a hyphen, as in OCH-L.  You'll see this handled below.
//
string RdbUtil::getEntityIdandType(const string& neId, const string& aid, string& entityType)
{
    string shelfNo, slotNo, portNo, chNo, levelNo;
    int index = 1;

    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType", __LINE__,
                             TRACE7, "convert from AID = %s", aid.c_str());

    StringParser p(aid, "-");
    int len = p.getLength();
    
    const string& neType = Server_i::instance()->getDefaultConfigValue (ConfigNames::NeTypeKey, neId, "OTS");
    if (neType == "NGX")  // 5500 NGX-S
        return getNGXEntityId(neId, aid);

    string aidType = p.getTokenbyIndex(0);

    bool needMore = false;
    if (aidType == "ODU0T" || aidType == "ODU1T" || aidType == "ODU2T" || aidType == "ODU3T" || aidType == "ODU4T")
    {
        string remainer = aid.substr(8);
        if(remainer.find("A") != string::npos)
        {
            needMore = true;
        }
    }

    if (needMore) //traditional ODUkT are not come to here
    {
        aidType = aid.substr(0, 7) ;
        index = 2;

        if (len > index)             // If there's a shelf,
        {
            shelfNo = p.getTokenbyIndex(index++);
        }
        if (len > index)             // If there's a slot,
        {
            slotNo = p.getTokenbyIndex(index++);
        }
        if (len > index)             // If there's a port or sub-slot,
        {
            portNo = p.getTokenbyIndex(index++);
        }

        if (len > index)             // If there's a channel or STS,
        {
            string dir = p.getTokenbyIndex(index++);
            if(dir.find("A") != string::npos)
            {
                chNo = "10"; // "10" indicate 'A'
                dir.erase(0,1);
                chNo += dir;
            }
        }
        if (len > index)  // If there's a level and other
        {
            string dir = p.getTokenbyIndex(index++);
            string level = dir;

            if(len > index)
            {
                dir = p.getTokenbyIndex(index++);
                level += dir;
            }

            if(!level.empty())
            {
                if (level.length() == 1)
                    levelNo.append("0");
                levelNo.append(level);
            }
        }
    }
    else if ((aidType == "ODU0" || aidType == "ODU1" || aidType == "ODU2" || aidType == "ODU3" || aidType == "ODU4" || aidType.find("GCC") != string::npos) 
        && (aid.find("A") != string::npos)) //traditionals are not come to here
    {       
        shelfNo = p.getTokenbyIndex(1);
        slotNo = p.getTokenbyIndex(2);
        portNo = p.getTokenbyIndex(3);
        string dir;
        if(len >= 5)
        {
            dir = p.getTokenbyIndex(4);
            if(dir.find("A") != string::npos)
            {
                chNo = "10"; // "10" indicate 'A'
                dir.erase(0,1);
                chNo += dir;

                if(len >= 6)
                {
                    string trib = p.getTokenbyIndex(5);
                    if(trib.find("Z") != string::npos) // "9" indicate 'Z'
                    {
                        levelNo = "9";
                        trib.erase(0,1);
                        levelNo += trib;
                    }
                    else
                    {
                        if (trib.length() == 1)
                            levelNo.append("0");
                        levelNo += trib;
                    }
                }
            }
            else
            {
                chNo = dir;
            }
        }
    }
    else
    {
        if (len > 1)
        {
            const string& Type = p.getTokenbyIndex(1);

            if ((Type.at(0) > '9') || (Type.at(0) < '0'))
            {
                // This must be a multipart AID type like OCH-P, so put it back together
                aidType = aidType + "-" + Type;
                index = 2;   // Advance the index to the shelf token
            }

            if (len > index)             // If there's a shelf,
            {
                shelfNo = p.getTokenbyIndex(index++);
            }
            if (len > index)             // If there's a slot,
            {
                slotNo = p.getTokenbyIndex(index++);
            }
            if (len > index)             // If there's a port or sub-slot,
            {
                portNo = p.getTokenbyIndex(index++);
            }
            if (len > index)             // If there's a channel or STS,
            {
                chNo = p.getTokenbyIndex(index++);
            }
            if (len > index)             // If there's a level
            {
                levelNo = p.getTokenbyIndex(index++);
            }
        }
    }

    // Get the numerical Id form of the type from the AID
    return getEntityIdandType(neId, aidType, shelfNo, slotNo, portNo, chNo, levelNo, entityType);
}

//******************************************************************************
// we can do it by: getEntityIdandType(const string& neId, const string& aid, string& entityType)
string RdbUtil::getEntityId(const string& neId, const string& aid)
{
    string entityType;
    return getEntityIdandType(neId, aid, entityType);
}

//******************************************************************************
string RdbUtil::getEntityIdandType(const string& neId, const string& aidType,
                                   const string& shelfNo, const string& slotNo,
                                   const string& portNo, const string& chNo,
                                   string& entityType )
{
    string entityId;

    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType",
                             TRACE7, "Numerical for AID TYPE %s %s %s",
                             aidType.c_str(), shelfNo.c_str(), slotNo.c_str());

    // Get the numerical Id form of the type from the AID
    if (RdbRefEntityTable::instance()->getEntityIdandType(aidType, entityId, entityType))
    {
        entityId.insert(0, neId);      // Prepend the NE Id

        // Assemble the numerical Id for this object.
        if (shelfNo.length() == 1)
            entityId.append("0");
        entityId.append(shelfNo);     // Shelf is often the NP as well

        if (!slotNo.empty())
        {
            if (aidType == "NODE")    // Control Plane Node is a special case.
            {
                if (slotNo.length() == 1)
                    entityId.append("00");
                else if (slotNo.length() == 2)
                    entityId.append("0");
                entityId.append(slotNo);
            }
            else if (slotNo.length() > 2)
            {
                if (slotNo == "100")  // A special case for Control Plane
                    entityId.append("00");
                else
                {
                    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType", MAJOR,
                                             "The AIDs second numerical is too large %s to be slot; continuing",
                                             slotNo.c_str());
                    entityId.append(slotNo);
                }
            }
            else
            {
                if (slotNo.length() == 1)
                    entityId.append("0");
                entityId.append(slotNo);
            }

            if (!portNo.empty())
            {
                if (portNo.length() > 2)
                {
                    if (portNo == "100")  // A special case for Control Plane (RSVPADJ and OSPFADJ)
                        entityId.append("00");
                    else
                    {
                        ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType", MAJOR,
                                                 "The AIDs third numerical is too large %s to be port; continuing",
                                                 portNo.c_str());
                        entityId.append(portNo);
                    }
                }
                else
                {
                    if (portNo.length() == 1)
                        entityId.append("0");
                    entityId.append(portNo);
                }

                if (!chNo.empty())
                {
                    if (chNo.length() == 1)
                        entityId.append("00");
                    else if (chNo.length() == 2)
                        entityId.append("0");
                    entityId.append(chNo);
                }
            }
        }
    }
    else  // If not a recognized AID type, return a default
    {
        entityId = neId;
        entityType = "NE";
    }

    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType",
                             TRACE7, "Returning Entity Id %s [%s]",
                             entityId.c_str(), entityType.c_str());
    return entityId;
}

//******************************************************************************
string RdbUtil::getEntityId(const string& neId, const string& aidType,
                            const string& shelfNo, const string& slotNo,
                            const string& portNo, const string& chNo, const string& levelNo)
{
    string entityId;

    ErrorLogger::logVarError(sourceName, objectName, "getEntityId",
                             TRACE7, "Numerical for AID TYPE %s %s %s",
                             aidType.c_str(), shelfNo.c_str(), slotNo.c_str());

    // Get the numerical Id form of the type from the AID
    if (RdbRefEntityTable::instance()->getEntityIdCode(aidType, entityId))
    {
        entityId.insert(0, neId);      // Prepend the NE Id

        // Assemble the numerical Id for this object.
        if (shelfNo.length() == 1)
            entityId.append("0");
        entityId.append(shelfNo);

        if (!slotNo.empty())
        {
            if (aidType == "NODE")    // Control Plane Node is a special case.
            {
                if (slotNo.length() == 1)
                    entityId.append("00");
                else if (slotNo.length() == 2)
                    entityId.append("0");
                entityId.append(slotNo);
            }
            else if (slotNo.length() > 2)
            {
                if (slotNo == "100")  // A special case for Control Plane
                    entityId.append("00");
                else
                {
                    ErrorLogger::logVarError(sourceName, objectName, "getEntityId", MAJOR,
                                             "The AIDs second numerical is too large %s to be slot; continuing",
                                             slotNo.c_str());
                    entityId.append(slotNo);
                }
            }
            else
            {
                if (slotNo.length() == 1)
                    entityId.append("0");
                entityId.append(slotNo);
            }

            if (!portNo.empty())
            {
                if (portNo.length() > 2)
                {
                    if (portNo == "100")  // A special case for Control Plane (RSVPADJ and OSPFADJ)
                        entityId.append("00");
                    else
                    {
                        ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType", MAJOR,
                                                 "The AIDs third numerical is too large %s to be port; continuing",
                                                 portNo.c_str());
                        entityId.append(portNo);
                    }
                }
                else
                {
                    if (portNo.length() == 1)
                        entityId.append("0");
                    entityId.append(portNo);
                }

                if (!chNo.empty())
                {
                    if (chNo.length() == 1)
                        entityId.append("00");
                    else if (chNo.length() == 2)
                        entityId.append("0");
                    entityId.append(chNo);

                    if(!levelNo.empty())
                    {
                        if (levelNo.length() == 1)
                            entityId.append("0");
                        entityId.append(levelNo);
                    }
                }
            }
        }
    }
    else  // If not a recognized AID type, return a default
    {
        entityId = neId;
    }

    ErrorLogger::logVarError(sourceName, objectName, "getEntityId",
                             TRACE7, "Returning Entity Id %s ", entityId.c_str());
    return entityId;
}

//******************************************************************************
string RdbUtil::getEntityTypeFromAid(const string& aid)
{
    ErrorLogger::logVarError(sourceName, objectName, "getEntityTypeFromAid",
                             TRACE7, "convert from AID = %s", aid.c_str());

    if (aid.empty()) {
        return "";
    }
    StringParser p(aid, "-");
    string aidType = p.getTokenbyIndex(0);
    if (p.getLength() > 1)
    {
        const string& Type = p.getTokenbyIndex(1);

        if ((Type.at(0) > '9') || (Type.at(0) < '0'))
        {
            // This must be a multipart AID type like OCH-P, so put it back together
            aidType = aidType + "-" + Type;
        }
    }

    string entityType;
    if (RdbRefEntityTable::instance()->getEntityIdandType(aidType, string(), entityType))
        return entityType;
    else
        return "NE";
}

//******************************************************************************
string RdbUtil::getAidTypeFromId(const string& entityId)
{
    ErrorLogger::logVarError(sourceName, objectName, "getAidTypeFromId",
                             TRACE7, "convert from Id = %s", entityId.c_str());

    if (entityId.length() >= 7)
    {
        // I'm assuming a neId is always 4 digits here
        const string& numType = entityId.substr(4, 3);
        string aidType;
        if (RdbRefEntityTable::instance()->getAidTypeandType(numType, aidType, string()))
            return aidType;
        else
            return "";
    }
    else
        return "";
}
//******************************************************************************
string RdbUtil::getAidTypeFromId(const string& entityId, const string& neid)
{
    ErrorLogger::logVarError(sourceName, objectName, "getAidTypeFromId",
                             TRACE7, "convert from Id = %s, neId = %s.", entityId.c_str(), neid.c_str());

    if (entityId.length() >= 7)
    {
        // the length of neid for Non 7100 ne is 6 but 4.
        const string& numType = entityId.substr(neid.length(), 3);
        string aidType;
        if (RdbRefEntityTable::instance()->getAidTypeandType(numType, aidType, string()))
            return aidType;
        else
            return "";
    }
    else if (0==neid.compare(entityId))
    {
        return "NE";
    }
    else
        return "";
}

//******************************************************************************
string RdbUtil::getEntityTypeFromId(const string& entityId)
{
    ErrorLogger::logVarError(sourceName, objectName, "getEntityTypeFromId",
                             TRACE7, "convert from Id = %s", entityId.c_str());

    if (entityId.length() >= 7)
    {
        // I'm assuming a neId is always 4 digits here
        string numType = entityId.substr(4, 3);
        string entityType;
        if (RdbRefEntityTable::instance()->getAidTypeandType(numType, string(), entityType))
            return entityType;
        else
            return "";
    }
    else
        return "";
}
//******************************************************************************
string RdbUtil::getEntityTypeFromId(const string& entityId, const string& neid)
{
    ErrorLogger::logVarError(sourceName, objectName, "getEntityTypeFromId",
                             TRACE7, "convert from Id = %s", entityId.c_str());

    if (entityId.length() >= 7)
    {
        string numType = entityId.substr(neid.length(), 3);
        string entityType;
        if (RdbRefEntityTable::instance()->getAidTypeandType(numType, string(), entityType))
            return entityType;
        else
            return "";
    }
    else
        return "";
}


//******************************************************************************
// parseEntityId is passed a numeric entityId and a neId.  If the neId is at the
// beginning of entityId and there are more digits, it breaks them down into the
// individual parts and returns true.  Otherwise, it returns false.
//
bool RdbUtil::parseEntityId(const string& entityId, const string& neId, string& aidType,
                            string& shelfNo, string& slotNo, string& portNo, string& chNo, string& levelNo)
{
    string entityType;

    ErrorLogger::logVarError(sourceName, objectName, "parseEntityId", __LINE__,
                             TRACE7, "Entity Id %s", entityId.c_str());

    // if neId found at start of entityId,
    if (entityId.compare(0, neId.size(), neId) == 0)
    {
        const string& restOfId = entityId.substr( neId.size(), entityId.size() - neId.size() );

        if (restOfId.size() > 2)
        {
            string numType = restOfId.substr(0, 3);

            if (!RdbRefEntityTable::instance()->getAidTypeandType(numType, aidType, entityType))
                aidType = numType;

            if (restOfId.size() > 4)
            {
                shelfNo = restOfId.substr(3, 2);
                if (shelfNo.at(0) == '0')
                    shelfNo.erase(0, 1);

                if (restOfId.size() > 6)
                {
                    if (aidType == "NODE" && restOfId.size() > 7)
                    {
                        slotNo = restOfId.substr(5, 3);
                        if (slotNo.at(0) == '0')
                            slotNo.erase(0, 1);
                        if (slotNo.at(0) == '0')
                            slotNo.erase(0, 1);
                    }
                    else
                    {
                        slotNo = restOfId.substr(5, 2);
                        if (slotNo == "00")    // Special for control plane
                            slotNo = "100";
                        else if (slotNo.at(0) == '0')
                            slotNo.erase(0, 1);
                    }

                    if (restOfId.size() > 8)
                    {
                        if(0 == entityType.compare("EVC"))
                        {
                            portNo = restOfId.substr(7, 3);
                        }
                        else
                        {
                            portNo = restOfId.substr(7, 2);
                        }
                        if (portNo == "00")    // Special for control plane
                            portNo = "100";
                        else if (portNo.at(0) == '0')
                            portNo.erase(0, 1);

                        if (restOfId.size() > 11)
                        {
                            chNo = restOfId.substr(9, 3);

                            // if padded with leading zero(es)
                            if (chNo.at(0) == '0')
                                chNo.erase(0, 1);
                            if (chNo.at(0) == '0')
                                chNo.erase(0, 1);
                        }
                    }
                }
            }
            return true;
        }
        else    // It was nothing but the neId, so
            return false;
    }
    else
    {
        ErrorLogger::logVarError(sourceName, objectName, "parseEntityId", __LINE__,
                                 MINOR, "NE Id %s not found at start of Entity Id %s",
                                 neId.c_str(), entityId.c_str());
        return false;
    }
}

//******************************************************************************
// parseEntityIdNumbers is passed a numeric entityId.
// If there are enough digits, it breaks them down into the individual parts and
// returns true.  Otherwise, it returns false.
//
bool RdbUtil::parseEntityIdNumbers(const string& entityId, string& shelfNo, string& slotNo, string& portNo, string& chNo, string &levelNo)
{
    ErrorLogger::logVarError(sourceName, objectName, "parseEntityIdNumbers",
                             TRACE7, "Entity Id %s", entityId.c_str());

    if (entityId.size() > 8)
    {
        shelfNo = entityId.substr(7, 2);
        if (shelfNo.at(0) == '0')
            shelfNo.erase(0, 1);

        if (entityId.size() > 10)
        {
            slotNo = entityId.substr(9, 2);
            if (slotNo == "00")    // Special for control plane
                slotNo = "100";
            if (slotNo.at(0) == '0')
                slotNo.erase(0, 1);

            if (entityId.size() > 12)
            {
                //EVC id may has 3 digits port number, like this EVC-2-1-178(10013380201178)
                if (14 == entityId.size())
                {
                    portNo = entityId.substr(11, 3);
                }
                else
                {
                    portNo = entityId.substr(11, 2);
                }
                if (portNo == "00")
                    portNo = "100";
                else if (portNo.at(0) == '0')
                    portNo.erase(0, 1);

                if (entityId.size() > 15)
                {
                    chNo = entityId.substr(13, 3);

                    // if padded with leading zero(es)
                    if (chNo.at(0) == '0')
                        chNo.erase(0, 1);
                    if (chNo.at(0) == '0')
                        chNo.erase(0, 1);
                }
            }
        }
        return true;
    }
    else
    {
        ErrorLogger::logVarError(sourceName, objectName, "parseEntityIdNumbers", __LINE__, TRACE3,
                                 "Not enough digits in Entity Id %s to fill number parts", entityId.c_str());
        return false;
    }
}

//******************************************************************************
// getAidAndType is passed a numeric entity id and returns the AID equivalent,
// or an empty string.  It also returns the internal (base) table type.
//
string RdbUtil::getAidAndType(const string& id, string& entityType, const string& neid)
{
    string aid, neId;

    ErrorLogger::logVarError(sourceName, objectName, "getAidAndType", __LINE__,
                             TRACE7, "Entity Id %s", id.c_str());

    if ( id.empty() || ((id.at(0) < '0') || (id.at(0) > '9')) )
    {
        ErrorLogger::logVarError(sourceName, objectName, "getAidAndType", __LINE__, TRACE1,
                                 "Entity Id (%s) empty or non-numeric", id.c_str());
        return id;
    }

    int nPos = 3;
    if(!neid.empty())
        nPos = neid.length()-1;

    // if neId found at start of entityId,
    if (id.size() > nPos)
    {
        // first four digits are NE Id.
        neId = id.substr( 0, nPos+1 );//index: 0-3 

        if (id.size() > nPos+3)//6
        {
            // digits 5, 6, and 7 are type
            string digits = id.substr(nPos+1, 3);//index: 4-6

            if (RdbRefEntityTable::instance()->getAidTypeandType(digits, aid, entityType))
            {
                if (id.size() > nPos+5)//8
                {
                    // 8th & 9th digits are shelf (or network partition)
                    digits = id.substr(nPos+4, 2);//index: 7-8

                    if (digits.at(0) == '0')
                        digits.erase(0, 1);

                    aid += "-" + digits;


                    if (id.size() > nPos+7)//10
                    {
                        if (aid.compare(0,4,"NODE") == 0 && id.size() > nPos+8)  // Control Plane Node special case
                        {
                            // 10th to 12th digits are node number
                            digits = id.substr(nPos+6, 3);

                            if (digits.at(0) == '0')
                                digits.erase(0, 1);
                            if (digits.at(0) == '0')
                                digits.erase(0, 1);

                            aid += "-" + digits;
                        }
                        else
                        {
                            // 10th & 11th digits are slot
                            digits = id.substr(nPos+6, 2); //index: 9-10

                            if (digits == "00")
                                digits = "100";  // Special case for control plane
                            else if (digits.at(0) == '0')
                                digits.erase(0, 1);

                            aid += "-" + digits;


                            if (id.size() > nPos+9)//12
                            {
                                // 12th & 13th are port (or subslot)
                                digits = id.substr(nPos+8, 2);//index: 11-12

                                if (digits == "00")
                                    digits = "100";  // Special case for control plane
                                else if (digits.at(0) == '0')
                                    digits.erase(0, 1);

                                aid += "-" + digits;

                                if ((aid.find("ODU") != string::npos || aid.find("GCC") != string::npos) && id.size() > 17) //traditional ODU id.size()=16, not come to here
                                {
                                    digits = id.substr(nPos+10, 3); //index: 13-15, channel
                                    
                                    string oduMux = digits.substr(0,2);
                                    if (oduMux == "10")
                                    {
                                        oduMux = "A";
                                        aid.append("-").append(oduMux).append(digits.substr(2));
                                    }
                                    else
                                    {
                                        if (digits.at(0) == '0')
                                            digits.erase(0, 1);
                                        if (digits.at(0) == '0')
                                            digits.erase(0, 1);
                                        aid.append("-").append(digits);
                                    }

                                    digits = id.substr(nPos+13, 2); //index: 16-17                                        
                                    if (digits.at(0) == '0')
                                    {
                                        digits.erase(0, 1);
                                        aid.append("-").append(digits);
                                    }
                                    else if(digits.at(0) == '9') 
                                    {
                                        aid.append("-").append("Z");
                                        digits.erase(0, 1);
                                        aid += digits;
                                    }
                                    else
                                    {
                                        aid.append("-").append(digits.substr(0, 1));
                                        digits.erase(0, 1);
                                        aid.append("-").append(digits);
                                    }
                                }
                                else if (id.size() > nPos+12)  //15
                                {
                                    // 14th, 15th, & 16th digits are Channel or TimeSlot
                                    digits = id.substr(nPos+10, 3);//index: 13-15

                                    // if padded with leading zero(es)
                                    if (digits.at(0) == '0')
                                        digits.erase(0, 1);
                                    if (digits.at(0) == '0')
                                        digits.erase(0, 1);

                                    aid += "-" + digits;

                                    if (id.size() > nPos + 14) //17,levelNo
                                    {
                                        digits = id.substr(nPos+13, 2); //index: 16-17
                                        if (digits.at(0) == '0')
                                            digits.erase(0, 1);
                                        aid += "-" + digits;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            else
            {
                ErrorLogger::logVarError(sourceName, objectName, "getAidAndType", __LINE__, TRACE3,
                                         "Unable to reverse type for Entity Id %s", id.c_str());
                return id;
            }
        }
        else
        {
            aid = Server_i::instance()->getDefaultConfigValue(ConfigNames::TID, neId, "");
            entityType = "NE";
        }
    }

    ErrorLogger::logVarError(sourceName, objectName, "getAidAndType", __LINE__,
                             TRACE7, "Entity Id %s translates to %s", id.c_str(), aid.c_str());

    return aid;
}

//******************************************************************************
// getAidFromId is passed a numeric entity id and returns the AID equivalent,
// or an empty string.
//we can do it by: getAidAndType(const string& id, string& entityType, const string& neid)
string RdbUtil::getAidFromId(const string& id)
{
    string entityType;
    return getAidAndType(id, entityType);
}

//******************************************************************************
// getInternalType is passed a numeric entity id and returns the
// internal (base) table type.
//
string RdbUtil::getInternalType(const string& id )
{
    string aidType, entityType;

    ErrorLogger::logVarError(sourceName, objectName, "getInternalType", __LINE__,
                             TRACE7, "Entity Id %s", id.c_str());

    if (id.size() > 7)
    {
        string digits = id.substr(4, 3);  // digits 4, 5, and 6 are type
        RdbRefEntityTable::instance()->getAidTypeandType(digits, aidType, entityType);
    }
    else if (id.size() > 3)
    {
        entityType = "NE";
    }

    ErrorLogger::logVarError(sourceName, objectName, "getInternalType", __LINE__,
                             TRACE7, "Entity Id %s is found in %s", id.c_str(), entityType.c_str());

    return entityType;
}

//******************************************************************************
// getAidComponents is passed a entity Aid and returns the
// all different components that build AID.
//
string RdbUtil::getAidTypeAndComponents(const string& aid, string& shelfNo, string& slotNo,
                                        string& portNo, string& chNo, string& levelNo)
{
    string Type;
    int index = 1;

    ErrorLogger::logVarError(sourceName, objectName, "getAidComponents", __LINE__,
                             TRACE7, "convert from AID = %s", aid.c_str());

    StringParser p(aid, "-");
    int len = p.getLength();

    string aidType = p.getTokenbyIndex(0);
    if (len > 1)
    {
        Type = p.getTokenbyIndex(1);

        if ((Type.at(0) > '9') || (Type.at(0) < '0'))
        {
            // This must be a multipart AID type like OCH-P, so put it back together
            aidType = aidType + "-" + Type;
            index = 2;   // Advance the index to the shelf token
        }

        if (len > index)             // If there's a shelf,
        {
            shelfNo = p.getTokenbyIndex(index++);
        }
        if (len > index)             // If there's a slot,
        {
            slotNo = p.getTokenbyIndex(index++);
        }
        if (len > index)             // If there's a port or sub-slot,
        {
            portNo = p.getTokenbyIndex(index++);
        }
        if (len > index)             // If there's a channel or STS,
        {
            chNo = p.getTokenbyIndex(index++);
        }
    }
    return aidType;
}


//******************************************************************************
// Returns true if a row exists in the given table with the given id.
//
bool RdbUtil::checkIfEntityExists(const string& table, const string& idColName, const string& id)
{
    auto_ptr<RDbMgr> rdbMgr (RDbFactory::getDbMgr());
    string sql("SELECT COUNT(*) FROM ");
    sql.append(table);
    sql.append(" WHERE ");

    int pos = idColName.find_first_of(",");
    if ( pos != string::npos)   // If there is a comma,
    {
        const string& col1 = idColName.substr(0, pos);
        const string& col2 = idColName.substr(pos + 1);
        pos = id.find_first_of(",");
        const string& id1 = id.substr(0, pos);
        string id2 = id.substr(pos + 1);
        
        pos = col2.find_first_of(",");   // Need to look for a second comma (only for TNA at the moment)
        if ( pos != string::npos)   // If there is a second comma,
        {
            const string& col2a = col2.substr(0, pos);
            const string& col3 = col2.substr(pos + 1);
            pos = id2.find_first_of(",");
            if (pos == string::npos)
            {
                ErrorLogger::logVarError(sourceName, objectName, "checkIfEntityExists", __LINE__, MAJOR,
                                         "column (%s) value (%s) mismatch on table %s", 
                                         idColName.c_str(), id.c_str(), table.c_str());
            }
            const string& id2a = id2.substr(0, pos);
            const string& id3 = id2.substr(pos + 1);
        
            sql += col1 + " = :x AND " + col2a + " = :y AND " + col3 + " = :z";
            rdbMgr->setSQL(sql);

            rdbMgr->bindStringParam(1, id1);
            rdbMgr->bindStringParam(2, id2a);  
            rdbMgr->bindStringParam(3, id3);  
        }
        else // just the one comma
        {
            if (id2.empty())
                id2 = '0'; //This is a hack to make SQL work if Id2 is empty and second column value is null in database
            sql += col1 + " = :x AND Nvl(" + col2 + ",0) = :y";
            rdbMgr->setSQL(sql);

            rdbMgr->bindStringParam(1, id1);
            rdbMgr->bindStringParam(2, id2);
        }
        auto_ptr<ResultSet> rs ( rdbMgr->executeQuery() );
        if (rs->moveNext())
        {
            if (rs->getInt(1) > 0)
                return true;
        }
    }
    else    // No comma in idColName (single column unique key)
    {
        sql.append(idColName);
        sql.append(" = :x");
        rdbMgr->setSQL(sql);
        rdbMgr->bindStringParam(1, id);
        auto_ptr<ResultSet> rs ( rdbMgr->executeQuery() );
        if (rs->moveNext())
        {
            if (rs->getInt(1) > 0)
                return true;
        }
    }
    return false;
}

string RdbUtil::getNeType(const string& neId)
{
    string neType("");
    auto_ptr<RDbMgr> rdbMgr (RDbFactory::getDbMgr());
    string sql("SELECT NE_TYPE FROM EMS_NE WHERE NE_ID=:1 ");
    rdbMgr->setSQL(sql);
    rdbMgr->bindStringParam(1, neId);
    auto_ptr<ResultSet> rs ( rdbMgr->executeQuery() );
    if (rs->moveNext())
    {
        neType = rs->getString(1);
    }
    return neType;
}

string RdbUtil::getShelfType(const string& neId, const string& shelfNo)
{
    string shelfType("");
    auto_ptr<RDbMgr> rdbMgr (RDbFactory::getDbMgr());
    string sql("SELECT NVL(SHELF_SUBTYPE ,'') FROM CM_SHELF WHERE NE_ID=:1 AND SHELF_AID_SHELF =:2 ");
    rdbMgr->setSQL(sql);
    rdbMgr->bindStringParam(1, neId);
    rdbMgr->bindStringParam(2, shelfNo);
    auto_ptr<ResultSet> rs ( rdbMgr->executeQuery() );
    if (rs->moveNext())
    {
        shelfType = rs->getString(1);
    }
    return shelfType;
}

bool RdbUtil::isCPE(const string& neId)
{
    //not include 7305 and O9400R
    const string& netype = Server_i::instance()->getDefaultConfigValue(ConfigNames::NeTypeKey, neId, "OTS");
    if (0==netype.compare(EMSDEF::AN_CPE7345_NETYPE) || 0==netype.compare(EMSDEF::AN_CPE7325_NETYPE) || 0==netype.compare(EMSDEF::AN_CPE7310_NETYPE))
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
bool RdbUtil::isCPENE(const string& neType)
{
    //not include 7305 and O9400R
    if (0==neType.compare(EMSDEF::AN_CPE7345_NETYPE) || 0==neType.compare(EMSDEF::AN_CPE7325_NETYPE) || 0==neType.compare(EMSDEF::AN_CPE7310_NETYPE))
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool RdbUtil::is7305NE(const string& neId)
{
    const string& neType = Server_i::instance()->getDefaultConfigValue(ConfigNames::NeTypeKey, neId, "OTS");
    if ( neType.find("7305") != string::npos)
    {
        return true;
    }

    return false;
}

bool RdbUtil::is7100E(const string& neId)
{
    const string& neType = Server_i::instance()->getDefaultConfigValue(ConfigNames::NeTypeKey, neId, "OTS");
    if(neType.find("O9400R") != string::npos)
    {
        return true;
    }

    return false;
}

void RdbUtil::getRcmmAidbySide(const string& neId, const string& side, string& aid)
{
    auto_ptr<RDbMgr> pDbMgr(RDbFactory::getDbMgr());
    try
    {
        string sql = "SELECT AID FROM CARD_CM_VW WHERE NE_ID = :1 AND DWDMSIDE =:2 AND AID_TYPE =:3";
        pDbMgr->setSQL(sql);
        pDbMgr->bindStringParam(1, neId);
        pDbMgr->bindStringParam(2, side);
        pDbMgr->bindStringParam(3, "RCMM");

        auto_ptr<ResultSet> rs(pDbMgr->executeQuery());
        if (rs->moveNext())
        {
            aid = rs->getString(1);
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, "getRcmmAidbySide",
                                 __LINE__, MAJOR, "Get db operation exception.");
    }
}

string RdbUtil::getNePreferenceNav(const string& neType)
{
    string type("7100");
    if (neType.find("7305") != string::npos || 0 == neType.compare(EMSDEF::AN_CPE7345_NETYPE) 
        || 0 == neType.compare(EMSDEF::AN_CPE7325_NETYPE) || 0 == neType.compare(EMSDEF::AN_CPE7310_NETYPE))
    {
        type = "7300";
    }
    else if(0 == neType.compare("O9400R"))
    {
        type = "7100E";
    }

    string preNav("1");
    auto_ptr<RDbMgr> rdbMgr (RDbFactory::getDbMgr());
    string sql("SELECT DEFAULT_ATTRIBUTE_VALUE FROM EMS_DEFAULT_ATTRIBUTE WHERE DEFAULT_SUBTYPE='PreferenceNav' AND DEFAULT_ATTRIBUTE_NAME=:1 ");
    rdbMgr->setSQL(sql);
    rdbMgr->bindStringParam(1, type);
    auto_ptr<ResultSet> rs ( rdbMgr->executeQuery() );
    if (rs->moveNext())
    {
        preNav = rs->getString(1);
    }
    return preNav;
}

//******************************************************************************
string RdbUtil::getEntityIdandType(const string& neId, const string& aidType,
                                   const string& shelfNo, const string& slotNo,
                                   const string& portNo, const string& chNo,                                   
                                   const string& levelNo, string& entityType )
{
    string entityId;

    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType",
                             TRACE7, "Numerical for AID TYPE %s %s %s",
                             aidType.c_str(), shelfNo.c_str(), slotNo.c_str());

    // Get the numerical Id form of the type from the AID
    if (RdbRefEntityTable::instance()->getEntityIdandType(aidType, entityId, entityType))
    {
        entityId.insert(0, neId);      // Prepend the NE Id

        // Assemble the numerical Id for this object.
        if (shelfNo.length() == 1)
            entityId.append("0");
        entityId.append(shelfNo);     // Shelf is often the NP as well

        if (!slotNo.empty())
        {
            if (aidType == "NODE")    // Control Plane Node is a special case.
            {
                if (slotNo.length() == 1)
                    entityId.append("00");
                else if (slotNo.length() == 2)
                    entityId.append("0");
                entityId.append(slotNo);
            }
            else if (slotNo.length() > 2)
            {
                if (slotNo == "100")  // A special case for Control Plane
                    entityId.append("00");
                else
                {
                    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType", MAJOR,
                                             "The AIDs second numerical is too large %s to be slot; continuing",
                                             slotNo.c_str());
                    entityId.append(slotNo);
                }
            }
            else
            {
                if (slotNo.length() == 1)
                    entityId.append("0");
                entityId.append(slotNo);
            }

            if (!portNo.empty())
            {
                if (portNo.length() > 2)
                {
                    if (portNo == "100")  // A special case for Control Plane (RSVPADJ and OSPFADJ)
                        entityId.append("00");
                    else
                    {
                        ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType", MAJOR,
                                                 "The AIDs third numerical is too large %s to be port; continuing",
                                                 portNo.c_str());
                        entityId.append(portNo);
                    }
                }
                else
                {
                    if (portNo.length() == 1)
                        entityId.append("0");
                    entityId.append(portNo);
                }

                if (!chNo.empty())
                {
                    if (chNo.length() == 1)
                        entityId.append("00");
                    else if (chNo.length() == 2)
                        entityId.append("0");
                    entityId.append(chNo);

                    if(!levelNo.empty())
                    {
                        if (levelNo.length() == 1)
                            entityId.append("0");
                        entityId.append(levelNo);
                    }
                }
            }
        }
    }
    else  // If not a recognized AID type, return a default
    {
        entityId = neId;
        entityType = "NE";
    }

    ErrorLogger::logVarError(sourceName, objectName, "getEntityIdandType",
                             TRACE7, "Returning Entity Id %s [%s]",
                             entityId.c_str(), entityType.c_str());
    return entityId;
}
//*************************************************************************************************************************
//                       Retrieve Deafult Management Provision Attributes by attribute name and sub type.
//*************************************************************************************************************************
void RdbUtil::retrieveDefaultProvisionAttributes(const string& attrName, const string& attrSubType, string& attrValue)
{
    auto_ptr<RDbMgr> pDbMgr(RDbFactory::getDbMgr());
    try
    {
        string sql = "SELECT DEFAULT_ATTRIBUTE_VALUE FROM EMS_DEFAULT_ATTRIBUTE WHERE DEFAULT_TYPE =:1 AND DEFAULT_ATTRIBUTE_NAME = :2 AND DEFAULT_SUBTYPE =:3 ";
        pDbMgr->setSQL(sql);
        pDbMgr->bindStringParam(1, "Provisioning");
        pDbMgr->bindStringParam(2, attrName);
        pDbMgr->bindStringParam(3, attrSubType);

        auto_ptr<ResultSet> rs(pDbMgr->executeQuery());
        if (rs->moveNext())
        {
            attrValue = rs->getString(1);
        }
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, objectName, "retrieveDefaultProvisionAttributes",
                                 __LINE__, MAJOR, "Get db operation exception.");
    }
}

