/***********************************************************************************************
* COPYRIGHT 1999 - 2005 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: CmDefines
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EMSDefines.cpp  $
* Version: $Revision:   1.2  $
* Description:
* Common Defines Used In CM
*
* Revision History:
*
**********************************************************************************************/

#include "EMSDefines.h"
#include "RandomNumberGenerator.h"

using namespace EMS_FW;


// These are properties for node summary
const string EMSDEF::DESCRIPTION = "EMS_DESCRIPTION";
const string EMSDEF::PARTITIONID = "EMS_PARTITIONID";
const string EMSDEF::POSITION = "EMS_POSITION";
const string EMSDEF::MOC = "EMS_MOC";
const string EMSDEF::MOI = "EMS_MOI";
const string EMSDEF::DLLNAME = "EMS_DLLNAME";
const string EMSDEF::BITMAPHINT = "EMS_BITMAPHINT";
const string EMSDEF::SLOTNUMBER = "EMS_SLOTNUMBER";
const string EMSDEF::POSITIONRIGHT = "EMS_POSITIONRIGHT";
const string EMSDEF::POSITIONLEFT = "EMS_POSITIONLEFT";
const string EMSDEF::POSITIONTOP = "EMS_POSITIONTOP";
const string EMSDEF::POSITIONBOTTOM = "EMS_POSITIONBOTTOM";
const string EMSDEF::NODEINTERFACE = "EMS_NODEINTERFACE";
const string EMSDEF::DEFAULTVIEWTYPE = "EMS_DEFAULTVIEWTYPE";
const string EMSDEF::ALARMSEVERITY = "EMS_ALARMSEVERITY";
const string EMSDEF::PROPALARMSEVERITY = "EMS_PROPALARMSEVERITY";
const string EMSDEF::SOURCE = "EMS_SOURCE";
const string EMSDEF::LOCATION = "EMS_LOCATION";
const string EMSDEF::TL1AID = "EMS_TL1AID";
const string EMSDEF::NODESUMMARYLIST = "EMS_NODESUMMARYLIST";
const string EMSDEF::AUTODISCOVERY_STATE = "EMS_AUTODISCOVER_STATE";
const string EMSDEF::EMSTYPE = "EmsType";
const string EMSDEF::EMSVERSION = "EmsVersion";
//Added by abhishek for MultiNE Type support
const string EMSDEF::NESUBTYPE = "EMS_NESUBTYPE";
//Added by John Chan to include site (i.e. parent) name for NE properties
const string EMSDEF::NE_SITE = "EMS_NE_SITE";


//addded by solay for client recovery
const string EMSDEF::CMAVAILABLE = "CM_AVAILABLE";
const string EMSDEF::FMAVAILABLE = "FM_AVAILABLE";
const string EMSDEF::SGAVAILABLE = "SG_AVAILABLE";

// These are the node::indication reasons
const string EMSDEF::SUBJECTRELOCATED = "SUBJECT_RELOCATED";
const string EMSDEF::SUBJECTMODIFIED = "SUBJECT_MODIFIED";
const string EMSDEF::SUBJECTDELETED = "SUBJECT_DELETED";
const string EMSDEF::CHILDCREATED = "CHILD_CREATED";
const string EMSDEF::CHILDDELETED = "CHILD_DELETED";
const string EMSDEF::BUBBLEUPFAULT = "BUBBLEUP_FAULT";
const string EMSDEF::CMUP = "CM_UP";
const string EMSDEF::CMDOWN = "CM_DOWN";
const string EMSDEF::FMUP = "FM_UP";
const string EMSDEF::FMDOWN = "FM_DOWN";
const string EMSDEF::FMLOSSOFCOMM = "FM_LOSS_COMM";
const string EMSDEF::CMLOSSOFCOMM = "CM_LOSS_COMM";
const string EMSDEF::SMUP = "SM_UP";
const string EMSDEF::SMDOWN = "SM_DOWN";
const string EMSDEF::NMUP = "NM_UP";
const string EMSDEF::NMDOWN = "NM_DOWN";
const string EMSDEF::SYSMONUP = "SYSMON_UP";
const string EMSDEF::SYSMONDOWN = "SYSMON_DOWN";
const string EMSDEF::SYSMONACTIVE = "SYSMON_ACTIVE";
const string EMSDEF::LPMUP = "LPM_UP";
const string EMSDEF::LPMDOWN = "LPM_DOWN";
const string EMSDEF::RMDOWN = "RM_DOWN";
const string EMSDEF::RMUP = "RM_UP";
const string EMSDEF::NBIDOWN = "NBI_DOWN" ;
const string EMSDEF::NBIUP = "NBI_UP";
const string EMSDEF::CRSMODIFIED = "CRSMODIFIED";
// Reasons added for class based notifications (64381)
const string EMSDEF::CLASSENTITYMODIFIED = "CLSENTMOD";
const string EMSDEF::CLASSENTITYADDED = "CLSENTADD";
const string EMSDEF::CLASSENTITYDELETED = "CLSENTDLT";

const string EMSDEF::FMDELETE = "FM_DELETE";
const string EMSDEF::NELOCKED = "NE_LOCKED";
const string EMSDEF::NEUNLOCKED = "NE_UNLOCKED";
const string EMSDEF::CMAUTODISCOVER_INPROGRESS = "CM_AUTODISCOVER_INPROGRESS";
const string EMSDEF::CMAUTODISCOVER_FAIL = "CM_AUTODISCOVER_FAIL";
const string EMSDEF::CMAUTODISCOVER_COMPLETED = "CM_AUTODISCOVER_COMPLETED";
const string EMSDEF::CMAUTODISCOVER_CANCELED = "CM_AUTODISCOVER_CANCELED";
const string EMSDEF::CMAUTODISCOVER_NOTINIT = "CM_AUTODISCOVER_NOTINIT";
const string EMSDEF::ONDEMANDDISCOVERY_INPROGRESS = "ONDEMAND_DISCOVERY_INPROGRESS";
const string EMSDEF::ONDEMANDDISCOVERY_COMPLETED = "ONDEMAND_DISCOVERY_COMPLETED";
const string EMSDEF::ONDEMANDDISCOVERY_FAIL = "ONDEMAND_DISCOVERY_FAIL";
const string EMSDEF::ONDEMAND_SETMODE = "ONDEMAND_SET_MODE";

// Added for network autodiscovery
const string EMSDEF::LINKDISCOVERY_COMPLETED = "LINK_DISCOVERY_COMPLETED";
const string EMSDEF::NTWKDISCOVERY_SYNCHRONIZED = "Synchronized";
const string EMSDEF::NTWKDISCOVERY_SYNCHRONIZING = "Synchronizing";
const string EMSDEF::NTWKDISCOVERY_SCHEDULED = "Scheduled";
const string EMSDEF::NTWKDISCOVERY_UNSCHEDULED = "Unscheduled";
const string EMSDEF::NTWKDISCOVERY_FAILED = "Discovery Failed";
const string EMSDEF::NTWKDISCOVERY_CANCELED = "Discovery Canceled";
const string EMSDEF::NTWKDISCOVERY_ESTABLISHING_SESSION = "Establishing Session";
const string EMSDEF::NTWKDISCOVERY_SYNCHING_TO_NE = "Synching to NE";

const string EMSDEF::NTWKDISCOVERY_MANAGED = "M";
const string EMSDEF::NTWKDISCOVERY_CANDIDATE = "C";
const string EMSDEF::NTWKDISCOVERY_PLANNED = "P";
const string EMSDEF::NTWKDISCOVERY_PLANNED_CANDIDATED = "B";


// Added the following reasons to report to the client that the S/W NE
// went down/is back in service
const string EMSDEF::SWNEUP = "SWNE_UP";
const string EMSDEF::SWNEDOWN = "SWNE_DOWN";

// aaali added for revert, migration and servergroup switch notifications
// these are sent in events to NM and as indication reasons by NM
// to the client so that it can display what's going on for the
// Ne during migration, snpahsot, revert or servergroup switches

// this implues that the Ne version has changed
const string EMSDEF::NEVERSIONCHANGED = "NE_VERSIONCHANGED";
// this implies that the migration has started and is underway
const string EMSDEF::NEMIGRATING = "NE_MIGRATING";
// this implies that a servergroup switch for the Ne has started and is underway
const string EMSDEF::NESWITCHINGSERVERGROUP = "NE_SWITCHINGSERVERGROUP";
// this implies that a revert for the Ne has started and is underway
const string EMSDEF::NEREVERTING = "NE_REVERTING";
// this implies that a revert has been completed for the Ne
const string EMSDEF::NEREVERTCOMPLETE = "NE_REVERTCOMPLETE";
// this implies that a servergroup switch has completed for the Ne
const string EMSDEF::NESWITCHSERVERGROUPCOMPLETE = "NESWITCHSERVERGROUPCOMPLETE";
// this implies that a migration has completed for the Ne
const string EMSDEF::NEMIGRATIONCOMPLETE = "NE_MIGRATIONCOMPLETE";
// aaali added for revert, migration and servergroup switch notifications end
/// aaali_tliaa62259 start
const string EMSDEF::AGENTVERSIONMISMATCH = "AGENTVERSIONMISMATCH";
/// aaali_tliaa62259 end

// values


#ifdef WIN32
const string EMSDEF::SLASH = "\\";
const char EMSDEF::CSLASH = '\\';
const string EMSDEF::EMSPATHSEP = ";";
#else
const string EMSDEF::SLASH = "/";
const char EMSDEF::CSLASH = '/';
const string EMSDEF::EMSPATHSEP = ":";
#endif


const string EMSDEF::True = "true";
const string EMSDEF::False = "false";
const string EMSDEF::EMSSTRING = "EMS_STRING";
const long EMSDEF::MAXEMSQUERY = 10000;
const long EMSDEF::SOURCEANDLOCATIONQUERY = 1;
const long EMSDEF::NE_PARTITION_ID_QUERY = 2;
const long EMSDEF::USERLABELQUERY = 3;
const long EMSDEF::NMQUERYOBJECT = 20000; //greater than the MAXEMSQUERY
const PARTITIONIDTYPE EMSDEF::ADMIN_PARTITION_ID = 0;
const PARTITIONIDTYPE EMSDEF::ROOT_PARTITION_ID = 0;
const PARTITIONIDTYPE EMSDEF::NETWORK_PARTITION_ID = 64;

const EMS_FW::SESSIONIDTYPE EMSDEF::INVALID_SESSION_ID = "-1";

// CM operations IDs.
const string EMSDEF::CM_AUTODISCOVERY_REQUEST = "CM_AUTODISCOVERY_REQUEST";
const string EMSDEF::CM_CANCEL_AUTODISCOVERY = "CM_CANCEL_AUTODISCOVERY";
const string EMSDEF::CM_RESEND_BUBBLEUP = "CM_RESEND_BUBBLEUP";

// ID's for operations on NM
const string EMSDEF::NM_RESTARTNE_REQUEST = "NM_RESTARTNE_REQUEST";

// added by asetlur for TLIaa50210
const string EMSDEF::NM_PINGNE_REQUEST = "NM_PINGNE_REQUEST";
const string EMSDEF::NM_PINGNE_STATUS = "NM_PINGNE_STATUS";

// added by solay
const string EMSDEF::RootManagedElementType = "NE";

// added by lzou
//used for nodesummary
const string EMSDEF::SERVERGROUP_VERSION = "ServerGroup_Version";

//The version here uses same value as the VersionKey in ConfigNames
//On Client side, it will sent the attribute to sysmon with the keys from ConfigNames
//And get the property back and parse it with the keys from EMSDEF
const string EMSDEF::VERSION = "VersionKey";
//used for registration in SysmonServer
//const string EMSDEF::ADAPTER_VERSION_1_1 = "1.1";
//const string EMSDEF::ADAPTER_VERSION_1_2 = "1.2";
//lzou for 3.8
//added for four options of actionName in sysmon_i
const string EMSDEF::GetNeListKey = "getCompleteNeList";
const string EMSDEF::GetSGListKey = "getCompleteSGList";
const string EMSDEF::GetSGListofNeVersionKey = "getSGListofNeVersion";
const string EMSDEF::GetNEListofSGVersionKey = "getNeListofSGVersion";
const string EMSDEF::GetCompleteNeTypeListKey = "getCompleteNeTypeList";
const string EMSDEF::GetSGVersionListofNeTypeKey = "getSGVersionListofNeType";
const string EMSDEF::GetAvailableSGListofNeVersionKey = "getAvailableSGListofNeVersion";

const string EMSDEF::GetSGListofNeVersionandSGVersionKey = "getSGListofNeVersionandSGVersion";

// aaali added for getting sg version and ne versions for migration
const string EMSDEF::GetNextSGVersionAndNEVersionsKey = "GetNextSGVersionAndNEVersions";


//end of 3.8
//lzou for 3.8.2
//added for one option for operationName in SGMServer
const string EMSDEF::UpdateSGMConfig = "UpdateSGMConfigOnthefly";
const string EMSDEF::SGMDisabledForConfigChange = "SGMDisabledForConfigChange";
//end of 3.8.2

// name for extension of MoValue that contain corba Attribute
const string EMSDEF::ATTRIBUTEHOLDERTYPE = "AttributeHolderType";

const string EMSDEF::ServerUser = "ServerUser";



// Shared Sysmon Attributes
// Visible ISM Client Attribute Names
const string EMSDEF::ROOT_TYPE = "ROOT";
const string EMSDEF::HOST_TYPE = "Host_Type";
const string EMSDEF::SERVERGROUP_TYPE = "ServerGroup_Type";
const string EMSDEF::NE_TYPE = "NE_Type";
const string EMSDEF::NOTIFY_SERVER_TYPE = "NOTIFY_SERVER";
const string EMSDEF::EMS_SERVER_TYPE = "EMS_SERVER";
const string EMSDEF::NOTIFY_CHANNEL_TYPE = "NOTIFY_CHANNEL";
const string EMSDEF::SW_SERVERGROUP_TYPE = "SW_SERVERGROUP";
const string EMSDEF::SW_NE_TYPE = "SW_NE";
const string EMSDEF::ADMIN_STATE_KEY = "AdminState_key";
const string EMSDEF::OPERATION_STATE_KEY = "OperationalState_key";
const string EMSDEF::LOCKED = "locked";
const string EMSDEF::UNLOCKED = "unlocked";
const string EMSDEF::ENABLED = "enabled";
const string EMSDEF::DISABLED = "disabled";
const string EMSDEF::NumberOfNesInServerGroupKey = "NumberOfNesInServerGroup";
const string EMSDEF::AvailableNesInServerGroupKey = "AvailableNesInServerGroup";
const string EMSDEF::NumberOfServerGroupsInHostKey = "NumberOfServerGroupsInHost";
const string EMSDEF::AvailableServerGroupsInHostKey = "AvailableServerGroupsInHost";
const string EMSDEF::SERVERGROUP_NAME = "ServerGroupName";
const string EMSDEF::HOST_LOCATION = "Location";
const string EMSDEF::MANAGEDRESOURCE_MAX = "ManagedResourceMax";
const string EMSDEF::MANAGEDRESOURCE_AVAILABLE = "AvailableManagedResources";
const string EMSDEF::ServerGroupToBeMigrated = "ServerGroupToBeMigrated";
const string EMSDEF::SoftwareAlarmSGName = "SoftwareAlarmSG";
const string EMSDEF::MAX_NES_REACHED = "MAXNESREACHED" ;
const string EMSDEF::INVALID_LICENSE_KEY = "MeaningLess";

// Attribute and Config Names
const string EMSDEF::HostName = "HostName";
const string EMSDEF::NetworkAddress = "NetworkAddress";
const string EMSDEF::ServerGroupHostKey = "ServerGroupHost";
const string EMSDEF::NumberOfServerNeActiveKey = "NumberOfServerNeActive";
const string EMSDEF::NeServerGroupKey = "NeServerGroup";
const string EMSDEF::AttributeMarkForDelete = "MarkForDelete";
const string EMSDEF::AttributeRestartFibonacciLower = "AttributeRestartFibonacciLower";
const string EMSDEF::AttributeRestartFibonacciUpper = "AttributeRestartFibonacciUpper";
const string EMSDEF::AttributeRestartFibonacciTime = "AttributeRestartFibonacciTime";
const string EMSDEF::PARENT_ID_KEY = "ParentId_Key";
const string EMSDEF::PARENT_NAME_KEY = "ParentName_Key";

const string EMSDEF::PRIMARY_HOST_NAME = "Primary Host";
const string EMSDEF::ISMROOT_NAME = "ISMROOT";
const string EMSDEF::SW_SERVERGROUP_NAME = "SoftwareAlarm_Servergroup";
const string EMSDEF::SW_NE_NAME = "SoftwareAlarmNe";

const string EMSDEF::CLIENT_NOTIFY_SERVER_ID = "Notify_Client";
const string EMSDEF::GLOBAL_NOTIFY_SERVER_ID = "Notify_Global";
const string EMSDEF::CLIENT_NOTIFY_SERVER_NAMEPREFIX = "Notify_Client";
const string EMSDEF::GLOBAL_NOTIFY_SERVER_NAMEPREFIX = "Notify_Global";

const string EMSDEF::SECURITY_NOTIFY_CHANNEL_ID = "SecurityChannel";
const string EMSDEF::GLOBAL_NOTIFY_CHANNEL_ID = "EMS_GLOBAL";
const string EMSDEF::FM_NOTIFY_CHANNEL_ID = "FmToClient";
const string EMSDEF::CM_NOTIFY_CHANNEL_ID = "CmToClient";
const string EMSDEF::BUBBLEUP_NOTIFY_CHANNEL_ID = "CmToClientAlarmBubbleUpChannel";
const string EMSDEF::SECURITY_NOTIFY_CHANNEL_NAME = "SecurityChannel";
const string EMSDEF::GLOBAL_NOTIFY_CHANNEL_NAME = "EMS_GLOBAL";
const string EMSDEF::FM_NOTIFY_CHANNEL_NAME = "FmToClient";
const string EMSDEF::CM_NOTIFY_CHANNEL_NAME = "CmToClient";
const string EMSDEF::BUBBLEUP_NOTIFY_CHANNEL_NAME = "CmToClientAlarmBubbleUpChannel";


const string EMSDEF::SM_EMS_SERVER_ID = "SM";
const string EMSDEF::NM_EMS_SERVER_ID = "NM";
const string EMSDEF::RM_EMS_SERVER_ID = "RM";
const string EMSDEF::PROXY_EMS_SERVER_ID = "Proxy";
const string EMSDEF::SM_EMS_SERVER_NAMEPREFIX = "SecurityMgr";
const string EMSDEF::NM_EMS_SERVER_NAMEPREFIX = "NodeMgr";
const string EMSDEF::RM_EMS_SERVER_NAMEPREFIX = "RptMgr";
const string EMSDEF::PROXY_EMS_SERVER_NAMEPREFIX = "ProxyServer";







const string EMSDEF::SOFTWARE_ALARM_GROUPNAME = "SoftwareAlarmGroup";
const string EMSDEF::PRIMARY_HOST_GROUPNAME = "PrimaryHostGroup";
const string EMSDEF::NePreviousHostKey = "NePreviousHost";
// aaali added for revert
const string EMSDEF::NePreviousServerGroupKey = "NePreviousServerGroup";
const string EMSDEF::NePreviousVersionKey = "NePreviousVersion";
// aaali added for revert

const string EMSDEF::ADMIN_STATE_ALARM_KEY = "AdminStateAlarmKey";
const string EMSDEF::MigrateNeActionKey = "MigrateNeAction";
const string EMSDEF::RevertNeActionKey = "RevertNeAction";
const string EMSDEF::SnapshotDbActionKey = "SnapshotDbAction";

const string EMSDEF::MoveNodeAction = "MOVE_ACTION"; // for moving Regions and Sites.

const string EMSDEF::AT_GETPROPERTIES = "GETPROPERTIES";
const string EMSDEF::AT_GETCHILDRENPROPERTIES = "GETCHILDRENPROPERTIES";

// aaali_TLIaa62259 - added for storing agent version info during version mismatch
const string EMSDEF::VersionMismtachInfoKey = "VersionMismtachException";
// const string EMSDEF::AgentVersionCheckKey     = "AgentVersionCheck";
// aaali_TLIaa62259 - added for storing agent version info during version mismatch end

///aaali_tliaa63384 start support for deleting snapshot from Ne after migration
const string EMSDEF::DeleteBackupActionKey = "DeleteBackupDbAction";
// support for configuration migration based on product
const string EMSDEF::MigrationModeKey = "MigrationMode";
///aaali_tliaa63384 end
//lzou for 70939
const string EMSDEF::ServerSuffixKey = "ServerSuffix";
//end for 70939

const string EMSDEF::SupportingEnhancedSecurityProperty = "IsSupportingEnhancedSecurity";

// Use for agent filter and scoping rule
const string EMSDEF::AgentFilterKey = "fl_str";
const string EMSDEF::AgentScopeTypeKey = "scope_type";
const string EMSDEF::AgentScopeLevelKey = "scope_level";
const string EMSDEF::AgentFilterDefinedKey = "filter_defined";
const string EMSDEF::AgentScopeDefinedKey = "scope_defined";
// Added by lzou for TL1
const string EMSDEF::TL1ProtocolKey = "TL1";
const string EMSDEF::CMIPTL1ProtocolKey = "CMIPTL1";
const string EMSDEF::ErrorCodeInfoKey = "ErrorCodeInfo";
const string EMSDEF::ErrorDescInfoKey = "ErrorDescInfo";
const string EMSDEF::GetSGInfoKey = "GetSGInfo";
const string EMSDEF::GetBestSGInfoKey = "GetBestSGInfo";
const string EMSDEF::RtrvConfigsFromServerKey = "RtrvConfigsFromServer";
const string EMSDEF::CmConNegotiationMsgKey = "CM_CONNECT";
const string EMSDEF::FmConNegotiationMsgKey = "FM_CONNECT";
const string EMSDEF::CmConNegotiationExpMsgKey = "CM_ACCEPT";
const string EMSDEF::FmConNegotiationExpMsgKey = "FM_ACCEPT";
const string EMSDEF::GATEWAYFUNCTION = "GatewayFunction";
const string EMSDEF::GATEWAYFUNCTIONDIR = "DIR";
const string EMSDEF::GATEWAYFUNCTIONGNEA = "GNEA";
const string EMSDEF::GATEWAYFUNCTIONGNEB = "GNEB";
const string EMSDEF::GATEWAYFUNCTIONRNE = "RNE";
const string EMSDEF::GATEWAYFUNCTIONSNMPRNE = "RemoteSNMPNE";
const string EMSDEF::NETWORKTYPE = "NetworkType";
const string EMSDEF::NETWORKTYPEPRIVATE = "Private";
const string EMSDEF::NETWORKTYPEPUBLIC = "Public";
const string EMSDEF::NETWORKTYPEDIRECT = "Direct";
const string EMSDEF::EONSTATEPUBLIC = "PUBLIC";
const string EMSDEF::EONSTATEPRIVATE = "PRIVATE";
const string EMSDEF::EONTYPEPGNE1 = "PGNE-1";
const string EMSDEF::EONTYPEPGNE2 = "PGNE-2";
const string EMSDEF::EONTYPEPRNE = "PRNE";
const string EMSDEF::EONTYPEDIROSC = "DIR_OSC";
const string EMSDEF::CONTROLSPANNUMBER = "ControlSpanNumber";
const string EMSDEF::DEFAULTGATEWAYNAME = "DefaultGatewayName";
const string EMSDEF::BACKUPGATEWAYNAME = "BackupGatewayName";
const string EMSDEF::DEFAULTIPADDRESS = "DefaultIPAddress";
const string EMSDEF::DEFAULTPORT = "DefaultPort";
const string EMSDEF::BACKUPIPADDRESS = "BackupIPAddress";
const string EMSDEF::BACKUPPORT = "BackupPort";
const string EMSDEF::TL1SHELLIPADDRESS = "TL1ShellIPAddress";
const string EMSDEF::TL1SHELLPORT = "TL1ShellPort";
const string EMSDEF::DEFAULTGATEWAYFUNCTION = "DefaultGatewayFunction";
const string EMSDEF::BACKUPGATEWAYFUNCTION = "BackupGatewayFunction";
const string EMSDEF::ENABLEPEERRELATION = "EnablePeerRelation";

const string EMSDEF::DEFAULTPROXYINTERFACENAME = "Proxy_Default";
const string EMSDEF::ACTION_GETALLGNES = "getAllGNEs";
const string EMSDEF::ACTION_GETNESTATUSLIST = "GetNEStatusList";
const string EMSDEF::ACTION_SETDISCOVERYSTATE = "SetDiscoveryState";
const string EMSDEF::ACTION_SETDISCOVERYPASSWORD = "SetDiscoveryPassword";
const string EMSDEF::ACTION_GETTIDLIST = "GetTIDList";
const string EMSDEF::ACTION_ADDDISCOVERYPASSWORD = "AddDiscoveryPassword";
const string EMSDEF::ACTION_DELETEDISCOVERYPASSWORD = "DeleteDiscoveryPassword";
const string EMSDEF::ACTION_UPDATEDISCOVERYPASSWORD = "UpdateDiscoveryPassword";
const string EMSDEF::ACTION_GET_NE_METERING_CONFIG = "GetMeteringConf";
const string EMSDEF::ACTION_SET_NE_METERING_CONFIG = "SetMeteringConf";
const string EMSDEF::ACTION_REFRESH_NE_METERING_CONFIG = "RefreshMeteringConf";

// action type for getting showArea of Network View
const string EMSDEF::AT_NETWORK_SHOWAREA = "showArea";

// attribute names for Network View
const string EMSDEF::AN_NETWORK_POPULATION = "NetworkPopulation";
const string EMSDEF::AN_NETWORK_BOUNDS = "NetworkBounds";
const string EMSDEF::ACTION_GETDISCOVERYPASSWORD = "GetDiscoveryPassword";

const string EMSDEF::AT_NETWORK_GETEMSCOORD = "AT_NETWORK_GETEMSCOORD";
const string EMSDEF::AT_NETWORK_SETEMSCOORD = "AT_NETWORK_SETEMSCOORD";
const string EMSDEF::AN_NETWORK_EMSCOORDLIST = "AN_NETWORK_EMSCOORDLIST";
const string EMSDEF::AN_NETWORK_EMSCOORD_TID = "AN_NETWORK_EMSCOORD_TID";
const string EMSDEF::AN_NETWORK_EMSCOORD_CLLI = "AN_NETWORK_EMSCOORD_CLLI";
const string EMSDEF::AN_NETWORK_EMSCOORD_DEFINEDBYCLLI = "AN_NETWORK_EMSCOORD_DEFINEDBYCLLI";
const string EMSDEF::AN_NETWORK_EMSCOORD_COORDBLANK = "AN_NETWORK_EMSCOORD_COORDBLANK";
const string EMSDEF::AT_NETWORK_BULK_SETEMSCOORD = "BULK_SETEMSCOORD";
const string EMSDEF::AN_NETWORK_EMSCOORDLIST_NEW = "EMSCOORDLIST_NEW";
const string EMSDEF::AN_NETWORK_EMSCOORDLIST_EXISTING = "EMSCOORDLIST_EXISTING";

const string EMSDEF::AT_GETLINKS = "GETLINKS";  // To query for the list of DWDM links
const string EMSDEF::AN_LINK_IDS = "LinkIds";
const string EMSDEF::AN_LINK_NAMES = "LinkNames";

const string EMSDEF::AN_RCA_RESULT = "RootCauseAlarms";
const string EMSDEF::AT_DISCOVER_CIRCUIT_RCA = "DISCOVERCIRCUITRCA";
const string EMSDEF::AN_RCA_EXCEPTION = "RCAEXCEPTION";
const string EMSDEF::AN_RCA_PARTIAL_ROUTES = "PARTIALRCAROUTES";
// End of TL1

const string EMSDEF::SNMPProtocolKey = "SNMP";
const string EMSDEF::AN_CPE7345_NETYPE = "7345";
const string EMSDEF::AN_CPE7325_NETYPE = "7325";
const string EMSDEF::AN_CPE7310_NETYPE = "7310";

// NE RADIUS
const string EMSDEF::AT_GET_NE_RADIUS_DETAILS="GET_NE_RADIUS_DETAILS";
const string EMSDEF::AN_NE_IDS="NE_IDS";
const string EMSDEF::AN_NE_RADIUS_DETAILS="NE_RADIUS_DETAILS";

// OSM40 ODUk RCA
const string EMSDEF::AN_COMPOSITE="COMPOSITE";
const string EMSDEF::AN_NE_LOGIN_RESULT="NE_LOGIN_RESULT";
