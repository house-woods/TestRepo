# Microsoft Developer Studio Project File - Name="NativeTL1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=NativeTL1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nativetl1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nativetl1.mak" CFG="NativeTL1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NativeTL1 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "NativeTL1 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "sysmon"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NativeTL1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "NativeTL1 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NativeTL1 - Win32 Release"
# Name "NativeTL1 - Win32 Debug"
# Begin Source File

SOURCE=.\src\AbstractSocketConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\AbstractSocketConnection.h
# End Source File
# Begin Source File

SOURCE=.\src\PTL1MoLite.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\PTL1MoLite.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketAcceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\SocketAcceptor.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\SocketConnection.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\SocketHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketListener.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\SocketListener.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1AutodiscoverAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1AutodiscoverAdapter.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1AutodiscoveryCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\tl1autodiscoverycmd.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1BaseCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1BaseCmd.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1CmBupProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1CmBupProcessor.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1CmdFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1CmdFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1CmdInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1CmdInfo.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1CmEventReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1CmEventReceiver.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Connection.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1ConnectionCountSemaphore.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1ConnectionCountSemaphore.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1ConnectionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1ConnectionManager.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1ConnectionResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1ConnectionResponse.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1DbEventHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1DbEventHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Defines.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Defines.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1EdPidCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1EdPidCmd.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1ErrCodeResolver.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1ErrCodeResolver.h
# End Source File
# Begin Source File

SOURCE=.\inc\TL1ErrCodes.h
# End Source File
# Begin Source File

SOURCE=.\inc\TL1EventReceiver.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1EventTypeResolver.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1EventTypeResolver.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1GenCmdFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1GenCmdFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1GenericCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1GenericCmd.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Grammar.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Grammar.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1InfoList.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1InfoList.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Lexer.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1LocRecoverHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1LocRecoverHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Logger.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1LoginCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1LoginCmd.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1LogoutCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1LogoutCmd.h
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Message.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1MoLiteDbMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1MoLiteDbMgr.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1MsgConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1MsgConverter.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1NameResolver.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1NameResolver.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1NESecurityObject.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1NESecurityObject.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Parser.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1ProductDecisionMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1ProductDecisionMaker.h
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Resp.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1RtrvHdrCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1RtrvHdrCmd.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1SocketAcceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1SocketAcceptor.h
# End Source File
# Begin Source File

SOURCE=.\src\TL1Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\inc\TL1Utility.h
# End Source File
# End Target
# End Project
