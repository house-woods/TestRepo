/***********************************************************************************************
* COPYRIGHT 1999 - 2004 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: ConfigNames
* Type: C++
* Originator: $Author:   cballard  $
* File: $Workfile:   ConfigNames.cpp  $
* Version: $Revision:   1.5  $
* Description:
*  Contain Names used in configuration files
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/ConfigNames.cvp  $

Rev 1.5   Oct 19 1999 16:32:44   cballard
Synchronized with 1.4

Rev 1.4   Oct 18 1999 16:49:26   vlawande
Software ne
**********************************************************************************************/


#include "ConfigNames.h"

using namespace EMS_FW;

/**********************************************************
 *
 *  Constants defined which can be used by any servers...
 *
 **********************************************************/
const string ConfigNames::True = "true";
const string ConfigNames::False = "false";
const string ConfigNames::ReaderMode = "r";
const string ConfigNames::WriterMode = "w";
const string ConfigNames::Null = "";
const string ConfigNames::Btm = "btm";
const string ConfigNames::Ots = "ots";
const string ConfigNames::Occs = "OCCS";
const string ConfigNames::An2100 = "an2100";
const string ConfigNames::SoftwareNe = "SoftwareNe";
const string ConfigNames::LpmAlarmNeId = "LpmAlarmNe";
const string ConfigNames::SoftwareAlarmNeId = "SoftwareAlarmNe";
const string ConfigNames::SoftwareAlarmNeName = "SoftwareAlarm";

/**********************************************************
 *
 *  Commands defined which can be used by any servers...
 *
 **********************************************************/
const string ConfigNames::CommandStartLogging = "StartLogging";
const string ConfigNames::CommandStopLogging = "StopLogging";
const string ConfigNames::CommandSetTraceLevel = "SetTraceLevel";
const string ConfigNames::EnableSyncFromLog = "EnableSyncFromLog";
const string ConfigNames::CommandChangeNePassword = "ChangeNePassword";


/**********************************************************
 *
 *  Configuration sent to all the EMS servers during startup...
 *
 **********************************************************/ 
// Names common to all servers....
const string ConfigNames::EmsIdKey = "EmsId";
const string ConfigNames::EmsTypeKey = "EmsType";
const string ConfigNames::EmsVersionKey = "EmsVersion";
const string ConfigNames::IdKey = "Id";
const string ConfigNames::ColdStartKey = "ColdStart";
const string ConfigNames::ServerColdStartKey = "ServerColdStart";
const string ConfigNames::SetSMWarmStartKey = "SetSMWarmStart";
const string ConfigNames::ServerNameKey = "ServerName";
const string ConfigNames::SecurityChannelNameKey = "SecurityChannelName";
const string ConfigNames::SysmonStateChangeChannelNameKey = "SysmonStateChangeChannelName";
//TLIaa54672 Added by jhu
const string ConfigNames::SgmStateChangeChannelNameKey = "SgmStateChangeChannelName";

const string ConfigNames::FmToClientChannelNameKey = "FmToClientChannelName";
const string ConfigNames::CmToClientChannelNameKey = "CmToClientChannelName";
const string ConfigNames::CmToClientAlarmBubbleUpChannelNameKey = "CmToClientAlarmBubbleUpChannelName";

const string ConfigNames::EmsGlobalChannelNameKey = "EmsGlobalChannelName";

const string ConfigNames::ProxyTimeOutKey = "ProxyTimeOut";
const string ConfigNames::SysmonTimeOutKey = "SysmonTimeOut";
const string ConfigNames::EnableProxyConsoleKey = "EnableProxyConsole";
const string ConfigNames::ProxyExecDirKey = "ProxyExecDir";
const string ConfigNames::EnableProxyLogKey = "EnableProxyLog";
const string ConfigNames::RestoreActiveSessions = "RestoreActiveSessions"; // this value should never be set in the config files.
const string ConfigNames::RestoreActiveSessionTimeOutKey = "RestoreActiveSessionTimeOutKey";
const string ConfigNames::startProxyCmdKey = "startProxyCmd";
const string ConfigNames::CMIPListenPortNameKey = "CMIPListenPortName";
// this is used internally as a flag b/w Sysmon and AM.
const string ConfigNames::ShutDownTimeOutKey = "ShutDownTimeOut";
const string ConfigNames::MaxProxyQueKey = "MaxProxyQue";

const string ConfigNames::FcpStateChangeChannelNameKey = "FcpStateChangeChannelName";
const string ConfigNames::SysmonInterfaceNameKey = "SysmonInterfaceName";
const string ConfigNames::NeTypeKey = "NeType";
const string ConfigNames::NeVersionKey = "NeVersion";
const string ConfigNames::NeNoVersion = "NoVersion";
const string ConfigNames::TraceLevelKey = "TraceLevel";
const string ConfigNames::TraceListKey = "TraceList";
const string ConfigNames::GlobalHeapCheckKey = "GlobalHeapCheck";
const string ConfigNames::WriteToExceptionFileKey = "WriteToExceptionFile";
const string ConfigNames::LogAgentNotificationsKey = "LogAgentNotifications";
const string ConfigNames::CraftStationModeKey = "CraftStationMode";
const string ConfigNames::MaxLexicalTransactionRetriesKey = "MaxLexicalTransactionRetries";
const string ConfigNames::AutodiscoverDelayKey = "AutodiscoverDelay";
const string ConfigNames::AutodiscoverAttemptsKey = "AutodiscoverAttempts";
const string ConfigNames::ExecuteCMAutodiscoverKey = "ExecuteCMAutodiscover";
const string ConfigNames::CMAutodiscoveryMethodKey = "CMAutodiscoverMethod";
const string ConfigNames::CMLayeredAutodiscovery = "CMLayeredAutodiscovery";
const string ConfigNames::CMDirectAutodiscovery = "CMDirectAutodiscovery";
const string ConfigNames::CMLeveledAutodiscovery = "CMLeveledAutodiscovery";
const string ConfigNames::CMAutodiscoveryObjectsPerTxnKey = "CMAutodiscoveryObjectsPerTxn";
const string ConfigNames::CMAutodiscoveryEnableAttributesCheckKey = "CMAutodiscoveryEnableAttributesCheck";
const string ConfigNames::CMMultiThreadedAutodiscoveryKey = "CMMultiThreadedAutodiscovery";
const string ConfigNames::HandleServerErrorsKey = "HandleServerErrors";
const string ConfigNames::RestartFailedNetworkElementsKey = "RestartFailedNetworkElements";
const string ConfigNames::ProxyAgentCount = "ProxyAgentPortCount";
const string ConfigNames::AgentCallTimeOutKey = "AgentCallTimeOut";
const string ConfigNames::ServerToProxyTimeOutKey = "ServerToProxyTimeOut";
const string ConfigNames::ProxyConnectTimeOutKey = "ProxyConnectTimeOut";
//TLIaa54672 Added by jhu
const string ConfigNames::ServerGroupIdKey = "ServerGroupId";
const string ConfigNames::ServerGroupNameKey = "ServerGroupName";
const string ConfigNames::NamingServiceHostNameKey = "NamingServiceHostName";
const string ConfigNames::NamingServicePortNumberKey = "NamingServicePortNumber";
const string ConfigNames::STORBConfigFileNameKey = "STORBConfigFileName";
const string ConfigNames::MTORBConfigFileNameKey = "MTORBConfigFileName";
const string ConfigNames::SMORBConfigFileNameKey = "SMORBConfigFileName";
const string ConfigNames::OrbConfigParametersKey = "OrbConfigParameters";

//Sysmon server specific names....
const string ConfigNames::NotifyLocationKey = "NotifyLocation";
const string ConfigNames::NotifyConfigParametersKey = "NotifyConfigParameters";
const string ConfigNames::NotifyDebugLevelKey = "NotifyDebugLevel";


// Cm server specific names ...
const string ConfigNames::ModeKey = "Mode";
const string ConfigNames::ClientMgrNameKey = "ClientMgrName";
const string ConfigNames::CmipMoiHeaderKey = "CmipMoiHeader";
const string ConfigNames::MapFileNameKey = "MapFileName";
const string ConfigNames::AddMoiHeaderKey = "AddMoiHeader";
const string ConfigNames::FilterKey = "Filter";
const string ConfigNames::OD_FilterKey = "OD_Filter";
const string ConfigNames::DllNameKey = "DllName";
const string ConfigNames::NmDllNameKey = "NmDllName";
const string ConfigNames::FmToCmBubbleupChannelNameKey = "FmToCmBubbleupChannelName";
const string ConfigNames::NeSubTypeKey = "NeSubType";

// Nm server specific names
const string ConfigNames::NodeDbPathKey = "NodeDbPath";
const string ConfigNames::NodeManagerInterfaceNameKey = "NodeManagerInterfaceName";
const string ConfigNames::CmToNmBubbleupChannelNameKey = "CmToNmBubbleupChannelName";
const string ConfigNames::DefaultNeLicenseKey = "DefaultNeLicenseKey";
const string ConfigNames::NMOperationTimeoutKey = "NMOperationTimeout";
const string ConfigNames::NMCreateNodeTimeoutKey = "NMCreateNodeTimeout";
const string ConfigNames::NMToCMExecuteActionTimeoutKey = "NMToCMExecuteActionTimeout";


// Report Manager specific names
const string ConfigNames::ReportManagerInterfaceNameKey = "ReportManagerInterfaceName";


// SM server specific names...
const string ConfigNames::SecurityDbPathKey = "SecurityDbPath";
const string ConfigNames::SecurityADbPathKey = "SecurityADbPath";
const string ConfigNames::SecurityClientInterfaceNameKey = "SecurityClientInterfaceName";
const string ConfigNames::SecurityAdminInterfaceNameKey = "SecurityAdminInterfaceName";
const string ConfigNames::SecurityServerInterfaceNameKey = "SecurityServerInterfaceName";
const string ConfigNames::SecurityProfileInterfaceNameKey = "SecurityProfileInterfaceName";
const string ConfigNames::MultiLogonAllowedKey = "MultiLogonAllowed";
const string ConfigNames::EnableClientActiveCheckKey = "EnableClientActiveCheck";
const string ConfigNames::ClientPollingPeriodKey = "ClientPollingPeriod";
const string ConfigNames::MaxNumberOfClientsKey = "MaxNumberOfClients";
const string ConfigNames::SmToClientRestoreSessionMaxRetryKey = "SmToClientRestoreSessionMaxRetry";
const string ConfigNames::SmToClientActiveCheckTimeoutKey = "SmToClientActiveCheckTimeout";
const string ConfigNames::TimeDeltaThreshold = "TimeDeltaThreshold";
const string ConfigNames::sleepTimeForTerminateSessionKey = "sleepTimeForTerminateSession";
const string ConfigNames::DestroyChannelTimeOutKey = "DestroyChannelTimeOut";



// Fm specific names...
const string ConfigNames::TranslatorFileNameKey = "TranslatorFileName";
const string ConfigNames::FmlToFmrChannelNameKey = "FmlToFmrChannelName";
const string ConfigNames::BubbleupInterfaceNameKey = "BubbleupInterfaceName";
const string ConfigNames::ArchiveDbPathKey = "ArchiveDbPath";
const string ConfigNames::PriArchiveDbPathKey = "PriArchiveDbPath";
const string ConfigNames::SecArchiveDbPathKey = "SecArchiveDbPath";
const string ConfigNames::AutoAgingPeriodKey = "AutoAgingPeriod";
const string ConfigNames::AgingKeepTimeKey = "AgingKeepTime";
const string ConfigNames::AgingEnabledKey = "AgingEnabled";
const string ConfigNames::ArchiveStartTimeKey = "ArchiveStartTime";
const string ConfigNames::ArchiveTimeSpanKey = "ArchiveTimeSpan";
const string ConfigNames::PriArchivePrefixKey = "PriArchivePrefix";
const string ConfigNames::SecArchivePrefixKey = "SecArchivePrefix";
const string ConfigNames::ArchiveBackupIntervalKey = "ArchiveBackupInterval";
const string ConfigNames::DeleteBatchFileKey = "DeleteBatchFile";
const string ConfigNames::MoveBatchFileKey = "MoveBatchFile";
const string ConfigNames::FMmaxNoOfAlarmsPerTxnKey = "FMmaxNoOfAlarmsPerTxn";

//////////////////////////////////////////////////////
// for SW Upgrade client support ...
const string ConfigNames::NeVersionSpecificKey = "NeVersionSpecificList";
const string ConfigNames::SgVersionSpecificKey = "SgVersionSpecificList";
// Used for all kinds of Resource
// provided by user
const string ConfigNames::VersionKey = "VersionKey";
/////////////////////////////////////////////////////////

// LPM server specific names...
const string ConfigNames::LpmUserNameKey = "LpmUserName";
const string ConfigNames::LpmPasswordKey = "LpmPassword";
const string ConfigNames::LpmCmdSleepTimeKey = "LpmCmdSleepTime";
const string ConfigNames::LpmAutoDiscSleepTimeKey = "LpmAutoDiscSleepTime";
const string ConfigNames::LpmMoLiteDatabaseNameKey = "LpmMoLiteDatabaseName";
const string ConfigNames::LpmDBChannelNameKey = "LpmDBChannelName";
const string ConfigNames::LpmInstanceNameKey = "LpmInstanceName";
const string ConfigNames::LpmClientMgrNameKey = "LpmClientMgrName";
const string ConfigNames::LpmServerNameKey = "LpmServerName";
const string ConfigNames::LpmLpmMgrNameKey = "LpmLpmMgrName";

// RingManager specific Names
const string ConfigNames::RMInstanceNameKey = "RMInstanceName";
const string ConfigNames::RMMoLiteDatabaseNameKey = "RMMoLiteDatabaseName";
const string ConfigNames::RMServerNameKey = "RMServerName";


// NBI server specific names ...
const string ConfigNames::NbiTL1ChannelNameKey = "NbiTL1ChannelName";
const string ConfigNames::NbiMetaModelTypeKey = "NbiMetaModelType";
const string ConfigNames::NbiAdapterTypeKey = "NbiAdapterType";
const string ConfigNames::NbiAdapterIdKey = "NbiAdapterId";
const string ConfigNames::NbiAdapterPortNumKey = "NbiAdapterPortNum";
const string ConfigNames::NbiAdapterMaxConnKey = "NbiAdapterMaxConn";
const string ConfigNames::NbiTidKey = "NbiTid";
const string ConfigNames::NbiTL1CmdGrammarFileKey = "NbiTL1CmdGrammarFile";
const string ConfigNames::ClientServiceName = "SecurityClient_TLAB";
const string ConfigNames::NbiTL1FeatureEnabledKey = "NbiTL1FeatureEnabled";
const string ConfigNames::NbiTL1MapFileKey = "NbiTL1MapFile";

const string ConfigNames::NbiTL1MaxIdleTimeKey = "NbiTL1MaxIdleTime";
const string ConfigNames::NbiTL1AlarmTextMessage = "NbiTL1AlarmTextMessage";
const string ConfigNames::NbiEnableAlarmMessaging = "NbiEnableAlarmMessaging";

const string ConfigNames::NbiKeepAliveIntervalKey = "NbiKeepAliveInterval";

/**********************************************************
 *
 *  Configuration sent to all the EMS servers while adding
 *  a new Network element
 *
 **********************************************************/ 
// Names Common to all servers...
const string ConfigNames::ServerSessionId = "ServerSessionId";
const string ConfigNames::NeColdStartKey = "NeColdStart";
const string ConfigNames::NeColdStopKey = "NeColdStop";
const string ConfigNames::NeIdKey = "NeId";
const string ConfigNames::NeNameKey = "NeName";
const string ConfigNames::AgentHostKey = "AgentHostName";
const string ConfigNames::AgentPortKey = "AgentPortName"; // Proxy
const string ConfigNames::ProtocolKey = "ProtocolName";
const string ConfigNames::EfdNameKey = "EfdName";
const string ConfigNames::MigrationInProgressKey = "MigrationInProgress";
const string ConfigNames::NotificationMaxRetriesQosKey = "NotificationMaxRetriesQos";
const string ConfigNames::NotificationRequestTimeoutQosKey = "NotificationRequestTimeoutQos";
const string ConfigNames::NotificationRetryTimeoutQosKey = "NotificationRetryTimeoutQos";
const string ConfigNames::NotificationRetryMultiplierQosKey = "NotificationRetryMultiplierQos";
const string ConfigNames::NotificationMaxRetryTimeoutQosKey = "NotificationMaxRetryTimeoutQos";
const string ConfigNames::MaxAddNERetriesKey = "MaxAddNERetries";

// For Network autodiscovery
const string ConfigNames::DBMonitorIntervalKey = "DBMonitorInterval";
const string ConfigNames::QueueMonitorIntervalKey = "QueueMonitorInterval";
const string ConfigNames::EnableNetworkAutodiscoveryKey = "EnableNetworkAutodiscovery";
const string ConfigNames::DefaultNEPasswordKey = "DefaultNEPassword";
const string ConfigNames::DefaultNEUserIdKey = "DefaultNEUserId";


// Cm server specific names
const string ConfigNames::RootMoiKey = "RootMoiName";
const string ConfigNames::RootMocKey = "RootMocName";
const string ConfigNames::AgentParamKey = "AgentParam";
const string ConfigNames::CmNodeInterfaceNameKey = "CmNodeInterfaceName";
const string ConfigNames::MoLiteDatabaseNameKey = "MoLiteDatabaseName";
const string ConfigNames::MoDatabaseNameKey = "MoDatabaseName";
const string ConfigNames::NeParentSiteIdKey = "NeParentIdKey";
const string ConfigNames::CmEfdNameKey = "CmEfdName";


// Fm server specific names...
const string ConfigNames::SocketTypeKey = "SocketType";
const string ConfigNames::AlarmDatabaseNameKey = "AlarmDatabaseName";
const string ConfigNames::BubbleUpDatabaseNameKey = "BubbleUpDatabaseName";
const string ConfigNames::FmwInterfaceNameKey = "FmwInterfaceName";
const string ConfigNames::FmrInterfaceNameKey = "FmrInterfaceName";
const string ConfigNames::FmlInterfaceNameKey = "FmlInterfaceName";
const string ConfigNames::FmEfdNameKey = "FmEfdName";
const string ConfigNames::ExecuteFMAutodiscoverKey = "ExecuteFMAutodiscover";



/**********************************************************
 *
 *  Configuration used by sysmon....
 *
 **********************************************************/
const string ConfigNames::DbPathKey = "DbPath";
const string ConfigNames::ExecutableDirectoryKey = "ExecutableDirectory";
const string ConfigNames::LogDirectoryKey = "LogDirectory";
const string ConfigNames::LogSizeKey = "LogSize";
//TLIaa54672 Added by jhu
const string ConfigNames::SgmExecutableNameKey = "SgmExecutableName";
const string ConfigNames::AmExecutableNameKey = "AmExecutableName";
const string ConfigNames::SmExecutableNameKey = "SmExecutableName";
const string ConfigNames::NmExecutableNameKey = "NmExecutableName";
const string ConfigNames::RingMgrExecutableNameKey = "RingMgrExecutableName";
const string ConfigNames::LpmrExecutableNameKey = "LpmrExecutableName";
const string ConfigNames::LpmwExecutableNameKey = "LpmwExecutableName";
const string ConfigNames::CmExecutableNameKey = "CmExecutableName";
const string ConfigNames::FmrExecutableNameKey = "FmrExecutableName";
const string ConfigNames::FmwExecutableNameKey = "FmwExecutableName";
const string ConfigNames::FmlExecutableNameKey = "FmlExecutableName";
const string ConfigNames::ServerConfigFileNameKey = "ServerConfigFileName";
const string ConfigNames::NeConfigFileNameKey = "NeConfigFileName";
const string ConfigNames::SecurityConfigFileNameKey = "SecurityConfigFileName";
const string ConfigNames::VersionConfigFileNameKey = "VersionConfigFileName";
const string ConfigNames::LeastClientVersionKey = "LeastClientVersion";
const string ConfigNames::LatestClientVersionKey = "LatestClientVersion";
const string ConfigNames::CurrentServerVersionKey = "CurrentServerVersion";
const string ConfigNames::StartLpmAlarmNeKey = "StartLpmAlarmNe";
const string ConfigNames::StartSoftwareAlarmNeKey = "StartSoftwareAlarmNe";
const string ConfigNames::CraftStationManagedNeKey = "CraftStationManagedNe";
const string ConfigNames::MaximumServerGroupsPerHostKey = "MaximumServerGroupsPerHost";
const string ConfigNames::MaximumNesPerServerGroupKey = "MaximumNesPerServerGroup";
const string ConfigNames::SysmonDbNameKey = "SysmonDatabaseName";
const string ConfigNames::ServerPollingIntervalKey = "ServerPollingInterval";
const string ConfigNames::ServerPingTimeoutKey = "ServerPingTimeout";
const string ConfigNames::ServerOperationTimeoutKey = "ServerOperationTimeout";
const string ConfigNames::NbiExecutableNameKey = "NbiExecutableName";
const string ConfigNames::PrimaryHostKey = "PrimaryHost";
const string ConfigNames::SysmonInitializingKey = "SysmonInitializingKey";
const string ConfigNames::RMLicenseCodeKey = "RMLicenseCode";
const string ConfigNames::LPMLicenseCodeKey = "LPMLicenseCode";
const string ConfigNames::LPMLicenseGrantedKey = "LPMLicenseGranted";
const string ConfigNames::RMLicenseGrantedKey = "RMLicenseGranted";
const string ConfigNames::SysmonRecoveryKey = "SysmonRecovery";


//TLIaa54672 Added by jhu
const string ConfigNames::InfrastructureServerKey = "InfrastructureServer";

const string ConfigNames::InfraServerRecoveryRetryKey = "InfraServerRecoveryRetry";
const string ConfigNames::ParentServerNameKey = "ParentServerName";

const string ConfigNames::HMOperationTimeOutKey = "HMOperationTimeOut";
const string ConfigNames::HMLaunchRetryKey = "HMLaunchRetry";

const string ConfigNames::SysmonHMInitRetryMaxKey = "SysmonHMInitRetryMax";
const string ConfigNames::SysmonHMInitRetryDelayKey = "SysmonHMInitRetryDelay";


//const string ConfigNames::PrimaryHostKey     = "PrimaryHost";
// added by lzou for version support
// in Sysmon.cfg
const string ConfigNames::VersionListKey = "VersionListKey";
const string ConfigNames::ServerConfigListKey = "ServerConfigListKey";
const string ConfigNames::NeConfigListKey = "NeConfigListKey";
const string ConfigNames::DelimitorKey = "DelimitorKey";
// Used for every version Server Group in sysmon.cfg (e.g
// NeVersionList0 = OTS1.1|OTS1.1c|OTS1.2|
// NeVersionList1 = OTS2.1|OTS2.2|OTS2.3|)
// if VersionList = 1.1|1.2|
const string ConfigNames::NeVersionListKey = "NeVersionListKey";
// every adapter may support multiple versions of SG.
// It will be specified like
// Adapter1=1.1|
//  Adapter2=1.2|1.3|
// The number of adapters reflects how many IDL should be used while
// the number of versions reflects how many versions of executables
const string ConfigNames::Adapter1 = "Adapter1";
const string ConfigNames::Adapter2 = "Adapter2";
const string ConfigNames::Adapter = "Adapter";

const string ConfigNames::SWAlarmSGVersionKey = "SWAlarmSGVersion";
const string ConfigNames::SWAlarmForNBIKey = "SWAlarmForNBI";

//Jar file name for this server group verison servers
const string ConfigNames::BaseJarFileNameKey = "BaseJarFileName";

const string ConfigNames::MigrationConfigFileNameKey = "MigrationConfigFileName";
const string ConfigNames::AddNEOperationTimeoutKey = "AddNEOperationTimeout";

const string ConfigNames::AutoMigrationKey = "AutoMigration";

//start threadpool support for sysmon warmstart
const string ConfigNames::NumofHostThreadatWarmStartKey = "NumofHostThreadatWarmStart";
const string ConfigNames::NumofSgThreadatWarmStartKey = "NumofSgThreadatWarmStart";
const string ConfigNames::SysmonWaitTimeForHMToUpKey = "SysmonWaitTimeForHMToUp";
const string ConfigNames::SysmonSleepTimeForObjectStoreToUpKey = "SysmonSleepTimeForObjectStoreToUp";
const string ConfigNames::SysmonRetryForObjectStoreToUpKey = "SysmonRetryForObjectStoreToUp";


//end threadpoolsupport

//added by solay for Proxy Agent
const string ConfigNames::SNMPEnableKey = "SNMPEnable";
const string ConfigNames::SNMPAgentRootDirKey = "SNMPAgentRootDir";
const string ConfigNames::EnableSecInterfaceKey = "EnableSecInterface";
const string ConfigNames::SecInterfaceIpAddrKey = "SecInterfaceIpAddr";
const string ConfigNames::AddNeKey = "AddNe";
const string ConfigNames::AgentRootDirKey = "AgentRootDir";
const string ConfigNames::AgentNameKey = "AgentName";
const string ConfigNames::X723applicationProcessTitleKey = "X723applicationProcessTitle";
const string ConfigNames::AgentDatabaseBatchFilekey = "AgentDatabaseBatchFile";

const string ConfigNames::ProxyAgentExeKey = "ProxyAgentExe";
const string ConfigNames::callingApTitleDnKey = "callingApTitleDn";

const string ConfigNames::pSelectorKey = "pSelector";
const string ConfigNames::sSelectorKey = "sSelector";
const string ConfigNames::tSelectorKey = "tSelector";
const string ConfigNames::nSelectorKey = "nSelector";

const string ConfigNames::PHexadecimalKey = "PHEXADECIMAL";
const string ConfigNames::SHexadecimalKey = "SHEXADECIMAL";
const string ConfigNames::THexadecimalKey = "THEXADECIMAL";



const string ConfigNames::PartitionIdkey = "NE_PARTITIONID";

const string ConfigNames::DSETAgentOidKey = "DSETAgentOid";
const string ConfigNames::ProxyOidKey = "ProxyOid";
const string ConfigNames::ApQualifierIdKey = "ApQualifierId";
const string ConfigNames::RemoteAeTitleKey = "RemoteAeTitle";

const string ConfigNames::CmEfdFilterKey = "CmEfdFilter";
const string ConfigNames::FmEfdFilterKey = "FmEfdFilter";

// Added by Shangping for system defensiveness
const string ConfigNames::PasswordEnabledKey = "PasswordEnabled";
const string ConfigNames::PasswordKey = "Password";

const string ConfigNames::AgentReConnectDelayKey = "AgentReConnectDelay";

const string ConfigNames::qosflagKey = "qosflag";
const string ConfigNames::qosreliabilityKey = "qosreliability";
const string ConfigNames::qossversionKey = "qossversion";
const string ConfigNames::qosextendedKey = "qosextended";
const string ConfigNames::qosmaxtimeKey = "qosmaxtime";

const string ConfigNames::OSINSAPKey = "OSINSAP";
const string ConfigNames::OSIStackFlagKey = "OSIStackFlag";

const string ConfigNames::DirectReferenceKey = "DirectReference";

// Added by hashraf for 7100 Enhanced Security
const string ConfigNames::SGVersionsSupportingEnhancedSecurityKey = "SGVersionsSupportingEnhancedSecurity";
// end hashraf

// To configure the topology root name
const string ConfigNames::TopologyRootNameKey = "TopologyRootName";

// Smat specific names
const string ConfigNames::SMATDbPathKey = "SMATDbPath";
const string ConfigNames::SMATEMSDbNameKey = "SMATEMSDbName";

// SMAT Database Archiving
const string ConfigNames::SMATArchiveEnabledKey = "SMATArchiveEnabled";
const string ConfigNames::PrimarySMATArchivePathKey = "PrimarySMATArchivePath";
const string ConfigNames::PrimarySMATArchivePrefixKey = "PrimarySMATArchivePrefix";
const string ConfigNames::SMATPriArchiveIntervalsKey = "SMATPriArchiveIntervals";
const string ConfigNames::SecondarySMATArchivePathKey = "SecondarySMATArchivePath";
const string ConfigNames::SecondarySMATArchivePrefixKey = "SecondarySMATArchivePrefix";
const string ConfigNames::OffloadedSMATArchivePathKey = "OffloadedSMATArchivePath";
const string ConfigNames::OffloadedSMATArchivePrefixKey = "OffloadedSMATArchivePrefix";
const string ConfigNames::SMATSecArchiveIntervalsKey = "SMATSecArchiveIntervals";
const string ConfigNames::SMATOffloadBatchFileNameKey = "SMATOffloadBatchFileName";
const string ConfigNames::SMATNEDbNamePrefixKey = "SMATNEDbNamePrefix";
const string ConfigNames::SMATAutoAgingIntervalKey = "SMATAutoAgingInterval";

// HM Specific Names
const string ConfigNames::ProcessDataFile = "ProcessDataFile";
const string ConfigNames::HmTimeout = "HmTimeout";
const string ConfigNames::CreateNewConsoleForServer = "CreateNewConsoleForServer";
const string ConfigNames::ServerRootContext = "ServerRootContext";
const string ConfigNames::HmThreadCount = "HmThreadCount";
const string ConfigNames::HmOrbProcessList = "HmOrbProcessList";
const string ConfigNames::OrbixDomainIsFileBased = "OrbixDomainIsFileBased";
const string ConfigNames::isPrimaryHost = "isPrimaryHost";
const string ConfigNames::ThirdPartyServerCount = "ThirdPartyServerCount";
;
const string ConfigNames::ThirdPartyServerLine = "ThirdPartyServer_";
// In the config file, a number is appended to the end of ThirdPartyServerLine
// for each server, e.g. ThirdPartyServer_1, ThirdPartyServer_2, etc.
const string ConfigNames::ObjectStoreServerName = "osserver";
const string ConfigNames::ObjectStoreCacheMgrName = "oscmgr";
const string ConfigNames::HMInitRetryMaxKey = "HMInitRetryMax";
const string ConfigNames::HMInitRetryDelayKey = "HMInitRetryDelay";

// Added by hwang
const string ConfigNames::GetHMServerRefTimeout = "GetHMServerRefTimeout";

// aaali_tliaa62259 start
const string ConfigNames::AgentVersionCheckKey = "AgentVersionCheck";
// aaali_tliaa62259 end
// MR: 85945 for lzou
const string ConfigNames::LocAgentVersionCheckKey = "LocAgentVersionCheck";
// end of 85945
// Configuration used for Switch from EM to ISM system
// Suppose we should use parameter like ISMSupported. However, we don't want user to figure out
// this parameter easily so that they can change the EM system into ISM system easily. That's why
// we use this strange name to hide this parameter.
const string ConfigNames::ISMSupportedKey = "DelayIntervalNumber";

// Configuration used for On Demand Discovery
const string ConfigNames::OnDemandSupportedKey = "OnDemandDiscovery";

//lzou for 73519
const string ConfigNames::NeTypeListKey = "NeTypeList";
const string ConfigNames::NeProductTypeKey = "NeProductType";
//end of 73519


//Configuration used for DSS SNMP Network Element
const string ConfigNames::GetSetCommunityString = "GetSetCommunityString";
const string ConfigNames::TrapCommunityString = "TrapCommunityString";
const string ConfigNames::SnmpResponseTimeout = "SnmpResponseTimeout";
const string ConfigNames::ConnectivityPollFrequency = "ConnectivityPollFrequency";
const string ConfigNames::SynchronizationFrequency = "SynchronizationFrequency";
const string ConfigNames::RetriesOnCommunicationFailure = "RetriesOnCommunicationFailure";
const string ConfigNames::ProxyManagementProtocol = "ProxyManagementProtocol";
const string ConfigNames::ProxySupportedMIBS = "ProxySupportedMIBS";
const string ConfigNames::MaxVarBindsPerPDU = "MaxVarBindsPerPDU";
const string ConfigNames::TrapPortAddress = "TrapPortAddress";
const string ConfigNames::SNMPSupport = "SNMPSupport";
const string ConfigNames::SNMPSupportPort = "SNMPSupportPort";
const string ConfigNames::PrimaryHostIPAddress = "PrimaryHostIPAddress";


//Configuration used for AMS Network Element
const string ConfigNames::M3100alarmStatus = "M3100alarmStatus";
const string ConfigNames::UserId = "UserId";
const string ConfigNames::TID = "TID";
const string ConfigNames::Password = "Password";
const string ConfigNames::M3100vendorName = "M3100vendorName";
const string ConfigNames::M3100networkName = "M3100networkName";
const string ConfigNames::Q3TL1keepAlivePeriod = "Q3TL1keepAlivePeriod";
const string ConfigNames::Q3TL1retryConnectionPeriod = "Q3TL1retryConnectionPeriod";
const string ConfigNames::Q3TL1loginRequired = "Q3TL1loginRequired";
const string ConfigNames::Q3TL1connectionType = "Q3TL1connectionType";
const string ConfigNames::Q3TL1loggingState = "Q3TL1loggingState";
const string ConfigNames::Q3TL1loggingFile = "Q3TL1loggingFile";
const string ConfigNames::TL1Longitude = "TL1Longitude";
const string ConfigNames::TL1Latitude = "TL1Latitude";
const string ConfigNames::Q3TL1elementType = "Q3TL1elementType";
const string ConfigNames::Q3TL1preConnectScript = "Q3TL1preConnectScript";
const string ConfigNames::Q3TL1postConnectScript = "Q3TL1postConnectScript";
const string ConfigNames::Q3TL1disconnectScript = "Q3TL1disconnectScript";
const string ConfigNames::Q3TL1lostConnectScript = "Q3TL1lostConnectScript";
const string ConfigNames::Q3TL1parserSyntax = "Q3TL1parserSyntax";
const string ConfigNames::Q3TL1namespaceId = "Q3TL1namespaceId";
const string ConfigNames::TL1LoginRetries = "TL1LoginRetries";

//Configuration used for Table Driven Security
// Kindly Use this Place to add any new keys of Security.cfg

const string ConfigNames::InvokePropertiesOfServerGroupKey = "InvokePropertiesOfServerGroup";
const string ConfigNames::ServerGroupManagementKey = "ServerGroupManagement";

const string ConfigNames::HostManagementKey = "HostManagement";

const string ConfigNames::InvokePropertiesOfRootKey = "InvokePropertiesOfRoot";
const string ConfigNames::ModifyPropertiesOfRootKey = "ModifyPropertiesOfRoot";

const string ConfigNames::InvokePropertiesOfRegionKey = "InvokePropertiesOfRegion";
const string ConfigNames::RegionManagementKey = "RegionManagement";

const string ConfigNames::InvokePropertiesOfSiteKey = "InvokePropertiesOfSite";
const string ConfigNames::SiteManagementKey = "SiteManagement";

const string ConfigNames::InvokePropertiesOfNEKey = "InvokePropertiesOfNE";
const string ConfigNames::NEManagementKey = "NEManagement";

const string ConfigNames::InvokeResynchronizationKey = "InvokeResynchronization";
const string ConfigNames::CancelResynchronizationKey = "CancelResynchronization";
const string ConfigNames::ScopedResynchronizationKey = "ScopedResynchronization";

const string ConfigNames::AlarmReportGenerationKey = "AlarmReportGeneration";
const string ConfigNames::NEReportGenerationKey = "NEReportGeneration";


const string ConfigNames::AlarmAcknowledgementKey = "AlarmAcknowledgement";
const string ConfigNames::AlarmClearKey = "AlarmClear";
const string ConfigNames::AlarmResynchronizationKey = "AlarmResynchronization";
const string ConfigNames::ClearStandingAlarmKey = "ClearStandingAlarm";

//For GetUsers,Adduser,DeleteUser,ModifyUserProfile etc will be refered with EMSUserManagement
const string ConfigNames::EMSUserManagementKey = "EMSUserManagement";
//For Assign GetPartitions, GetNonAdminUsers will be refered with EMSPartitionManagement
const string ConfigNames::EMSPartitionManagementKey = "EMSPartitionManagement";
//For GetSessions,TerminateSession,SetMaintenanceMode will be refered with EMSSessionManagement
const string ConfigNames::EMSSessionManagementKey = "EMSSessionManagement";
// For Admin broadcasting
const string ConfigNames::BroadcastMessageKey = "BroadcastMessage";


const string ConfigNames::UpdateAccountPolicyKey = "UpdateAccountPolicy";
const string ConfigNames::SetAdvisoryWarningMessageKey = "SetAdvisoryWarningMessage";
const string ConfigNames::InvokeISMwindowKey = "InvokeISMwindow";






////////////////////// End Of Table Driven Security Keys//////////



// Configuration used for NativeTL1 support
const string ConfigNames::TL1Uid = "TL1Uid";
const string ConfigNames::TL1Tid = "TL1Tid";
const string ConfigNames::TL1AgentHostName = "TL1AgentHostName";
const string ConfigNames::TL1AgentPort = "TL1AgentPort";
const string ConfigNames::TL1Password = "TL1Password";
const string ConfigNames::TL1PasswordEncrypted = "TL1PasswordEncrypted";
const string ConfigNames::CMTL1Autodiscovery = "CMTL1Autodiscovery";
const string ConfigNames::EventTypeMapFile = "EventTypeMap";
const string ConfigNames::TL1ResponseFile = "TL1ResponseFile";
const string ConfigNames::DefaultDbChgSyntax = "DefaultDbChgSyntax";
const string ConfigNames::DefaultReptEvtSyntax = "DefaultReptEvtSyntax";
const string ConfigNames::TL1SessionKeepCmd = "TL1SessionKeepCmd";
const string ConfigNames::RetrieveHeaderInterval = "RetrieveHeaderInterval";
const string ConfigNames::TL1SetupConWaitTimeOut = "TL1SetupConWaitTimeOut";
const string ConfigNames::TL1ConnectionRetry = "TL1ConnectionRetry";
const string ConfigNames::TL1DefaultAlmSyntax = "DefaultAlmSyntax";
const string ConfigNames::TL1MaxLogSize = "MaxTL1LogSize";
const string ConfigNames::TL1LogWrapAround = "TL1LogWrapAround";
const string ConfigNames::TL1MaxNumberOfFiles = "TL1MaxNumberOfFiles";
const string ConfigNames::TL1LogDeleteOnExit = "DeleteOnExit";
const string ConfigNames::AMSUHCSimulatorSupport = "AMSUHCSimulatorSupport";

// Configuration used for Proxy Server
const string ConfigNames::ProxyServerExecutableNameKey = "ProxyServerExecutableName";
const string ConfigNames::RetryPortCountKey = "RetryPortCountKey";
const string ConfigNames::OffsetPortSectionKey = "OffsetPortSection";
const string ConfigNames::NegotiateTimeOutKey = "NegotiateTimeOut";
const string ConfigNames::NegotiateRetryKey = "NegotiateRetry";

const string ConfigNames::LpmTL1SGVersionListKey = "LpmTL1SGVersionList";

const string ConfigNames::PrecisionProfilingEnabledKey = "PrecisionProfilingEnabled";
const string ConfigNames::SeverityQueryTimeoutKey = "SeverityQueryTimeout";

const string ConfigNames::ResolveNameFromNamingRetryKey = "ResolveNameFromNamingRetry";
const string ConfigNames::ResolveNameFromNamingRetryDelayKey = "ResolveNameFromNamingRetryDelay" ;

const string ConfigNames::DefaultBroadCastFlagNameKey = "DefaultBroadCastFlag";

//CPE Trap 
const string ConfigNames::CPE_NE = "NE";
const string ConfigNames::CPE_CPEMODULE = "CPEModule";
const string ConfigNames::CPE_MAC = "CPEMAC";
const string ConfigNames::CPE_LAG = "CPELAG";
const string ConfigNames::CPE_MODULE_LED = "MODULE_LED";
const string ConfigNames::CPE_7305MODULE = "7305MODULE";
const string ConfigNames::CPE_MGMTPORT = "MGMTPORT";
const string ConfigNames::CPE_MGMTVLANPORT = "MGMTVLANPORT";
const string ConfigNames::CPE_MST = "MST";
const string ConfigNames::CPE_RST = "RST";
const string ConfigNames::CPE_CFM = "CFM";
const string ConfigNames::CPE_CPESHELF = "CPESHELF";
const string ConfigNames::CPE_LOW_CPESHELF = "CPEShelf";
const string ConfigNames::CPE_CPESWITCH = "CPESWITCH";
const string ConfigNames::CPE_CMCC = "CMCC";
const string ConfigNames::CPE_PSU = "PSU";
const string ConfigNames::CPE_CPEFAN = "CPEFAN";
const string ConfigNames::CPE_CPEXFP = "CPEXFP";
const string ConfigNames::CPE_CPESFP = "CPESFP";
const string ConfigNames::CPE_CAP_CPEMODULE = "CPEMODULE";
const string ConfigNames::CPE_SNTP = "SNTP";
const string ConfigNames::CPE_ENV = "CPEENV";
const string ConfigNames::CPE_EXT = "CPEEXT";
const string ConfigNames::CPE_VLAN = "CPEVLAN";
const string ConfigNames::CPE_RVT = "CPERVT";
const string ConfigNames::CPE_SYNC = "CPESYNC";
//O9400R trap
const string ConfigNames::O9400R = "O9400R";

const string ConfigNames::SNMP_CONTEXT_ENGINEID = "SNMPContextEngineId";
const string ConfigNames::SNMP_USERID = "SNMPUserId";
const string ConfigNames::SNMP_AUTH_PROTOCOL = "SNMPAuthProtocol";
const string ConfigNames::SNMP_AUTH_PASSWORD = "SNMPAuthPassword";
const string ConfigNames::SNMP_PRIV_PROTOCOL = "SNMPPrivProtocol";
const string ConfigNames::SNMP_PRIV_PASSWORD = "SNMPPrivPassword";
const string ConfigNames::SNMP_AGENT_IP = "SNMPAgentIP";
const string ConfigNames::SNMP_AGENT_PORT = "SNMPAgentPort";
const string ConfigNames::SNMP_IP_ADDR = "SNMPIPAddress";

const string ConfigNames::DPM_SNMPAGENT_USERNAME = "DPMSNMPAgentUsername";
const string ConfigNames::DPM_SNMPAGENT_AUTHPASSWORD = "DPMSNMPAgentAuthPassword";
const string ConfigNames::DPM_SNMPAGENT_PRIVPASSWORD = "DPMSNMPAgentPrivPassword";
const string ConfigNames::DPM_SNMPAGENT_AUTHPROTOCOL = "DPMSNMPAgentAuthProtocol";
const string ConfigNames::DPM_SNMPAGENT_PRIVPROTOCOL = "DPMSNMPAgentPrivProtocol";
const string ConfigNames::CUR_DPM_SNMPAGENT_USERNAME = "CurDPMSNMPAgentUsername";
const string ConfigNames::CUR_DPM_SNMPAGENT_AUTHPASSWORD = "CurDPMSNMPAgentAuthPassword";
const string ConfigNames::CUR_DPM_SNMPAGENT_PRIVPASSWORD = "CurDPMSNMPAgentPrivPassword";
const string ConfigNames::CUR_DPM_SNMPAGENT_AUTHPROTOCOL = "CurDPMSNMPAgentAuthProtocol";
const string ConfigNames::CUR_DPM_SNMPAGENT_PRIVPROTOCOL = "CurDPMSNMPAgentPrivProtocol";

//add for public EON feature
const string ConfigNames::PUBLIC_EON_ENABLE = "PUBLIC_EON_ENABLE";
