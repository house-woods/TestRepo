
############################################################
#  Make conventions
############################################################

SHELL = /bin/sh


.SUFFIXES:
.SUFFIXES: .cpp .o .scm .src .idl


############################################################
#  END Make conventions
############################################################


############################################################
#  TAO Environment Settings
# IMPORTANT: keep this settings before Framework-wide compiler/
# linker settings in order to pick up correct compiler
############################################################

#keep the following around so we can debug
#ACE_ROOT = /home/objstadm/TAO14_P11_OpenSSL_098c/Release/ACE_wrappers
ACE_ROOT := /vobs/cots/TAO/ACE_wrappers
TAO_ROOT := $(ACE_ROOT)
SSL_ROOT = /vobs/cots/TAO/OpenSSL
SNMP_PP_ROOT = /vobs/SNMP/snmp++
DES_ROOT = /vobs/SNMP/libdes
FRAMEWORK_ROOT = /home/bwang1/src/EMS_OTS/Framework

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

############################################################
#  END TAO Environment Settings
############################################################

############################################################
# ORACLE Environment Settings
# IMPORTANT: keep this settings before Framework-wide compiler/
# linker settings in order to pick up correct compiler
############################################################

ORAINCLUDE=-I/home/bwang1/occi/include
ORALIBPATH=-L/home/bwang1/occi/lib

ORACLE_LDFLAGS = \
        $(ORALIBPATH) \
        -locci \
        -lclntsh \
        -laio \
        -lposix4 \
        -lkstat \
        -lm \
        -lthread \
        -ldl \
        -lresolv


############################################################
#  END ORACLE Environment Settings
############################################################

############################################################
#  Framework-wide compiler/linker settings.
#
#  We compile and link in separate steps.  When doing so it
#  is IMPORTANT to send come compiler options to both the
#  compiler and linker.  We attempt to do that with FW_CXXFLAGS,
#  but other options (such as -mt in the OSpace section) are
#  an exception to that rule.  See "2.3.3 Consistent Compiling 
#  and Linking" of the Sun C++ User's Guide for more info.
############################################################

#  Full path to compiler (CXX = Gnu, but works with clearmake)

CXX_HOME = /home/addon/Sun_Studio_11/SUNWspro

CXX =      /home/addon/Sun_Studio_11/SUNWspro/bin/CC

JAVA_HOME=/home/addon/jdk1.5.0_11

export JAVA_HOME

#  Full path to compiler's template repostitory administrator.

CCADMIN =     /home/addon/Sun_Studio_10/SUNWspro/bin/CCadmin

TEMPL_REPOS = SunWS_cache


#  Flags for the C++ Compiler proper
#
#  -g - Add debug info.
#
#  -xsb - Produce source browser info - this seems to cause
#         problems during a clearmake parallel build.
#
#  -features=
#
#     For the features, prefixing an argument with "no%"
#     turns it off.
#
#       conststrings - Put literal strings in read-only memory.
#       except - Allow C++ exceptions
#       rtti - Always enabled in the compiler's standard mode.
#              No need to specify (unless -compat=4).
#       namespace - Recognize the keywords namespace and using.
#       bool - Allow the bool type and literals. When enabled, 
#              the macro _BOOL=1. When not enabled, the macro 
#              is not defined.
#       mutable - Recognize the keyword mutable (used in ospace).
#       iddollar - Allow a $ as a non-initial identifier character.
#       altspell - Recognize alternative token spellings (for example, 
#                  "and" for "&&").  We turn this off because some
#                  CM generated lib headers declare 'and', 'or', etc.
#                  as identifiers.
#
#  -xarch=v9 - Compile for the SPARC-V9 ISA. Enables the compiler
#              to generate code for good performance on the V9 SPARC 
#              architecture.  The resulting .o object files are in 
#              ELF64 format and can only be linked with other SPARC-V9 
#              object files in the same format.  The resulting 
#              executable can only be run on an UltraSPARC processor 
#              running a 64-bit enabled Solaris operating environment 
#              with the 64-bit kernel.  -xarch=v9 is only available 
#              when compiling in a 64-bit enabled Solaris environment. 
#
#  -compat=4 - (Compatibility mode) Set language and binary compatibility 
#              to that of the 4.0.1, 4.1, and 4.2 compilers. Set the 
#              __cplusplus preprocessor macro to 1 and the __SUNPRO_CC_COMPAT 
#              preprocessor macro to 4.  This is needed because of UHC
#              and the fact that the EMS code was developed on a 32-bit
#              environment.  Going to 64-bit (xarch=v9) is not simply
#              a matter of compiling with the 64-bit option; it entails
#              using lint to discover possible bugs in the 32-bit code
#              that would manifest themselves in a 64-bit environment.
#
#  -xildoff - Turns off the incremental linker (use standard linker).
#             This greatly reduces the binary size, though links 
#             take a tad longer.
#
#  -errtags - Displays the message tag for each warning message of the C++
#             compiler front-end.
#
#  -errwarn - Causes the C++ compiler to exit with a failure status for the
#             given warning messages.
#
#  -i - Ignore LD_LIBRARY_PATH during link.  We should be able to resolve
#       all symbols by explicitly listing our libraries, and letting the
#       the compiler pick up its standard libraries.
#

#FW_CORE_CXXFLAGS = -features=%all,no%iddollar,no%altspell \
#                   -library=iostream,no%Cstd \
#                   -mt

FATAL_WARNING_LIST = nonpodvarargw

FW_CORE_CXXFLAGS = -features=%all,no%iddollar,no%altspell \
                   -library=iostream \
                   -mt

FW_CXXFLAGS = $(FW_CORE_CXXFLAGS) \
              -g \
              -xildoff \
              -errtags \
              -errwarn=$(FATAL_WARNING_LIST) \
              -i \
              -R.:/usr/lib/lwp/:/usr/lib:/lib \
              -KPIC


#  For official builds, we want the executables to contain all debug
#  information (-xs option, which makes them much larger).  For developer
#  builds, we turn off the -xs option, which means some debug info
#  must be accessed from the .o files.

# we want to keep our deployment executables smaller
#ifeq (1,${GodEmperorBuilder})
#FW_CXXFLAGS += -xs
#endif


#  If SB=1 is on the clearmake command line, we're doing a source browser build.

ifeq (1,${SB})
FW_CXXFLAGS := $(FW_CORE_CXXFLAGS) -xsbfast
endif


FW_DEFINES       = -DEMSUNIX -Dasctime="threadunsafe_asctime" -Dctermid="threadunsafe_ctermid" -Dctime="threadunsafe_ctime" -Dfgetgrent="threadunsafe_fgetgrent" \
			-Dfgetpwent="threadunsafe_fgetpwent" -Dfgetspent="threadunsafe_fgetspent" -Dgamma="threadunsafe_gamma" -Dgetgrgid="threadunsafe_getgrgid" -Dgetgrnam="threadunsafe_getgrnam" \
			-Dgetlogin="threadunsafe_getlogin" -Dgetpwnam="threadunsafe_getpwnam" -Dgetpwuid="threadunsafe_getpwuid" -Dgetgrent="threadunsafe_getgrent" \
			-Dgethostbyaddr="threadunsafe_gethostbyaddr" -Dgethostbyname="threadunsafe_gethostbyname" -Dgethostent="threadunsafe_gethostent" -Dgetnetbyaddr="threadunsafe_getnetbyaddr" \
			-Dgetnetbyname="threadunsafe_getnetbyname" -Dgetnetent="threadunsafe_getnetent" -Dgetprotobyname="threadunsafe_getprotobyname" \
			-Dgetprotobynumber="threadunsafe_getprotobynumber" -Dgetprotoent="threadunsafe_getprotoent" -Dgetpwent="threadunsafe_getpwent" \
			-Dgetrpcbyname="threadunsafe_getrpcbyname" -Dgetrpcbynumber="threadunsafe_getrpcbynumber" -Dgetrpcent="threadunsafe_getrpcent" \
			-Dgetservbyname="threadunsafe_getservbyname" -Dgetservbyport="threadunsafe_getservbyport" -Dgetservent="threadunsafe_getservent" \
			-Dgetspent="threadunsafe_getspent" -Dgetspnam="threadunsafe_getspnam" -Dgmtime="threadunsafe_gmtime" -Dlgamma="threadunsafe_lgamma" \
			-Dlocaltime="threadunsafe_localtime" -Dnis_sperror="threadunsafe_nis_sperror" -Drand="threadunsafe_rand" \
			-Dreaddir="threadunsafe_readdir" -Dstrtok="threadunsafe_strtok" -Dtmpnam="threadunsafe_tmpnam" -Dttyname="threadunsafe_ttyname"

FWONLY_DEFINES   = -DEMS_FRAMEWORK

FW_LIB           = $(FRAMEWORK_ROOT)/libs/libfw/libfw.so

NATIVETL1_LIB    = $(FRAMEWORK_ROOT)/libs/libtl1/libtl1.so

RDBMGR_LIB	=$(FRAMEWORK_ROOT)/libs/libdbms/libdbms.so

IDLGEN_LIB = $(FRAMEWORK_ROOT)/libs/libidl/libidl.so

ELEMMGR_LIB = $(FRAMEWORK_ROOT)/libs/libelemmgr/libelemmgr.so

#
#  NOTE: The ObjectStore schema generator cannot, like, deal with
#  '%' characters in its compilation options.  So we remove them
#  here, on the assumption that individual project make files
#  create CXXFLAGS, and use OSSG_CXXFLAGS in the ossg command.
#  The % character occurs in our C++ compiler options -library
#  and -features.
#

NOCSTD=,no%Cstd
NO_PERCENT_STUFF=-features=%all,no%iddollar,no%altspell
TMP1 = $(subst $(NOCSTD),,$(CXXFLAGS))
OSSG_CXXFLAGS = $(subst $(NO_PERCENT_STUFF),,$(TMP1))


#  ObjectStore schema compilation requires that the adb file
#  gets created locally.  This variable provides a place
#  in /tmp for the individual user.


############################################################
#  END Framework-wide compiler settings.
############################################################



############################################################
#  Framework-wide clearmake macros.
############################################################

#  Sets the umask(1) value to be used for files created from
#  a clearmake build script.

CCASE_BLD_UMASK = 2



############################################################
#  END Framework-wide clearmake macros.
############################################################


############################################################
#  ObjectSpace compiler/linker flags.  These correspond to
#  those you'd see when compiling their examples, which
#  according to their documentation is the set of flags
#  we should use.
############################################################

OBJECTSPACE_HOME = /home/bwang1/ospace2.2.2/ccs-2.2.2


#  Use libiostream, the classic iostreams library, per how the
#  Objectspace examples compile.
#
#  mt = multi-threaded (Sun) library.

OSPACE_INCLUDES = \
	-I$(OBJECTSPACE_HOME) \
	-I$(OBJECTSPACE_HOME)/ospace/std


OSPACE_DEFINES = \
	-DOS_SOLARIS_2_7 \
	-DOS_STL_ASSERT \
	-DOS_NO_WSTRING \
	-DOS_MULTI_THREADED \
	-DOS_SOLARIS_PTHREADS \
	-DOS_PTHREADS \
	-DOS_REALTIME \
	-D_REENTRANT \
	-D_POSIX_PTHREAD_SEMANTICS \
  -DOS_USE_ALTERNATE_STD \
  -DOS_USE_STD_IOSTREAMS \
	-DTELLABS_EMS_BYPASS_AUTOPTR \
       -DTELLABS_TAO_UNION_SEMUN \

OSPACE_LDFLAGS = \
	-L$(OBJECTSPACE_HOME)/lib \
	-los_std \
	-los_helper \
	-los_time \
	-los_thread \
	-los_io \
	-los_security \
	-los_pipe \
	-los_file \
	-los_stream \
	-los_network \
	-los_unix \
	-los_uss_file \
	-los_uss_network \
	-los_uss_security \
	-los_uss_std \
	-los_uss_helper \
	-los_uss_time \
	-los_uss_unix \
	-lnsl \
	-lsocket \
	-lposix4 \
  -lospace


############################################################
#  END ObjectSpace compiler/linker flags.
############################################################



############################################################
#  TAO (C++ Server ORB) and SSL-specific compiler/linker flags.
#
############################################################

ORB_INCLUDES = \
	-I$(ACE_ROOT) \
  -I$(ACE_ROOT)/include \
  -I$(ACE_ROOT)/include/ace \
  -I$(ACE_ROOT)/include/tao \
  -I$(ACE_ROOT)/include/orbsvcs \
  -I$(ACE_ROOT)/include/orbsvcs/orbsvcs

ORB_DEFINES = \
#	-DACE_USES_OLD_IOSTREAMS

ORB_LDFLAGS = \
  -L$(ACE_ROOT)/lib \
  -lACE \
  -lACE_SSL \
  -lTAO \
  -lTAO_CosNaming \
  -lTAO_CosNotification \
  -lTAO_CosEvent \
  -lTAO_RTCORBA \
  -lTAO_PortableServer \
  -lTAO_CosNotification_Serv \
  -lTAO_CosNotification_Skel \
  -lTAO_RTPortableServer \
  -lTAO_Messaging \
  -lTAO_Valuetype \
  -lTAO_SSLIOP \
  -lTAO_Strategies \
  -lTAO_RTEvent

SSL_INCLUDES = 	-I$(SSL_ROOT)/include

SNMP_PP_INCLUDES = -I$(SNMP_PP_ROOT)/include
                   
SNMP_PP_LDFLAGS = \
  -L$(SNMP_PP_ROOT)/lib \
  -lsnmp++\

DES_INCLUDES = -I$(DES_ROOT)

DES_LDFLAGS = \
  -L$(DES_ROOT) \
  -ldes
                  
############################################################

############################################################
#  JacOrb (Java Client ORB) compiler/linker flags.
#
############################################################

#JAVA_ORB_HOME = /home/objstadm/jacorb

############################################################
#  END JacOrb (Java Client ORB) compiler/linker flags.
############################################################




############################################################
#  ObjectStore compiler/linker flags.
#
#  NOTES:  The union of Ostore documentation and their examples
#          use the following options.  We do NOT use all
#          these options per the notes...
#
#  1)  -xarch=v9 - Compile for the SPARC-V9 ISA.
#
#  2)  -vdelx - Compatibility mode only (-compat[=4]): For expressions 
#               using delete[], this option generates a call to the 
#               runtime library function _vector_deletex_ instead of 
#               generating a call to _vector_delete_.  The function 
#               _vector_delete_ takes two arguments: the pointer to be 
#               deleted and the size of each array element.  The function 
#               _vector_deletex_ behaves the same as _vector_delete_ except 
#               that it takes a third argument: the address of the 
#               destructor for the class. This third argument is not used by 
#               the function, but is provided to be used by third-party vendors. 
#               Default: The compiler generates a call to _vector_delete_ for 
#               expressions using delete[].  Warnings: This is an obsolete 
#               option that will be removed in future releases. Don't use 
#               this option unless you have bought some software from a 
#               third-party vendor and the vendor recommends using this option. 
#
#  NOTE: 1) and 2) and incompatible, so leaving off -vdelx !!!!!!!!!!!!!!!!!!!
#
#
#  3)  -template=wholeclass - Instantiate a whole template class, 
#             rather than only those functions that are used. 
#             You must reference at least one member of the 
#             class; otherwise, the compiler does not instantiate 
#             any members for the class.   This option is 
#             equivalent to -pta.  Note: this came from the Ostore
#             examples, and causes compilation errros in some of
#             the EMS code.  Taking it out for now, because it is
#             not specified as necessary in the Ostore documentation.
#
#  4)  -DPTHREADS - They use it; so do we.
#
#  5)  -mt - Already specified in Ospace.  (Multithreaded)
#
#  6)  -g - Already specified above as a Framework flag.
#           (Debug info)
#
#  7)  -KPIC - SPARC: Same as -xcode=pic32.  Generates position-independent 
#              code (large model), which is slow, but has full range. Equivalent 
#              to -KPIC. Permits references to at most 2**30 unique external
#              symbols on 32-bit architectures; 2**29 on 64-bit.  For SPARC 
#              and UltraSPARC processors, when you use -xarch=(v9|v9a|v9b), 
#              the default is -xcode=abs64.  NOT SURE IF WE SHOULD USE THIS
#              BECAUSE IT'S USUALLY FOR BUILDING SHARED LIBRARIES.
#
#  8)  -R - Builds dynamic library search paths into the executable file. 
#           If the LD_RUN_PATH environment variable is defined and the -R 
#           option is specified, then the path from -R is scanned and the 
#           path from LD_RUN_PATH is ignored. 
#
#  9)  -ldl - the dynamic linking interface library.  Do "man libdl"
#             for more info.
#
# 10)  -xildoff - Turns off the incremental linker.
#
# 11)  -losqry - Ostore query feature library
#
# 12)  -loscol - Ostore collections feature library
#
# 13)  -los [-losth]  - Any Ostore feature library [multithreaded]
#
# 14)  -losmop - Ostore MOP feature library (Framework doesn't
#                use this feature specifically, but Ostore documentation
#                says to include it if using "queries & indexes."
#
#  NOTE: Ostore library link order is important!
#
############################################################

#ODI_HOME = /home/objstadm/ODISP9patch

#ODI_ROOT = $(ODI_HOME)/ostore


#ODI_INCLUDES = -I$(ODI_HOME)/ostore/include


#ODI_DEFINES = \
#	-DPTHREADS


#ODI_LDFLAGS = \
#	-L$(ODI_HOME)/ostore/lib \
#	-losmop \
#	-losqry \
#	-loscol \
#	-losdbu \
#	-los \
#	-losth \
#	-ldl


############################################################
#  END ObjectStore compiler/linker flags.
############################################################


############################################################
#  PURIFY staff
############################################################

RSU_LICENSE_MAP=/home/addon/rational/config/License_Map

PURIFY_HOME=/home/addon/rational/releases/PurifyPlusFamily.2003.06.00/sun4_solaris2/bin

PURIFY_DIRS=home tmp usr vobs

ifeq (1,${PURIFY})
PURIFY_OPT=$(PURIFY_HOME)/purify -follow_terminal_symlinks -cache-dir=$(shell /bin/pwd) -always-use-cache-dir 
else
PURIFY_OPT=
endif

############################################################
#  End PURIFY staff
############################################################


############################################################
#  Pattern rules for compiling .cpp C++ source files.
#  The individual project make files are responsible for
#  constructing:
#
#    CXXFLAGS - flags to pass for compilation
#    LDFLAGS - flags to pass to linkage
#
#  Also, we are cleaning up the Sun C++ compiler template
#  repository to avoid confusing clearmake.  See clearmake
#  documentation regarding its difficulties with the
#  template repository (and the not so great solutions
#  they have for it).
#
#  The double colon '::' makes the rule terminal, which means 
#  that its prerequisite may not be an intermediate file.
#  Note, changed this to a single : so that OTS CM can
#  build IDL files in the CM make file.
############################################################

#%.o : %.cpp
#	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
#	@-/bin/rm -f *core*




############################################################
#  END construction of commands and pattern rules.
############################################################
