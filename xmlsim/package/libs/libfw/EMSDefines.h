/***********************************************************************************************
* COPYRIGHT 1999 - 2005 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: CmDefines
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EMSDefines.h  $
* Version: $Revision:   1.2  $
* Description:
* Common Defines Used In CM
*
* Revision History:
*
**********************************************************************************************/

#ifndef _EMSDEFINES_H_
#define _EMSDEFINES_H_

#include <string>
#include <list>

#define EMS_OBJECT_NOT_FOUND 1
#define EMS_COMMENT_OBJECT_NOT_FOUND "Object not found"
#define EMS_OBJECT_MODIFIED 2
#define EMS_COMMENT_OBJECT_MODIFIED "Object modified"
#define EMS_OBJECT_LOCKED 3
#define EMS_COMMENT_OBJECT_LOCKED "Object already locked"
#define EMS_INTERNAL_ERROR 4
#define EMS_COMMENT_INTERNAL_ERROR "Internal Error"
#define EMS_OPERATION_FAIL 5
#define EMS_COMMENT_ACCESS_DENIED "Access denied"
#define EMS_ACCESS_DENIED 6
#define EMS_BAD_PARAMETERS 7
#define EMS_UNIMPLEMENTED_FEATURE 8
#define EMS_COMMENT_UNIMPLEMENTED_FEATURE "Unimplement feature"


// Sysmon Exception Messsages
//added by lzou
#define NO_VERSION_EXCEPTION  "No Version Specified"
#define INVALID_VERSION_EXECEPTION  "Invalid Version"
/***************************************************************/
// Sysmon Exception Messsages
#define INVALID_TYPE_EXECEPTION      "Invalid Managed Resource Type"
#define HOST_OPERATIONAL_EXECEPTION     "Host is enabled for running"
#define SG_OPERATIONAL_EXECEPTION     "Server Group is enabled for running"
#define NE_OPERATIONAL_EXECEPTION     "NE is enabled for running"
#define REACHED_MAX_SG_EXCEPTION     "Failed to add a Server Group. The Host cannot manage anymore Server Groups"
#define REACHED_MAX_NE_EXCEPTION     "Failed to add a NE. The Server Group cannot manage anymore NEs"
#define HOST_DISABLED_EXCEPTION      "The Host must be operational."
#define SERVER_GROUP_DISABLED_EXCEPTION    "The Server Group must be operational."
#define INVALID_SYSTEM_EXECEPTION     "The Multiple Host feature isn't supported by this system."
//TLIaa54672   Added by jhu
#define SERVER_GROUP_START_FAILURE_EXCEPTION  "The Server Group Start Failure"


#define MR_OPERATIONAL_EXECEPTION     "The Managed Resource must be operational."
#define MR_DISABLED_EXCEPTION      "The Managed Resource must be operational."
#define MR_LOCKED_EXCEPTION       "The Managed Resource must be unlocked."

#define HOST_ADMIN_UNLOCKED_EXCEPTION    "The Host must be unlocked."
#define MANAGEDRESOURCE_ADMIN_UNLOCKED_EXCEPTION "The Parent Managed Resource must be unlocked."
#define SERVERGROUP_ADMIN_UNLOCKED_EXCEPTION  "The Server Group must be unlocked."
#define MANAGEDRESOURCE_ADMIN_LOCKED_EXCEPTION  "The Child Managed Resource must be locked."
#define HOST_ADMIN_LOCKED_EXCEPTION     "The Host must be locked."
#define SERVERGROUP_ADMIN_LOCKED_EXCEPTION   "The Server Group must be locked."

#define NE_ADMIN_LOCKED_EXCEPTION     "The NE must be locked."
#define DUPLICATE_NAME_EXCEPTION     "This name already exists - Please specify another name."

#define MANAGEDRESOURCE_CONTAINS_CHILDREN_EXCEPTION "The Managed Resource is managing resources. These resources must be removed first."
#define SERVERGROUP_CONTAINS_CHILDREN_EXCEPTION  "The Server Group is managing NEs. These NEs must be removed first."
#define HOST_CONTAINS_CHILDREN_EXCEPTION   "The Host is managing Server Groups. These Server Groups must be removed first."
#define MAX_NE_RUNNING_EXCEPTION     "The Maximum NEs per Server Group attribute cannot be set as the max is less than the resources currently supported."
#define MAX_NES_CURRENTLY_SUPPORTED     "Failed to Add NE, please create a new Server Group."
#define PRIMARY_HOST_CANNOT_REMOVE_EXCEPTION  "The Primary Host cannot be removed."
#define NETWORK_ADDRESS_EXISTS_EXCEPTION   "A new Host cannot be added as a Host with this network address already exists."
#define UNKNOWN_ERROR_EXCEPTION      "Unknown Error"
#define PRIMARYHOST_CANNOT_BELOCKED_EXCEPTION  "The Primary Host cannot be locked"
#define MAX_SG_RUNNING_EXCEPTION  "The Maximum Server Groups per Host attribute cannot be set as the max is less than the resources currently supported."
#define MANAGEDRESOURCE_CHILD_MAX_CANNOT_BE_ZERO "The maximum children value must be greater than zero."
#define SECONDARY_DAEMON_UNAVAILABLE   "Secondary Host daemon is not running"

// Sysmon Exception Error Codes
#define HOST_ERROR_CODE        20
#define SERVER_GROUP_ERROR_CODE      21
#define NE_ERROR_CODE        22
#define REMOVE_CHILD_ERROR_CODE      23
#define MANAGEDRESOURCE_CHILD_LOCKED_ERROR_CODE  24
#define MANAGEDRESOURCE_PARENT_LOCKED_ERROR_CODE 25
#define MANAGEDRESOURCE_CONTAINS_CHILDREN   26
#define SECONDARY_DAEMON_ERROR_CODE 27
#define HOST_DISABLED_ERROR_CODE 28
#define SERVERGROUP_DISABLED_ERROR_CODE 29
// TLIaa62259 - Added by aaali for version mismatch
#define VERSION_MISMATCH_EXCEPTION 254
// TLIaa62259 - Added by aaali for version mismatch end


///aaali_tliaa63384 start - SWUP changes . These error codes are required to prevent a potential revertable servergroup to be deleted
#define ATTACHED_NERESOURCE_ERROR_CODE 30
#define ATTACHED_NERESOURCE_ERROR_EXCEPTION  "This servergroup is set to be reverted to by one or more NEs"
///aaali_tliaa63384 end
#define SERVERGROUP_ADMIN_UNLOCKED_CODE 31
//#define SERVERGROUP_ADMIN_UNLOCKED_EXCEPTION   "The Server Group must be Unlocked."

//Dialog state defines
const int ISMADD = 0;
const int ISMMODIFY = 1;
const int ISMDISPLAY = 2;

// This define is used to represent Sysmon is
// stating processes
#define SYSMON_ADMIN    0

typedef std::list<std::string> LIST_OF_STRING_TYPE;

struct EMSProperty
{
    std::string name;
    std::string value;
};

struct NodeSumProperty
{
    std::string nodeName;
    std::string nodeId;
    std::string parentId;
    std::string nodeType;

    std::list<EMSProperty> properties;
    void addProperty (const std::string &name, const std::string &value)
    {
        _property.name = name;
        _property.value = value;
        properties.push_back (_property);
    }

private:
    EMSProperty _property;
};

typedef std::list<NodeSumProperty> LIST_OF_NODESUMPROP;

namespace EMS_FW
{

// Class containing EMS attributes names defined and used in a Framework. If
// specific product need to define more EMS attributes they have to be
// defined in a separate file.

typedef int PARTITIONIDTYPE;
typedef std::string SESSIONIDTYPE;
typedef const char * SESSIONID;

class EMSDEF
{
public:

    static const EMS_FW::SESSIONIDTYPE INVALID_SESSION_ID;

    static const std::string MoveNodeAction;

    static const std::string DESCRIPTION;
    // Name used for description attribute of MoLite Object.

    static const std::string PARTITIONID;
    // Name used for PartitionId attribute of MoLite Object. Partition Id is used
    // as a key based to gain access to the element.

    static const std::string POSITION;
    // Name of Position attribute used by Gui client to determine location of the Object
    // on the screen.

    static const std::string MOC;
    // Managed Object class assigned to the PMoLite Object.

    static const std::string MOI;

    static const std::string DLLNAME;
    // Name of the dll containing appropriate code for specific element.

    static const std::string BITMAPHINT;
    // Code used by the Gui Client to show appropriate icon for the element.

    static const std::string SLOTNUMBER;
    // Slot number where the card is positioned in the shelf.

    static const std::string POSITIONLEFT;
    // Left point of the Position.

    static const std::string POSITIONRIGHT;
    // Right point of the position;

    static const std::string POSITIONTOP;
    // Top point of the position.

    static const std::string POSITIONBOTTOM;
    // Bottom point of the position.

    static const std::string NODEINTERFACE;
    // Interface name registered in Naming Service from where Node could be reached.

    static const std::string DEFAULTVIEWTYPE;
    // Default value for view type

    static const std::string ALARMSEVERITY;

    static const std::string PROPALARMSEVERITY;

    static const std::string SOURCE;

    static const std::string LOCATION;

    static const std::string TL1AID;

    static const std::string NODESUMMARYLIST;

    static const std::string NESUBTYPE;
    //NE Sub Type, e.g. for OTS: OLT,OLA etc..

    static const std::string NE_SITE;

    static const std::string AUTODISCOVERY_STATE;

    static const std::string EMSTYPE;
    static const std::string EMSVERSION;

    static const std::string EMSSTRING;
    // data type used to represent generic string attribute

    static const long MAXEMSQUERY;
    // max number reserved for EMS Query ids. All Framework query ids are bellow this number

    static const long SOURCEANDLOCATIONQUERY;
    // Unique Id for Source and Location Ems query.

    static const long NE_PARTITION_ID_QUERY;  // added 03/01/01 ykouame
    // Unique Id for ne partition Ems query.

    static const long USERLABELQUERY;
    // Unique Id for module name Ems query.

    static const long NMQUERYOBJECT;
    // Unique Id for module name NM query.

    static const PARTITIONIDTYPE ADMIN_PARTITION_ID;

    static const PARTITIONIDTYPE ROOT_PARTITION_ID;

    static const PARTITIONIDTYPE NETWORK_PARTITION_ID;

    //addded by solay for client recovery
    static const std::string CMAVAILABLE;
    static const std::string FMAVAILABLE;
    static const std::string SGAVAILABLE;

    // These are the node::indication reasons
    static const std::string SUBJECTRELOCATED;
    static const std::string SUBJECTMODIFIED;
    static const std::string SUBJECTDELETED;
    static const std::string CHILDCREATED;
    static const std::string CHILDDELETED;
    static const std::string BUBBLEUPFAULT;
    static const std::string CMUP;
    static const std::string CMDOWN;
    static const std::string FMUP;
    static const std::string FMDOWN;
    static const std::string CMLOSSOFCOMM;
    static const std::string FMLOSSOFCOMM;
    static const std::string SMDOWN;
    static const std::string SMUP;
    static const std::string NMDOWN;
    static const std::string NMUP;
    static const std::string LPMDOWN;
    static const std::string LPMUP;
    static const std::string RMDOWN;
    static const std::string RMUP;
    static const std::string NBIDOWN;
    static const std::string NBIUP;
    static const std::string CRSMODIFIED;
    static const std::string CLASSENTITYMODIFIED;
    static const std::string CLASSENTITYADDED;
    static const std::string CLASSENTITYDELETED;

    static const std::string FMDELETE;
    static const std::string NELOCKED;
    static const std::string NEUNLOCKED;
    static const std::string CMAUTODISCOVER_INPROGRESS;
    static const std::string CMAUTODISCOVER_FAIL;
    static const std::string CMAUTODISCOVER_COMPLETED;
    static const std::string CMAUTODISCOVER_CANCELED;
    static const std::string CMAUTODISCOVER_NOTINIT;
    static const std::string ONDEMANDDISCOVERY_INPROGRESS;
    static const std::string ONDEMANDDISCOVERY_COMPLETED;
    static const std::string ONDEMANDDISCOVERY_FAIL;
    static const std::string ONDEMAND_SETMODE;
    static const std::string SYSMONUP;
    static const std::string SYSMONDOWN;
    static const std::string SYSMONACTIVE;

    // Added for network autodiscovery
    static const std::string LINKDISCOVERY_COMPLETED;
    static const std::string NTWKDISCOVERY_SYNCHRONIZED;
    static const std::string NTWKDISCOVERY_SYNCHRONIZING;
    static const std::string NTWKDISCOVERY_SCHEDULED;
    static const std::string NTWKDISCOVERY_UNSCHEDULED;
    static const std::string NTWKDISCOVERY_FAILED;
    static const std::string NTWKDISCOVERY_CANCELED;
    static const std::string NTWKDISCOVERY_ESTABLISHING_SESSION;
    static const std::string NTWKDISCOVERY_SYNCHING_TO_NE;

    static const std::string NTWKDISCOVERY_MANAGED;
    static const std::string NTWKDISCOVERY_CANDIDATE;
    static const std::string NTWKDISCOVERY_PLANNED;
    static const std::string NTWKDISCOVERY_PLANNED_CANDIDATED;

    // Added the following reasons to report to the client that the S/W NE
    // went down/is back in service
    static const std::string SWNEUP;
    static const std::string SWNEDOWN;

    // aaali added for revert, migration and servergroup switch notifications
    static const std::string NEVERSIONCHANGED;
    static const std::string NEMIGRATING;
    static const std::string NEMIGRATIONCOMPLETE;
    static const std::string NESWITCHINGSERVERGROUP;
    static const std::string NEREVERTING;
    static const std::string NEREVERTCOMPLETE;
    static const std::string NESWITCHSERVERGROUPCOMPLETE;
    // aaali added for revert, migration and servergroup switch notifications end
    /// aaali_tliaa62259 start
    static const std::string AGENTVERSIONMISMATCH;
    /// aaali_tliaa62259 end


    static const std::string SLASH; // back or forward slash /NT,UNIX/ that is used in EMS as string
    static const char CSLASH; // back or forward slash /NT,UNIX/ that is used in EMS as char

    static const std::string EMSPATHSEP; // semicolon or colon for /NT,UNIX/ that is used in EMS as string

    static const std::string True;
    static const std::string False;

    // ID's for operations on CM
    static const std::string CM_AUTODISCOVERY_REQUEST;
    static const std::string CM_CANCEL_AUTODISCOVERY;
    static const std::string CM_RESEND_BUBBLEUP;

    // ID's for operations on NM
    static const std::string NM_RESTARTNE_REQUEST;

    static const std::string NM_PINGNE_REQUEST;
    static const std::string NM_PINGNE_STATUS;
    static const std::string AT_GETPROPERTIES;
    static const std::string AT_GETCHILDRENPROPERTIES;

    // added by solay
    static const std::string RootManagedElementType;

    static const std::string ATTRIBUTEHOLDERTYPE;


    static const std::string ServerUser;

    //added by lzou for NodeSummary of Ne
    static const std::string SERVERGROUP_VERSION;
    static const std::string VERSION;
    /*static const std::string ADAPTER_VERSION_1_1;
    static const std::string ADAPTER_VERSION_1_2;
    */

    //lzou for 3.8
    //added for four options of actionName in sysmon_i
    static const std::string GetNeListKey;
    static const std::string GetSGListKey;
    static const std::string GetSGListofNeVersionKey;
    static const std::string GetNEListofSGVersionKey;
    static const std::string GetAvailableSGListofNeVersionKey;
    //for 73519
    static const std::string GetCompleteNeTypeListKey;
    static const std::string GetSGVersionListofNeTypeKey;

    //for 76006
    static const std::string GetSGListofNeVersionandSGVersionKey;



    // aaali added for getting sg version and ne versions for migration
    static const std::string GetNextSGVersionAndNEVersionsKey;

    //end of 3.8
    //lzou for 3.8.2
    static const std::string UpdateSGMConfig;
    static const std::string SGMDisabledForConfigChange;
    //end of 3.8.2


    // Shared Sysmon Attributes
    // Visible ISM Client Attribute Names
    static const std::string ROOT_TYPE;
    static const std::string HOST_TYPE;
    static const std::string SERVERGROUP_TYPE;
    static const std::string NE_TYPE;
    static const std::string NOTIFY_SERVER_TYPE;
    static const std::string EMS_SERVER_TYPE;
    static const std::string NOTIFY_CHANNEL_TYPE;
    static const std::string SW_SERVERGROUP_TYPE;
    static const std::string SW_NE_TYPE;
    static const std::string ADMIN_STATE_KEY;
    static const std::string OPERATION_STATE_KEY;
    static const std::string LOCKED;
    static const std::string UNLOCKED;
    static const std::string ENABLED;
    static const std::string DISABLED;
    static const std::string NumberOfNesInServerGroupKey;
    static const std::string AvailableNesInServerGroupKey;
    static const std::string NumberOfServerGroupsInHostKey;
    static const std::string AvailableServerGroupsInHostKey;
    static const std::string SERVERGROUP_NAME;
    static const std::string HOST_LOCATION;
    static const std::string MANAGEDRESOURCE_MAX;
    static const std::string MANAGEDRESOURCE_AVAILABLE;
    static const std::string ServerGroupToBeMigrated;
    //atambe added
    static const std::string MAX_NES_REACHED ;
    //atambe end
    static const std::string INVALID_LICENSE_KEY;
    // Attribute and Config Names
    static const std::string HostName;
    static const std::string NetworkAddress;
    static const std::string ServerGroupHostKey;
    static const std::string NumberOfServerNeActiveKey;
    static const std::string NeServerGroupKey;
    static const std::string AttributeMarkForDelete;
    static const std::string AttributeRestartFibonacciLower;
    static const std::string AttributeRestartFibonacciUpper;
    static const std::string AttributeRestartFibonacciTime;
    static const std::string PARENT_ID_KEY;
    static const std::string PARENT_NAME_KEY;
    static const std::string PRIMARY_HOST_NAME;
    static const std::string ISMROOT_NAME;
    static const std::string SW_SERVERGROUP_NAME;
    static const std::string SW_NE_NAME;

    static const std::string CLIENT_NOTIFY_SERVER_ID;
    static const std::string GLOBAL_NOTIFY_SERVER_ID;
    static const std::string CLIENT_NOTIFY_SERVER_NAMEPREFIX;
    static const std::string GLOBAL_NOTIFY_SERVER_NAMEPREFIX;

    static const std::string SECURITY_NOTIFY_CHANNEL_ID;
    static const std::string GLOBAL_NOTIFY_CHANNEL_ID;
    static const std::string FM_NOTIFY_CHANNEL_ID;
    static const std::string CM_NOTIFY_CHANNEL_ID;
    static const std::string BUBBLEUP_NOTIFY_CHANNEL_ID;
    static const std::string SECURITY_NOTIFY_CHANNEL_NAME;
    static const std::string GLOBAL_NOTIFY_CHANNEL_NAME;
    static const std::string FM_NOTIFY_CHANNEL_NAME;
    static const std::string CM_NOTIFY_CHANNEL_NAME;
    static const std::string BUBBLEUP_NOTIFY_CHANNEL_NAME;


    static const std::string SM_EMS_SERVER_ID;
    static const std::string NM_EMS_SERVER_ID;
    static const std::string RM_EMS_SERVER_ID;
    static const std::string PROXY_EMS_SERVER_ID;
    static const std::string SM_EMS_SERVER_NAMEPREFIX;
    static const std::string NM_EMS_SERVER_NAMEPREFIX;
    static const std::string RM_EMS_SERVER_NAMEPREFIX;
    static const std::string PROXY_EMS_SERVER_NAMEPREFIX;


    static const std::string NePreviousHostKey;
    // aaali added for revert
    static const std::string NePreviousServerGroupKey;
    static const std::string NePreviousVersionKey;
    // aaali added for revert
    // TLIaa62259 - aaali added for storing agent version info during version mismatch
    static const std::string VersionMismtachInfoKey;
    //static const std::string AgentVersionCheckKey;
    // TLIaa62259 - aaali added for storing agent version info during version mismatch end

    static const std::string SoftwareAlarmSGName;
    static const std::string SOFTWARE_ALARM_GROUPNAME;
    static const std::string PRIMARY_HOST_GROUPNAME;

    // ISM Managed Resource alamr identifier
    static const std::string ADMIN_STATE_ALARM_KEY;
    static const std::string SnapshotDbActionKey;
    static const std::string RevertNeActionKey;
    static const std::string MigrateNeActionKey;

    ///aaali_tliaa63384 start
    // support for deleting snapshot from Ne after migration
    static const std::string DeleteBackupActionKey;
    // support for configuration migration based on product
    static const std::string MigrationModeKey;
    ///aaali_tliaa63384 end
    //lzou for 70939
    static const std::string ServerSuffixKey;
    //end for 70939
    static const std::string SupportingEnhancedSecurityProperty;

    // Use for scope and filter rule
    static const std::string AgentFilterKey;
    static const std::string AgentScopeTypeKey;
    static const std::string AgentScopeLevelKey;
    static const std::string AgentFilterDefinedKey;
    static const std::string AgentScopeDefinedKey;

    // Added by lzou for TL1
    static const std::string TL1ProtocolKey;
    static const std::string CMIPTL1ProtocolKey;
    static const std::string ErrorCodeInfoKey;
    static const std::string ErrorDescInfoKey;
    static const std::string GetSGInfoKey;
    static const std::string GetBestSGInfoKey;
    static const std::string RtrvConfigsFromServerKey;
    static const std::string CmConNegotiationMsgKey;
    static const std::string FmConNegotiationMsgKey;
    static const std::string CmConNegotiationExpMsgKey;
    static const std::string FmConNegotiationExpMsgKey;
    // End of TL1

    static const std::string GATEWAYFUNCTION;
    static const std::string GATEWAYFUNCTIONDIR;
    static const std::string GATEWAYFUNCTIONGNEA;
    static const std::string GATEWAYFUNCTIONGNEB;
    static const std::string GATEWAYFUNCTIONRNE;
    static const std::string GATEWAYFUNCTIONSNMPRNE;
    static const std::string NETWORKTYPE;
    static const std::string NETWORKTYPEPRIVATE;
    static const std::string NETWORKTYPEPUBLIC;
    static const std::string NETWORKTYPEDIRECT;
    static const std::string EONSTATEPUBLIC;
    static const std::string EONSTATEPRIVATE;
    static const std::string EONTYPEPGNE1;
    static const std::string EONTYPEPGNE2;
    static const std::string EONTYPEPRNE;
    static const std::string EONTYPEDIROSC;
    static const std::string CONTROLSPANNUMBER;
    static const std::string DEFAULTGATEWAYNAME;
    static const std::string BACKUPGATEWAYNAME;
    static const std::string DEFAULTIPADDRESS;
    static const std::string DEFAULTPORT;
    static const std::string BACKUPIPADDRESS;
    static const std::string BACKUPPORT;
    static const std::string TL1SHELLIPADDRESS;
    static const std::string TL1SHELLPORT;
    static const std::string DEFAULTGATEWAYFUNCTION;
    static const std::string BACKUPGATEWAYFUNCTION;
    static const std::string ENABLEPEERRELATION;

    static const std::string DEFAULTPROXYINTERFACENAME;

    static const std::string ACTION_GETALLGNES;
    static const std::string ACTION_GETNESTATUSLIST;
    static const std::string ACTION_SETDISCOVERYSTATE;
    static const std::string ACTION_SETDISCOVERYPASSWORD;
    static const std::string ACTION_GETTIDLIST;
    static const std::string ACTION_ADDDISCOVERYPASSWORD;
    static const std::string ACTION_DELETEDISCOVERYPASSWORD;
    static const std::string ACTION_UPDATEDISCOVERYPASSWORD;
    static const std::string ACTION_GET_NE_METERING_CONFIG;
    static const std::string ACTION_SET_NE_METERING_CONFIG;
    static const std::string ACTION_REFRESH_NE_METERING_CONFIG;

    static const std::string AT_NETWORK_SHOWAREA;
    static const std::string AN_NETWORK_POPULATION;
    static const std::string AN_NETWORK_BOUNDS;
    static const std::string ACTION_GETDISCOVERYPASSWORD;

    static const std::string AT_NETWORK_GETEMSCOORD;
    static const std::string AT_NETWORK_SETEMSCOORD;
    static const std::string AN_NETWORK_EMSCOORDLIST;
    static const std::string AN_NETWORK_EMSCOORD_TID;
    static const std::string AN_NETWORK_EMSCOORD_CLLI;
    static const std::string AN_NETWORK_EMSCOORD_DEFINEDBYCLLI;
    static const std::string AN_NETWORK_EMSCOORD_COORDBLANK;

    // bulk coord import, please see MR 37443
    static const std::string AT_NETWORK_BULK_SETEMSCOORD;
    static const std::string AN_NETWORK_EMSCOORDLIST_NEW;
    static const std::string AN_NETWORK_EMSCOORDLIST_EXISTING;

    static const std::string AT_GETLINKS;
    static const std::string AN_LINK_IDS;
    static const std::string AN_LINK_NAMES;

    static const std::string AN_RCA_RESULT;
    static const std::string AT_DISCOVER_CIRCUIT_RCA;
    static const std::string AN_RCA_EXCEPTION;
    static const std::string AN_RCA_PARTIAL_ROUTES;
    
    static const std::string SNMPProtocolKey;
    static const std::string AN_CPE7345_NETYPE;
    static const std::string AN_CPE7325_NETYPE;
    static const std::string AN_CPE7310_NETYPE;

    // NE RADIUS
    static const std::string AT_GET_NE_RADIUS_DETAILS;
    static const std::string AN_NE_IDS;
    static const std::string AN_NE_RADIUS_DETAILS;

    //OSM ODUk RCA
    static const std::string AN_COMPOSITE;
    //NE login result
    static const std::string AN_NE_LOGIN_RESULT;
    
};


};


#endif
