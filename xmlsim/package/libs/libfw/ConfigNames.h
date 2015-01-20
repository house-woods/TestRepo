/**********************************************************************************************
* COPYRIGHT 1999 - 2004 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: ConfigNames
* Type: H
* Originator: $Author:   tmarsh  $
* File: $Workfile:   ConfigNames.h  $
* Version: $Revision:   1.7  $
* Description:
*  Contain Names used in configuration files
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/ConfigNames.hv_  $
*
*    Rev 1.7   Oct 19 1999 17:10:52   tmarsh
* modified
*
*    Rev 1.6   Oct 19 1999 16:33:02   cballard
* Synchronized with 1.4
*
*    Rev 1.5   Oct 18 1999 16:49:14   vlawande
* Software ne
**********************************************************************************************/

#ifndef _CONFIGNAMES_H_
#define _CONFIGNAMES_H_

#include <string>
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

namespace EMS_FW
{

class ConfigNames
{
public:

    /**********************************************************
     *
     *  Constants defined which can be used by any servers...
     *
     **********************************************************/
    static const string True;
    static const string False;
    static const string ReaderMode; // Indicates that the server is running in a reader mode.
    static const string WriterMode; // Indicates that the server is running in a writer mode.
    static const string Null;
    static const string Btm;  // Indicates a Btm ne type
    static const string Ots;  // Indicates a ots ne type
    static const string Occs;  // Indicates the 6700 ems type
    static const string An2100;  // Indicates an2100 ne type
    static const string SoftwareNe; // Indicates software ne without agent
    static const string LpmAlarmNeId;
    static const string SoftwareAlarmNeId;
    static const string SoftwareAlarmNeName;

    /**********************************************************
     *
     *  Commands defined which can be used by any servers...
     *
     **********************************************************/
    static const string CommandStartLogging;
    static const string CommandStopLogging;
    static const string CommandSetTraceLevel;
    static const string EnableSyncFromLog;
    static const string CommandChangeNePassword;

    /**********************************************************
     *
     *  Configuration sent to all the EMS servers during startup...
     *
     **********************************************************/ 
    // Names common to all servers....
    static const string IdKey;        // Each resource including server has unique id
    //TLIaa54672   Added by jhu
    static const string ServerGroupIdKey;     // Each server group has unique id
    static const string ServerGroupNameKey;     // Each server group has unique name

    static const string ColdStartKey;      // If true, global resources shared by all servers instances should be created.
    static const string ServerColdStartKey;     // If true, Server should be cold started.
    static const string SetSMWarmStartKey;                  // When specified true, SM will start in warmStart mode.
    static const string ServerNameKey;      // Name of the server for orbix daemon.
    static const string SecurityChannelNameKey;    // For receiving client login & logout events.
    static const string SysmonStateChangeChannelNameKey; // To send state change events to sysmon.
    static const string SysmonRecoveryKey;   // STart Sysmon inRecovery mode.

    //TLIaa54672  Added by jhu
    static const string SgmStateChangeChannelNameKey;  // To send state change events to sgm.

    static const string FmToClientChannelNameKey;  // To send state change events to sgm.
    static const string CmToClientChannelNameKey;  // To send state change events to sgm.
    static const string CmToClientAlarmBubbleUpChannelNameKey;  // To send alarm bubble up change events to sgm.

    static const string EmsGlobalChannelNameKey;   // To broadcast to all servers/clients.

    static const string FcpStateChangeChannelNameKey;  // To send state change events to node manager.
    static const string SysmonInterfaceNameKey;    // Sysmon interface name...
    static const string NeTypeKey;       // Type of ne, e.g btm, ots etc...
    static const string NeVersionKey;      // Network Element version like 1.1 1.2 etc.
    static const string NeNoVersion;      // Constant that specify no version
    static const string TraceLevelKey;      // Trace level for logging...
    static const string TraceListKey;      // Trace List, true or false
    static const string GlobalHeapCheckKey;     // GlobalHeapCheck if true enable heapCheck everytime when trace message is printed
    static const string WriteToExceptionFileKey;   // If set to true the trace messages from dumpTraceList will be logged in exceptions.log file.
    static const string LogAgentNotificationsKey;   // If set to true all agent responses will be logged into the agent.log
    static const string CraftStationModeKey;    // True indicates ems is runiing in craftstation maode
    static const string MaxLexicalTransactionRetriesKey; // Max Number of Automatic Retries Within Lexical Transaction
    static const string NotificationMaxRetriesQosKey;  // Maximum number of times that a proxy push supplier calls push() on its consumer before it gives up.
    static const string NotificationRequestTimeoutQosKey; // Time elapsed between attempts by a proxy push supplier to call push() on its consumer
    static const string NotificationRetryTimeoutQosKey;  // Time elapsed between attempts by a proxy push supplier to call push()
    static const string NotificationRetryMultiplierQosKey;  // Number by which the current value of RetryTimeout os multiplied to determine the next RetryTimeout
    static const string NotificationMaxRetryTimeoutQosKey; // Max. value of retryTimeout.
    static const string NamingServiceHostNameKey;   //Host name that Naming Service is running on
    static const string NamingServicePortNumberKey;   //The port number of the Naming Service
    static const string STORBConfigFileNameKey;    // Single threaded orb configuration
    static const string MTORBConfigFileNameKey;    // Multi threaded orb configuration
    static const string SMORBConfigFileNameKey;    // SM specific ORB configuration
    static const string OrbConfigParametersKey;  //additional EMS server orb configurations

    //Sysmon server specific names...
    static const string NotifyLocationKey;   // Location of the Notification Service executable
    static const string NotifyConfigParametersKey;  //Notification Service Configurations
    static const string NotifyDebugLevelKey;  // Notification Service debug level

    // Am server specific names ...
    static const string SNMPEnableKey;                   // Enable the SNMP
    static const string EnableSecInterfaceKey;              // Enable the secondary interface
    static const string SecInterfaceIpAddrKey;              // Secondary interface IP address
    static const string ProxyTimeOutKey;                    // Ping time for the Proxy
    static const string SysmonTimeOutKey;                   // Time out for Sysmon/SGM calls
    static const string EnableProxyConsoleKey;  // Show the Proxy Console
    static const string ProxyExecDirKey;   // Shows the Proxy execute directory
    static const string EnableProxyLogKey;   // Enable logging of Proxy Console messages.
    static const string RestoreActiveSessions;  // Should SM retore sessions while warm starting, false by default
    // Set to true only when SM crashes and is re-started.
    static const string RestoreActiveSessionTimeOutKey;
    static const string startProxyCmdKey;   // Start another console for proxy output
    static const string CMIPListenPortNameKey;  // The port number on which the proxy agent listens for some CMIP requests.
    static const string ShutDownTimeOutKey;   // TimeOut used by SGM and SysMon at the time of Shuting down Servers and Removing NetworkElement
    static const string MaxProxyQueKey;    // Maximum number of queued alarms


    // Cm server specific names ...
    static const string ModeKey;       // To tell CM to start as Reader or Writer.
    static const string ClientMgrNameKey;     // To add and remove interested moi list.
    static const string CmipMoiHeaderKey;     // Name used to extract pure moi from MOI string.
    static const string MapFileNameKey;      // Map file name for the file containing relations between agent names and EMS names
    static const string AddMoiHeaderKey;     // String that is inserted in from of Managed Object Instance because it is required by the agent
    static const string FilterKey;       // Filter used to contact agent.
    static const string OD_FilterKey;      // On Demand Discovery Filter used to contact agent.
    static const string DllNameKey;       // DLL name that client may use.
    static const string NmDllNameKey;      // Nm DLL name that client may use.
    static const string FmToCmBubbleupChannelNameKey;  // Channel used to send mo buubleup indication from fm to cm
    static const string AutodiscoverDelayKey;    // Delay in msec to allow autodiscover to start before return the call
    static const string AutodiscoverAttemptsKey;            // Number of failed NE warmstart attempts before the NE's databases are erased and coldstarted
    static const string ExecuteCMAutodiscoverKey;   // If false autodiscover want be done after add Ne.
    static const string CMAutodiscoveryMethodKey;   // Method used for autodiscover CM Layered or whole tree
    static const string CMLayeredAutodiscovery;
    static const string CMDirectAutodiscovery;
    static const string CMLeveledAutodiscovery;
    static const string CMAutodiscoveryObjectsPerTxnKey; // specifies how many objects will be processed per transaction.
    static const string CMAutodiscoveryEnableAttributesCheckKey; // If true attributes that are coming from the agent will be checked against
    // the attributes already stored in the database. If they are the same they want be updated.
    static const string CMMultiThreadedAutodiscoveryKey; // if true autodiscovery is running in multithreaded mode
    static const string HandleServerErrorsKey;    // If set to true, then sysmon will restart the server group for aborted server.
    static const string RestartFailedNetworkElementsKey; // If set to true, then sysmon will restart failed network elements

    // Nm server specific names ...
    static const string NodeDbPathKey;      // Full path to the nodemgr database.
    static const string NodeManagerInterfaceNameKey;  // Name of the nodeMgr interface name in naming service.
    static const string CmToNmBubbleupChannelNameKey;  // Channel used to send mo buubleup indication from cm to nm
    static const string DefaultNeLicenseKey;                // License key used when the NM does a coldstart.
    static const string NMOperationTimeoutKey;    // Timeout for CORBA operations from NM to CM(getAttributes,getChildren, getNodeSummary)
    static const string NMCreateNodeTimeoutKey;    // Timeout for CORBA operations from NM to sysmon to create NE
    static const string NMToCMExecuteActionTimeoutKey;  // Timeout for ExecuteAction call from NM to CM

    // Report Manager specific names
    static const string ReportManagerInterfaceNameKey;  // Name of the rptMgr interface name in naming service.


    // SM server specific names...
    static const string SecurityDbPathKey;     // Full path for security database.
    static const string SecurityADbPathKey;     // Full path for security adb file.
    static const string SecurityClientInterfaceNameKey;  // Security client interface name in naming service.
    static const string SecurityAdminInterfaceNameKey;  // Security admin interface name in naming service
    static const string SecurityServerInterfaceNameKey;  // Security server interface name in naming service
    static const string SecurityProfileInterfaceNameKey;  // Security Profile interface name in naming service
    static const string MultiLogonAllowedKey;    // Allow mutiple users logon with the same user id
    static const string EnableClientActiveCheckKey;   // SM checks if the client session is still active. If not,
    // terminate the session
    static const string ClientPollingPeriodKey;    // SM ping client polling period
    static const string MaxNumberOfClientsKey;    // the max number of ems clients
    static const string SmToClientRestoreSessionMaxRetryKey; // Number of retries the ORB should attempt a socket connection when
    static const string SmToClientActiveCheckTimeoutKey; //  Time out for pinging to Clients from SM
    static const string sleepTimeForTerminateSessionKey; // Sleep between Logoff Notification and terminate session
    // (Just to make sure All servers got the Logoff Notification.)
    static const string TimeDeltaThreshold;               // Maximum gap (in seconds) between client/server timestamp.
    static const string DestroyChannelTimeOutKey;         // Timeout to be passed to DestroyChannel call


    // Fm specific names...
    static const string TranslatorFileNameKey;    // Full path for the translator file name
    static const string FmlToFmrChannelNameKey;    // Channel between fml & fmr
    static const string FmwInterfaceNameKey;    // Fmw name in naming service.
    static const string FmrInterfaceNameKey;    // Fmr name in naming service
    static const string FmlInterfaceNameKey;    // Fml name in naming service
    static const string BubbleupInterfaceNameKey;   // Bubbleup name in naming service.
    static const string FMmaxNoOfAlarmsPerTxnKey;   // Maximum number of alarms written to the database at a time.
    static const string ArchiveDbPathKey;     // Name of the archive database directory.
    static const string PriArchiveDbPathKey;    // Name of the primary archive database directory.
    static const string SecArchiveDbPathKey;    // Name of the secondary archive database directory.
    static const string AutoAgingPeriodKey;     // Period in msec to start auto archiving aged alarms.
    static const string AgingKeepTimeKey;     // Time in msec to for cleared alarms being archived.
    static const string AgingEnabledKey;     // Detemine if the aging is enabled
    static const string ArchiveStartTimeKey;    // The configurable Time at which archiving should start initially
    static const string ArchiveTimeSpanKey;     // The time interval within which all NE's should start archiving
    static const string PriArchivePrefixKey;    // prefix for the primary archived alarm DB name
    static const string SecArchivePrefixKey;    // prefix for the secondary archived alarm DB name
    static const string ArchiveBackupIntervalKey;   // the interval,of auto aging period, at
    // which the primary archived alarm DB is copied
    // to the secondary archived alarm DB
    static const string DeleteBatchFileKey;     // path to and name of the delete batch file
    static const string MoveBatchFileKey;     // path to and name of the move batch file

    //////////////////////////////////////////////////////
    // for SW Upgrade client support ...
    static const string NeVersionSpecificKey;    //
    static const string SgVersionSpecificKey;    //
    // ////////////////////////////////////////////////////////
    // for SW Upgrade sysmon support
    static const string VersionKey; // Version of all kinds of Resource
    //static const string ServerConfigKey; // Version of all kinds of Resource
    //static const string NeConfigKey; // Version of all kinds of Resource
    //static const string SequenceKey; // Version of all kinds of Resource
    //static const string SGVersionResourceGroup; // Version of all kinds of Resource

    //////////////////////////////////////////////////////////////////


    //LPM specific names...
    static const string LpmUserNameKey;      // LPM user name
    static const string LpmPasswordKey;      // LPM password
    static const string LpmCmdSleepTimeKey;     // LPM command sleep time
    static const string LpmAutoDiscSleepTimeKey;   // LPM auto discovery sleep time
    static const string LpmMoLiteDatabaseNameKey;   // LPM molite database name
    static const string LpmDBChannelNameKey;    // LPM database channel name
    static const string LpmInstanceNameKey;     // LPM instance name
    static const string LpmClientMgrNameKey;    // LPM client manager name
    static const string LpmServerNameKey;     // LPM server name
    static const string LpmLpmMgrNameKey;     // LPM manager name

    // RingManager specific Names
    static const string RMInstanceNameKey;     // RM instance name
    static const string RMMoLiteDatabaseNameKey;    // RM molite database name
    static const string RMServerNameKey;     // LPM server name


    // NBI TL1 specific name
    static const string NbiTL1ChannelNameKey;                               // NBI channel name
    static const string NbiMetaModelTypeKey;                                // NBI MetaModel type
    static const string NbiAdapterTypeKey;                                  // NBI mediation adapter type name
    static const string NbiAdapterIdKey;                                    // NBI mediation adapter id name
    static const string NbiAdapterPortNumKey;                               // NBI TL1 mediation adapter port number name
    static const string NbiAdapterMaxConnKey;                               // NBI TL1 mediation adapter maximum number of connection
    static const string NbiTidKey;                                          // NBI TL1 mediation adapter tid name
    static const string NbiTL1CmdGrammarFileKey;                            // NBI TL1 Command Grammar file for parser
    static const string ClientServiceName;                                  // Name of the clientService object in SM.
    static const string NbiTL1FeatureEnabledKey;                            // Verifies if the NBI TL1 feature is enabled
    static const string NbiTL1MapFileKey;                                   // Name of the file which contains mapping between EMS and TL1 parameters.

    static const string NbiTL1MaxIdleTimeKey;                               // ...
    static const string NbiTL1AlarmTextMessage;                             // ...
    static const string NbiEnableAlarmMessaging;                            // ...

    static const string NbiKeepAliveIntervalKey;    // Key name of the NBI Keep Alive parameter

    /**********************************************************
     *
     *  Configuration sent to all the EMS servers while adding
     *  a new Network element
     *
     **********************************************************/ 
    // Names Common to all servers...
    static const string ServerSessionId;
    static const string NeColdStartKey;   // If true, Ne should create persistent resources in initialize.
    static const string NeColdStopKey;   // If true, Ne should remove persistent resources in finalize.
    static const string NeIdKey;    // Unique id for the ne assigned by nodeMgr.
    static const string NeNameKey;    // Name of the network element.
    static const string AgentHostKey;   // IP number of the machine running proxy agent.
    static const string AgentPortKey;   // Port number of the proxy agent.
    static const string ProtocolKey;   // Protocol used to talk to ne like CMS, TL1 etc.
    static const string MoLiteDatabaseNameKey; // Full path for the  MoLite database
    static const string MoDatabaseNameKey;  // Full path for the  Mo database
    static const string CraftStationManagedNeKey; // True indicates this ne is currently managed by craftstaion.
    static const string EfdNameKey;    // Efd Object Name used in init socket
    static const string EmsIdKey;    // Ems id key.
    static const string EmsTypeKey;
    static const string EmsVersionKey;
    static const string AgentCallTimeOutKey; // Specifies timeout for the operations against the agent
    static const string ServerToProxyTimeOutKey; // Specifies timeout between server and proxy agent.
    static const string ProxyConnectTimeOutKey; // Specifies timeout between server and proxy agent.
    static const string MigrationInProgressKey; // Indicates servers that migtration is in progress for the ne
    static const string MaxAddNERetriesKey;    // Max number of times to add NE in SGM
    // Cm server specific names
    static const string RootMoiKey;    // Root moi used to know from where to start autodiscovery.
    static const string RootMocKey;    // Root Object Managed Object Class.
    static const string AgentParamKey;   // Parameters needed to specify connection to the agent.
    static const string CmNodeInterfaceNameKey; // CM's Nodei interface name in naming service.
    static const string NeParentSiteIdKey;  // Id of the site which contains ne.
    static const string CmEfdNameKey;   // CM Efd Object Name used in initsocket
    static const string NeSubTypeKey;   // For Supporting NE Sub Types. Added by Abhishek

    // For Network autodiscovery
    static const string EnableNetworkAutodiscoveryKey; // For auto network discovery
    static const string DBMonitorIntervalKey;
    static const string QueueMonitorIntervalKey;
    static const string DefaultNEPasswordKey;
    static const string DefaultNEUserIdKey;

    // Fm server specific names...
    static const string SocketTypeKey;   // Type of socket.
    static const string AlarmDatabaseNameKey; // Full path for the  alarm database
    static const string BubbleUpDatabaseNameKey; // Full path for the  bubbleup database
    static const string FmEfdNameKey;   // FM Efd Object Name used in initsocket
    static const string ExecuteFMAutodiscoverKey; // Default is true, meaning perform auto-discovery during add NE.


    /**********************************************************
     *
     *  Configuration used by sysmon....
     *
     **********************************************************/
    static const string DbPathKey;    // Name of the databse directory.
    static const string ExecutableDirectoryKey;  // Name of the directory where all the executable resides.
    static const string LogDirectoryKey;   // Name of the directory where all the logs resides.
    static const string LogSizeKey;    // Maximum size of the log file.
    //TLIaa54672  Added by jhu
    static const string SgmExecutableNameKey;  // Name of Sgm Executable.
    static const string AmExecutableNameKey;  // Name of Am Executable.
    static const string SmExecutableNameKey;  // Name of SM Executable.
    static const string NmExecutableNameKey;  // Name of NM Executable.
    static const string NbiExecutableNameKey;  // Name of NBI Executable.
    static const string RingMgrExecutableNameKey;  // Name of RingMgr Executable.
    static const string LpmrExecutableNameKey;  // Name of Lpm reader Executable.
    static const string LpmwExecutableNameKey;  // Name of Lpm writer Executable.
    static const string CmExecutableNameKey;  // Name of CMW Executable.
    static const string FmrExecutableNameKey;  // Name of Fmr  Executable.
    static const string FmwExecutableNameKey;  // Name of Fmw Executable.
    static const string FmlExecutableNameKey;  // Name of Fml Executable.
    static const string BubbleupExecutableNameKey;  // Name of bubbleup Executable.
    static const string ServerConfigFileNameKey;  // Name of server template config file.
    static const string NeConfigFileNameKey;  // Name of ne template config file.
    static const string SecurityConfigFileNameKey;  //Name of the Security config file.
    static const string VersionConfigFileNameKey;  // Name of version config file using to check compatibilty between server and client
    static const string LeastClientVersionKey;  // Least client version that can work with this server
    static const string LatestClientVersionKey;  // Latest client version that can work with this server
    static const string CurrentServerVersionKey;  // current server version
    static const string StartLpmAlarmNeKey;
    static const string StartSoftwareAlarmNeKey;
    static const string MaximumServerGroupsPerHostKey;
    static const string MaximumNesPerServerGroupKey;
    static const string SysmonDbNameKey;            // Name of sysmon database file (e.g., sysmon.db)
    static const string ServerPollingIntervalKey; // time in minutes after which sysmon polls servers
    // for detecting hung server
    static const string ServerPingTimeoutKey; // time in seconds after which ping timeouts and server
    static const string ServerOperationTimeoutKey; // time in minutes after which server operations will timeout
    static const string AddNEOperationTimeoutKey; // time in minutes after which AddNE operations will timeout

    static const string PrimaryHostKey;
    static const string SysmonInitializingKey;
    static const string RMLicenseCodeKey;
    static const string LPMLicenseCodeKey;
    static const string LPMLicenseGrantedKey;
    static const string RMLicenseGrantedKey;

    //TLIaa54672   Added by jhu
    static const string InfrastructureServerKey; // If this server is infrastructure server or server group server?

    static const string InfraServerRecoveryRetryKey; // If How many times SysMon should try to Recover Infrastructure Server
    // If it fails to Recover.
    static const string ParentServerNameKey;  // Sysmon/SGM interface name. Used by HM to notify server crash.


    static const string HMOperationTimeOutKey;  // Timeout to be used to call HM to Launch Or Terminate process
    static const string HMLaunchRetryKey;   // No of times SysMon and SGM will try to launch Process using HM if fails to launch
    static const string SysmonHMInitRetryMaxKey; // Number of times to retry initialization that depends on HM being up
    // (e.g. number of times to setRootContext (because HM starts the ORB),
    //  number of times to ping HM, etc.).
    static const string SysmonHMInitRetryDelayKey; // Number of seconds between retrying HM dependent initialization tasks.



    // added by lzou for version support
    // in Sysmon.cfg
    static const string VersionListKey; // Version List that the EMS could support
    static const string ServerConfigListKey; // the server configuration file name list for different version server group
    static const string NeConfigListKey; // the ne configuration file name list for different version
    static const string DelimitorKey; // the delimitor for seperating the token in string list of configuration files

    // added by Hammad for OTS Enhanced Security
    static const string SGVersionsSupportingEnhancedSecurityKey;

    // NeVersionList+version index to used for every version of Server Group in sysmon configuration files
    static const string NeVersionListKey; // Ne Version List for a version of SG
    static const string Adapter1;  // the SG version that ServerVersionAdapter1_1 can support
    static const string Adapter2; // the SG version that ServerVersionAdapter can support
    static const string Adapter; // which adapter should be used by this SG version in persistent SG version object

    static const string SWAlarmSGVersionKey; // Server Group version to be used to bring up the SW AlarmNE server group
    static const string SWAlarmForNBIKey;  // Flag used to check if software alarms need to be sent to NBI or not

    static const string BaseJarFileNameKey;  //Jar file name for this server group verison servers
    static const string MigrationConfigFileNameKey;

    // NE Migration Support
    static const string AutoMigrationKey; // Need AuotMigration or not?


    //start threadpool support for sysmon warmstart
    static const string NumofHostThreadatWarmStartKey;
    static const string NumofSgThreadatWarmStartKey;
    static const string SysmonWaitTimeForHMToUpKey;
    static const string SysmonSleepTimeForObjectStoreToUpKey;
    static const string SysmonRetryForObjectStoreToUpKey;


    //end threadpoolsupport

    static const string NeTypeListKey;
    static const string NeProductTypeKey;

    //static const string PrimaryHostKey;

    // identifies the Primary Host name

    //mod for Proxy Agent
    static const string AddNeKey;
    static const string AgentRootDirKey;
    static const string AgentNameKey;
    static const string X723applicationProcessTitleKey;
    static const string AgentDatabaseBatchFilekey;

    static const string OSINSAPKey;
    static const string OSIStackFlagKey;



    static const string ProxyAgentExeKey;

    // This for sending the calling AP ttile as a Distinguished Name
    static const string callingApTitleDnKey;


    static const string pSelectorKey;
    static const string sSelectorKey;
    static const string tSelectorKey;
    static const string nSelectorKey;

    static const string PHexadecimalKey;
    static const string SHexadecimalKey;
    static const string THexadecimalKey;


    static const string PartitionIdkey;

    static const string DSETAgentOidKey;
    static const string ProxyOidKey;
    static const string ApQualifierIdKey;
    static const string RemoteAeTitleKey;

    static const string CmEfdFilterKey;
    static const string FmEfdFilterKey;

    // Added by Shangping for System defensiveness
    static const string PasswordEnabledKey;
    static const string PasswordKey;

    // Agent reconnect delay while creating asssociation object in Proxy
    static const string AgentReConnectDelayKey;


    static const string qosflagKey;
    static const string qosreliabilityKey;
    static const string qossversionKey;
    static const string qosextendedKey;
    static const string qosmaxtimeKey;

    static const string DirectReferenceKey;


    // To configure the topology root name
    static const string TopologyRootNameKey;

    // Contains the unique identifier to be used by the Agent Manager to invoke the
    // dynamic script for mulitple proxies per Server Group
    static const string ProxyAgentCount;

    // smat specific names ...
    static const string SMATDbPathKey;      // Full path to the smat database.
    static const string SMATEMSDbNameKey;


    // SMAT Database Archiving
    static const string SMATArchiveEnabledKey;
    static const string SMATAgingKeepTimeKey;
    static const string PrimarySMATArchivePathKey;
    static const string PrimarySMATArchivePrefixKey;
    static const string SMATPriArchiveIntervalsKey;
    static const string SecondarySMATArchivePathKey;
    static const string SecondarySMATArchivePrefixKey;
    static const string OffloadedSMATArchivePathKey;
    static const string OffloadedSMATArchivePrefixKey;
    static const string SMATSecArchiveIntervalsKey;
    static const string SMATOffloadBatchFileNameKey;
    static const string SMATNEDbNamePrefixKey;
    static const string SMATAutoAgingIntervalKey;

    // HM specific names ...
    static const string ProcessDataFile;
    static const string HmTimeout;
    static const string CreateNewConsoleForServer;
    static const string ServerRootContext;
    static const string HmThreadCount;
    static const string HmOrbProcessList;
    static const string OrbixDomainIsFileBased;
    static const string isPrimaryHost;
    static const string ThirdPartyServerCount;
    static const string ThirdPartyServerLine;
    static const string ObjectStoreServerName;
    static const string ObjectStoreCacheMgrName;

    // Number of times to retry initialization of ThirdParty servers (e.g. number
    // of times to setRootContext).
    static const string HMInitRetryMaxKey;

    // Number of seconds between retrying HM dependent initialization tasks.
    static const string HMInitRetryDelayKey;

    // Added by hwang
    static const string GetHMServerRefTimeout;

    static const string AgentVersionCheckKey;

    static const string LocAgentVersionCheckKey;

    // Configuration used for Switch from EM to ISM system
    static const string ISMSupportedKey;
    // Configuration used for Switch from On Demand Discovery to Full Discovery
    static const string OnDemandSupportedKey;

    //Configuration used for DSS SNMP Network Element
    static const string GetSetCommunityString;
    static const string TrapCommunityString;
    static const string SnmpResponseTimeout;
    static const string ConnectivityPollFrequency;
    static const string SynchronizationFrequency;
    static const string RetriesOnCommunicationFailure;
    static const string ProxyManagementProtocol;
    static const string ProxySupportedMIBS;
    static const string MaxVarBindsPerPDU;
    static const string TrapPortAddress;
    static const string SNMPSupport;
    static const string SNMPSupportPort;
    static const string PrimaryHostIPAddress;
    static const string SNMPAgentRootDirKey;



    //Configuration used for AMS Network Element
    static const string M3100alarmStatus;
    static const string UserId;
    static const string TID;
    static const string Password;
    static const string M3100vendorName;
    static const string M3100networkName;
    static const string Q3TL1keepAlivePeriod;
    static const string Q3TL1retryConnectionPeriod;
    static const string Q3TL1loginRequired;
    static const string Q3TL1connectionType;
    static const string Q3TL1loggingState;
    static const string Q3TL1loggingFile;
    static const string TL1Longitude;
    static const string TL1Latitude;
    static const string Q3TL1elementType;
    static const string Q3TL1preConnectScript;
    static const string Q3TL1postConnectScript;
    static const string Q3TL1disconnectScript;
    static const string Q3TL1lostConnectScript;
    static const string Q3TL1parserSyntax;
    static const string Q3TL1namespaceId;
    static const string TL1LoginRetries; //login retries on recovery after loc in am

    /// Security Table...
    //EMS Administration;

    static const string InvokePropertiesOfServerGroupKey;
    static const string ServerGroupManagementKey;

    static const string HostManagementKey;

    static const string InvokePropertiesOfRootKey;
    static const string ModifyPropertiesOfRootKey;

    static const string InvokePropertiesOfRegionKey;
    static const string RegionManagementKey;

    static const string InvokePropertiesOfSiteKey;
    static const string SiteManagementKey;

    static const string InvokePropertiesOfNEKey;
    static const string NEManagementKey;

    //CM
    static const string InvokeResynchronizationKey;
    static const string CancelResynchronizationKey;
    static const string ScopedResynchronizationKey;

    //Report Generation
    static const string AlarmReportGenerationKey;
    static const string NEReportGenerationKey;

    //FM
    static const string AlarmAcknowledgementKey;
    static const string AlarmClearKey;
    static const string AlarmResynchronizationKey;
    static const string ClearStandingAlarmKey;

    //Security Manager
    static const string EMSUserManagementKey;
    static const string EMSPartitionManagementKey;
    static const string EMSSessionManagementKey;
    static const string UpdateAccountPolicyKey;
    static const string SetAdvisoryWarningMessageKey;
    static const string InvokeISMwindowKey;
    static const string BroadcastMessageKey;



    // Configuration used for NativeTL1 support
    static const string TL1Uid;
    static const string TL1Tid;
    static const string TL1AgentHostName;
    static const string TL1AgentPort;
    static const string TL1Password;
    static const string TL1PasswordEncrypted;
    static const string CMTL1Autodiscovery;  // One of CM Autodiscover Method Keys
    static const string EventTypeMapFile;  // EventType map config file name
    static const string TL1ResponseFile;  // TL1 Response config file name
    static const string DefaultDbChgSyntax;  // Syntax Definition for default DbChg Event
    static const string DefaultReptEvtSyntax; // Syntax Definition for default Evt Event
    static const string TL1SessionKeepCmd;  // Cmd to be sent to maintain the TL1 session and check LOC
    static const string RetrieveHeaderInterval; // Interval between two times retry
    static const string TL1SetupConWaitTimeOut; // Time out for each connection setup
    static const string TL1ConnectionRetry;  // Retry Times after failed to setup connection
    static const string TL1DefaultAlmSyntax;     // TL1 Default Alm Message Syntax
    static const string TL1MaxLogSize;           // TL1 Message Max Log size for Proxy Server
    static const string TL1LogWrapAround;        // TL1 Log file Wrap or not
    static const string TL1MaxNumberOfFiles;     // TL1 log file numbers
    static const string TL1LogDeleteOnExit;      // TL1 Log file will be deleted or not
    static const string AMSUHCSimulatorSupport;  // whether we support the AMS UHC NE Simulator

    static const string ProxyServerExecutableNameKey;  // Proxy Server executable name
    static const string RetryPortCountKey;   // Number of port will be scanned
    static const string OffsetPortSectionKey;   // Integer value for offset of port sections
    static const string NegotiateTimeOutKey;  // time out value for negotiation
    static const string NegotiateRetryKey;   // Retry time for negotiation between FM/CM to proxy server

    static const string LpmTL1SGVersionListKey;  // List of SG version support TL1 for OTS
    // added for profiling
    static const string PrecisionProfilingEnabledKey;

    static const string SeverityQueryTimeoutKey;  // Time out for getSeverity corba call

    static const string ResolveNameFromNamingRetryKey; // retry for resolving name from naming service
    static const string ResolveNameFromNamingRetryDelayKey;

    static const string DefaultBroadCastFlagNameKey;     // whether broadcast the notification by default
    //CPE Trap 
    static const string CPE_NE;
    static const string CPE_CPEMODULE;
    static const string CPE_MAC;
    static const string CPE_LAG;
    static const string CPE_MODULE_LED;
    static const string CPE_7305MODULE;
    static const string CPE_MGMTPORT;
    static const string CPE_MGMTVLANPORT;
    static const string CPE_MST;
    static const string CPE_RST;
    static const string CPE_CFM;
    static const string CPE_CPESHELF;
    static const string CPE_LOW_CPESHELF;
    static const string CPE_CPESWITCH;
    static const string CPE_CMCC;
    static const string CPE_PSU;
    static const string CPE_CPEFAN;
    static const string CPE_CPEXFP;
    static const string CPE_CPESFP;
    static const string CPE_CAP_CPEMODULE;
    static const string CPE_SNTP;
    static const string CPE_ENV;
    static const string CPE_EXT;
    static const string CPE_VLAN;
    static const string CPE_RVT;
    static const string CPE_SYNC;
    //O9400R Trap 
    static const string O9400R; 
    // SNMP related configs
    static const string SNMP_CONTEXT_ENGINEID;
    static const string SNMP_USERID;
    static const string SNMP_AUTH_PROTOCOL;
    static const string SNMP_AUTH_PASSWORD;
    static const string SNMP_PRIV_PROTOCOL;
    static const string SNMP_PRIV_PASSWORD;
    static const string SNMP_AGENT_IP;
    static const string SNMP_AGENT_PORT;
    static const string SNMP_IP_ADDR;

    static const string DPM_SNMPAGENT_USERNAME;
    static const string DPM_SNMPAGENT_AUTHPASSWORD;
    static const string DPM_SNMPAGENT_PRIVPASSWORD;
    static const string DPM_SNMPAGENT_AUTHPROTOCOL;
    static const string DPM_SNMPAGENT_PRIVPROTOCOL;
    static const string CUR_DPM_SNMPAGENT_USERNAME;
    static const string CUR_DPM_SNMPAGENT_AUTHPASSWORD;
    static const string CUR_DPM_SNMPAGENT_PRIVPASSWORD;
    static const string CUR_DPM_SNMPAGENT_AUTHPROTOCOL;
    static const string CUR_DPM_SNMPAGENT_PRIVPROTOCOL;
    //Add for Public EON feature
    static const string PUBLIC_EON_ENABLE;
    
};

};

#endif
