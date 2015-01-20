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
include Makefile

# Object Directory
OBJECTDIR=build/Debug/Sun12-Solaris-Sparc

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/specilization.o \
	${OBJECTDIR}/newmain.o

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
.build-conf: ${BUILD_SUBPROJECTS} dist/Debug/Sun12-Solaris-Sparc/templates

dist/Debug/Sun12-Solaris-Sparc/templates: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/Sun12-Solaris-Sparc
	${LINK.cc} -o dist/Debug/Sun12-Solaris-Sparc/templates ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/specilization.o: specilization.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -o ${OBJECTDIR}/specilization.o specilization.cc

${OBJECTDIR}/newmain.o: newmain.cc 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -o ${OBJECTDIR}/newmain.o newmain.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/Sun12-Solaris-Sparc/templates
	${CCADMIN} -clean

# Subprojects
.clean-subprojects:

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.${CONF}
