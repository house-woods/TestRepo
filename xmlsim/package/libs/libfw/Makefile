
include /vobs/EMS_OTS/GlobalRules.mk

##############################################################
#  TODO:
#
#  5)  Potential future issue with packaging: we're putting
#      this lib in install/Release/lib, but if we link other
#      servers with it (e.g., NM), this lib will be needed
#      at runtime at both the infrastructure and server
#      group levels.  Feel sorry for whoever has to figure
#      this out.
##############################################################

##############################################################
#  libtlabFw.so goes here.
#  The safest (albeit most conservative) way to ensure the
#  the one-definition rule of the ISO/ANSI C++ standard is to
#  build only one program or library within any one directory.
#  A template must have the same source in all uses of the
#  template.  Violating this rule produces undefined behavior.
#  Otherwise, we would place all Framework object files in
#  one common Intermediate directory, which would create one
#  large, Framework-wide template repository.  All object
#  files go into the same directory as this Makefile.
##############################################################

OUTDIR = $(FRAMEWORK_ROOT)/libs



##############################################################
#  The 'all' target might have multiple dependencies, and the
#  fact that it's not a real file target, we declare it .PHONY.
#  If MAKELEVEL > 0, then this make file was invoked by a 
#  build from the Framework directory, so we don't want to
#  make dependent projects because the parent make file will take
#  care of that.  If we're building this project by itself,
#  assume we want to make the dependent projects.
##############################################################

FW_TARG = libfw.so


.PHONY : all

ifeq (0,${MAKELEVEL})
all : IDLCompile $(FW_TARG)
else
all : $(FW_TARG)
endif

FW_LIB_SRCS = \
        AbstractProductDecisionMaker.cpp \
        AgentCmd.cpp \
        AgentCmdInfo.cpp \
        AuthorizationException.cpp \
        BaseCmd.cpp \
        BaseCmdFactory.cpp \
        BaseCmdInfo.cpp \
        BaseException.cpp \
        BaseNameResolver.cpp \
        BaseTypesASN1.cpp \
        CmAutodiscoveryCmd.cpp \
        CMEmsObject.cpp \
        CmEventQueue.cpp \
        CmEventQueueControlCmd.cpp \
        CMFrameworkServer.cpp \
        CM_i.cpp \
        CmMessageQueue.cpp \
        CmNameResolver.cpp \
        CmNetworkElement.cpp \
        CmPropertyObject.cpp \
        CmServer.cpp \
        CMSessionManager.cpp \
        ComManager.cpp \
        ConfigFile.cpp \
        ConfigNames.cpp \
        ConfigObject.cpp \
        CORBAUtil.cpp \
        CpUtil.cpp \
        Cryptor.cpp \
        DbEventHandeler.cpp \
        EMSDefines.cpp \
        EmsGUIHandeler.cpp \
        EmsLock.cpp \
        EmsObject.cpp \
        EmsObjectFactory.cpp \
        EmsProcess.cpp \
        EmsTaskScheduler.cpp \
        EmsUnixDefines.cpp \
        EventHandeler.cpp \
        EventManager.cpp \
        FileReader.cpp \
        FWComponent.cpp \
        GuiCmdInfo.cpp \
        LicenseManager.cpp \
        MD5.cpp	\
        MoAttributefactory.cpp \
        NavigatorObject.cpp \
        NavigatorObjectFactory.cpp \
        NetworkElement.cpp \
        NETYPEDEFINES.cpp \
        NodeDefines.cpp \
        NodeInterface.cpp \
        NotificationManager.cpp \
        OSUtil.cpp \
        ProductDecisionMaker.cpp \
        PropertyObject.cpp \
        ProtocolProductDecisionMaker.cpp \
        QueryObjectFactory.cpp \
        RandomNumberGenerator.cpp \
        RdbRefEntityTable.cpp \
        RdbUtil.cpp \
        ReusableThread.cpp \
        Rijndael.cpp \
        SecurityAccess.cpp \
        SecurityManager.cpp \
        SecurityProfileManager.cpp \
        Server_i.cpp \
        ServerSessionData.cpp \
        ServerSessionManager.cpp \
        SessionManager.cpp \
        SMATAttribute.cpp \
        SMATCMNMRecord.cpp \
        SMATFMRecord.cpp \
        SMATRecord.cpp \
        sm_i.cpp	\
        SmRDbMgr.cpp \
        SmServer.cpp \
        StringParser.cpp \
        sysErrLog.cpp \
        SystemConfigMgr.cpp \
        ThreadPool.cpp \
        TimeCollector.cpp \
        ttime.cpp \
        tutil.cpp \
        UserManager.cpp
        
               
FW_LIB_OBJS = $(FW_LIB_SRCS:.cpp=.o)


##############################################################
#  vpath directive so [clear|gnu]make can find our C++
#  source file prerequisites, which are typically
#  scattered all over Framework.  The paths here were
#  specifically ordered such that they contain from most
#  to least cpp files needed for the Framework library.
##############################################################

FW_LIB_VPATH_DIRS = \
          $(FRAMEWORK_ROOT)/libs/libfw \
          $(FRAMEWORK_ROOT)/libs/libelemmgr/cm \
          $(FRAMEWORK_ROOT)/libs/libelemmgr/proxy \
          $(FRAMEWORK_ROOT)/libs/libelemmgr/fm \
          $(FRAMEWORK_ROOT)/libs/libdbms \
          $(FRAMEWORK_ROOT)/servers/sm

vpath %.cpp $(FW_LIB_VPATH_DIRS)



##############################################################
#  Construct the arguments to the compiler/linker.
#
#  -xregs=no%appl - Does not allow use of registers g2, g3 (v9, v9a, v9b).
#           In the SPARC ABI, these registers are described as application
#           registers.  Using these registers can increase performance
#           because fewer load and store instructions are needed. However,
#           such use can conflict with programs that use the registers for
#           other purposes.  Use of these registers in a public library is
#           generally not compliant with the SPARC ABI.  When building a
#           library for public use, compile all objects with the
#           -xregs=no%appl option to ensure that the application registers
#           are not used.
#
#  -G - Build a dynamic shared library instead of an executable file. 
#       All source files specified in the command line are compiled with 
#       -Kpic by default. When building a shared library that uses templates, 
#       it is necessary in most cases to include in the shared library 
#       those template functions that are instantiated in the template data 
#       base. Using this option automatically adds those templates to the 
#       shared library as needed. 
#
#
#  -Kpic - Generates position-independent code (small model), which is 
#          fast, but has limited range. Equivalent to -Kpic. Permits 
#          references to at most 2**11 unique external symbols on 32-bit 
#          architectures; 2**10 on 64-bit.  NOTE: if this fails may
#          need to use -KPIC instead.
#
#  -h libtlabFw.so - Assigns the name to the generated dynamic shared 
#         library. This is a loader option, passed to ld. In general, the 
#         name after -h should be exactly the same as the one after -o. A 
#         space between the -h and name is optional.  The compile-time 
#         loader assigns the specified name to the shared dynamic library 
#         you are creating. It records the name in the library file as 
#         the intrinsic name of the library. If there is no -hname option, 
#         then no intrinsic name is recorded in the library file. 
#
#  -norunpath - Does not build a runtime search path for shared libraries 
#         into the executable. If an executable file uses shared libraries, 
#         then the compiler normally builds in a path that points the 
#         runtime linker to those shared libraries. To do so, the compiler 
#         passes the -R option to ld. The path depends on the directory 
#         where you have installed the compiler. This option is helpful if 
#         you have installed the compiler in some nonstandard location, 
#         and you ship an executable file to your customers. Your customers 
#         do not have to work with that nonstandard location. 
#
#
#  ORBIXTALK_DLL define not researched, came from NT.
##############################################################

FW_LIB_CXXFLAGS = -xregs=no%appl -G

FW_LIB_INCLUDES = \
	-I$(FRAMEWORK_ROOT)/libs/libtl1 \
	-I$(FRAMEWORK_ROOT)/libs/libidl \
        -I$(FRAMEWORK_ROOT)/libs/libfw \
        -I$(FRAMEWORK_ROOT)/libs/libelemmgr/cm \
        -I$(FRAMEWORK_ROOT)/libs/libelemmgr/proxy \
        -I$(FRAMEWORK_ROOT)/libs/libelemmgr/fm \
        -I$(FRAMEWORK_ROOT)/libs/libdbms \
	-I$(FRAMEWORK_ROOT)/libs/libsnmp \
        -I$(FRAMEWORK_ROOT)/servers/sm \
	-I$(LIBURADIUS_HOME) \
	$(ORAINCLUDE) \
        $(OSPACE_INCLUDES) \
	$(ORB_INCLUDES) \
        $(SSL_INCLUDES)
        

FW_LIB_DEFINES = \
	$(FW_DEFINES) \
	$(FWONLY_DEFINES) \
	$(OSPACE_DEFINES) \
	$(ORB_DEFINES)


FW_LIB_LDFLAGS = \
	-h $(FW_TARG) \
	-norunpath \
	$(FW_LIB_CXXFLAGS) \

CXXFLAGS = \
	$(FW_LIB_CXXFLAGS) \
	$(FW_CXXFLAGS) \
	$(FW_LIB_INCLUDES) \
	$(FW_LIB_DEFINES)


LDFLAGS = \
	$(FW_LIB_LDFLAGS)



##############################################################
#  Rule for building (linking) the Framework shared library.
##############################################################

$(FW_TARG) : $(FW_LIB_OBJS)
ifeq (1,${SB})
	@-/bin/echo "$(FW_TARG) source browser info generated."
else
	@if test -f $(FW_TARG) || test -h $(FW_TARG) ; then \
		/bin/rm -f $(FW_TARG) ; \
	fi
	@if test -f $(OUTDIR)/$(FW_TARG) || test -h $(OUTDIR)/$(FW_TARG) ; then \
		/bin/rm -f $(OUTDIR)/$(FW_TARG) ; \
	fi
	$(CXX) $(FW_LIB_OBJS) $(LDFLAGS) -o $@
	@if test -f $(FW_TARG) ; then \
		/bin/echo Soft linking $(OUTDIR)/$(FW_TARG) to `pwd`/$(FW_TARG) ; \
		/bin/ln -s `pwd`/$(FW_TARG) $(OUTDIR)/$(FW_TARG) ; \
	fi
endif




##############################################################
#  Rules for projects on which I depend.
##############################################################

.PHONY : IDLCompile

IDLCompile : 
	@cd $(FRAMEWORK_ROOT)/libs/libidl ; \
	$(MAKE) -f Makefile.idl

##############################################################
#  Rule for clean, including cleaning the projects
#  I depend on.  CCADMIN cleans the template repository.
##############################################################

.PHONY : clean

ifeq (1,${MAKELEVEL})
clean :
else
clean :  
endif
	@/bin/echo Removing Framework library objects...
	@-/bin/rm -f $(FW_LIB_OBJS) $(FW_TARG) core $(OUTDIR)/$(FW_TARG)
	@-$(CCADMIN) -clean
	@-/bin/rm -f -r $(TEMPL_REPOS)/sb_*
