#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add custumized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=cc
CCC=CC
CXX=CC
FC=f77

# Include project Makefile
include nm_1-Makefile.mk

# Object Directory
OBJECTDIR=build/Default/Sun12-Solaris-Sparc

# Object Files
OBJECTFILES=

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: 
	cd /vobs/EMS_OTS/Framework/servers/nm && make -f Makefile

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	cd /vobs/EMS_OTS/Framework/servers/nm && make -f Makefile clean

# Subprojects
.clean-subprojects:
