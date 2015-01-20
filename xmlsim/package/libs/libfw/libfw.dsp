# Microsoft Developer Studio Project File - Name="libfw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libfw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libfw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libfw.mak" CFG="libfw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libfw - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libfw - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libfw - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libfw - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libfw - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libfw - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libfw"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libfw - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\libfw.dll
InputPath=.\Debug\libfw.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libfw - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\libfw.dll
InputPath=.\DebugU\libfw.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libfw - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\libfw.dll
InputPath=.\ReleaseMinSize\libfw.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libfw - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\libfw.dll
InputPath=.\ReleaseMinDependency\libfw.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libfw - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\libfw.dll
InputPath=.\ReleaseUMinSize\libfw.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "libfw - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\libfw.dll
InputPath=.\ReleaseUMinDependency\libfw.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "libfw - Win32 Debug"
# Name "libfw - Win32 Unicode Debug"
# Name "libfw - Win32 Release MinSize"
# Name "libfw - Win32 Release MinDependency"
# Name "libfw - Win32 Unicode Release MinSize"
# Name "libfw - Win32 Unicode Release MinDependency"
# Begin Source File

SOURCE=.\AbstractEventReceiver.h
# End Source File
# Begin Source File

SOURCE=.\AbstractProductDecisionMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\AbstractProductDecisionMaker.h
# End Source File
# Begin Source File

SOURCE=.\AgentCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\AgentCmd.h
# End Source File
# Begin Source File

SOURCE=.\AgentCmdInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AgentCmdInfo.h
# End Source File
# Begin Source File

SOURCE=.\AuthorizationException.cpp
# End Source File
# Begin Source File

SOURCE=.\AuthorizationException.h
# End Source File
# Begin Source File

SOURCE=.\BaseCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseCmd.h
# End Source File
# Begin Source File

SOURCE=.\BaseCmdFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseCmdFactory.h
# End Source File
# Begin Source File

SOURCE=.\BaseCmdInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseCmdInfo.h
# End Source File
# Begin Source File

SOURCE=.\BaseException.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseException.h
# End Source File
# Begin Source File

SOURCE=.\BaseNameResolver.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseNameResolver.h
# End Source File
# Begin Source File

SOURCE=.\BaseTypesASN1.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseTypesASN1.h
# End Source File
# Begin Source File

SOURCE=.\BupChannelSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\BupChannelSupplier.h
# End Source File
# Begin Source File

SOURCE=.\CmMessageQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\CmMessageQueue.h
# End Source File
# Begin Source File

SOURCE=.\ComManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ComManager.h
# End Source File
# Begin Source File

SOURCE=.\ConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigFile.h
# End Source File
# Begin Source File

SOURCE=.\ConfigNames.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigNames.h
# End Source File
# Begin Source File

SOURCE=.\ConfigObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigObject.h
# End Source File
# Begin Source File

SOURCE=.\CORBAUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBAUtil.h
# End Source File
# Begin Source File

SOURCE=.\Cryptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Cryptor.h
# End Source File
# Begin Source File

SOURCE=.\DbChannelSupp.cpp
# End Source File
# Begin Source File

SOURCE=.\DbChannelSupp.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\EMSDefines.cpp
# End Source File
# Begin Source File

SOURCE=.\EMSDefines.h
# End Source File
# Begin Source File

SOURCE=.\EmsLock.cpp
# End Source File
# Begin Source File

SOURCE=.\EmsLock.h
# End Source File
# Begin Source File

SOURCE=.\EmsMoValueT.h
# End Source File
# Begin Source File

SOURCE=.\EmsProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\EmsProcess.h
# End Source File
# Begin Source File

SOURCE=.\EmsTaskScheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\EmsTaskScheduler.h
# End Source File
# Begin Source File

SOURCE=.\EmsTypes.h
# End Source File
# Begin Source File

SOURCE=.\EmsUnixDefines.cpp
# End Source File
# Begin Source File

SOURCE=.\EmsUnixDefines.h
# End Source File
# Begin Source File

SOURCE=.\FileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\FileReader.h
# End Source File
# Begin Source File

SOURCE=.\FWComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\FWComponent.h
# End Source File
# Begin Source File

SOURCE=.\GlobalChannelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalChannelManager.h
# End Source File
# Begin Source File

SOURCE=.\LicenseManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseManager.h
# End Source File
# Begin Source File

SOURCE=.\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5.h
# End Source File
# Begin Source File

SOURCE=.\MessageQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageQueue.h
# End Source File
# Begin Source File

SOURCE=.\MoAttributefactory.cpp
# End Source File
# Begin Source File

SOURCE=.\MoAttributefactory.h
# End Source File
# Begin Source File

SOURCE=.\MoAttributePo.h
# End Source File
# Begin Source File

SOURCE=.\MoCorbaList.h
# End Source File
# Begin Source File

SOURCE=.\NetworkElement.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkElement.h
# End Source File
# Begin Source File

SOURCE=.\NetworkElementAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkElementAccess.h
# End Source File
# Begin Source File

SOURCE=.\NETYPEDEFINES.cpp
# End Source File
# Begin Source File

SOURCE=.\NETYPEDEFINES.h
# End Source File
# Begin Source File

SOURCE=.\NodeDefines.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeDefines.h
# End Source File
# Begin Source File

SOURCE=.\NsgAbstractNotification.h
# End Source File
# Begin Source File

SOURCE=.\NsgErrorDef.h
# End Source File
# Begin Source File

SOURCE=.\NsgEventChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\NsgEventChannel.h
# End Source File
# Begin Source File

SOURCE=.\OSUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\OSUtil.h
# End Source File
# Begin Source File

SOURCE=.\PDbObject.h
# End Source File
# Begin Source File

SOURCE=.\Platform.h
# End Source File
# Begin Source File

SOURCE=.\PManagedObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PManagedObject.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolProductDecisionMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\ProtocolProductDecisionMaker.h
# End Source File
# Begin Source File

SOURCE=.\RandomNumberGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomNumberGenerator.h
# End Source File
# Begin Source File

SOURCE=.\RdbRefEntityTable.cpp
# End Source File
# Begin Source File

SOURCE=.\RdbRefEntityTable.h
# End Source File
# Begin Source File

SOURCE=.\RdbUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\RdbUtil.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ReusableThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ReusableThread.h
# End Source File
# Begin Source File

SOURCE=.\Rijndael.cpp
# End Source File
# Begin Source File

SOURCE=.\Rijndael.h
# End Source File
# Begin Source File

SOURCE=.\Server_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_i.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\StringParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StringParser.h
# End Source File
# Begin Source File

SOURCE=.\sysErrLog.cpp
# End Source File
# Begin Source File

SOURCE=.\sysErrLog.h
# End Source File
# Begin Source File

SOURCE=.\SystemConfigMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemConfigMgr.h
# End Source File
# Begin Source File

SOURCE=.\ThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadPool.h
# End Source File
# Begin Source File

SOURCE=.\TimeCollector.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeCollector.h
# End Source File
# Begin Source File

SOURCE=.\ttime.cpp
# End Source File
# Begin Source File

SOURCE=.\ttime.h
# End Source File
# Begin Source File

SOURCE=.\tutil.cpp
# End Source File
# Begin Source File

SOURCE=.\tutil.h
# End Source File
# Begin Source File

SOURCE=.\UserException.h
# End Source File
# Begin Source File

SOURCE=.\version.cpp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Target
# End Project
