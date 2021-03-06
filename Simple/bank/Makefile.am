##  Process this file with automake to create Makefile.in
##
## $Id: Makefile.am 76257 2007-01-02 10:02:38Z johnnyw $
##
## This file was generated by MPC.  Any changes made directly to
## this file will be lost the next time it is generated.
##
## MPC Command:
## ../bin/mwc.pl -type automake -noreldefs TAO.mwc

ACE_BUILDDIR = $(top_builddir)/..
ACE_ROOT = $(top_srcdir)/..
TAO_BUILDDIR = $(top_builddir)
TAO_IDL = ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(TAO_BUILDDIR)/TAO_IDL/tao_idl
TAO_IDL_DEP = $(TAO_BUILDDIR)/TAO_IDL/tao_idl
TAO_IDLFLAGS = -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf
TAO_ROOT = $(top_srcdir)

noinst_PROGRAMS =

## Makefile.bank_IDL.am

if BUILD_EXCEPTIONS

BUILT_SOURCES = \
  BankC.cpp \
  BankC.h \
  BankC.inl \
  BankS.cpp \
  BankS.h \
  BankS.inl

CLEANFILES = \
  Bank-stamp \
  BankC.cpp \
  BankC.h \
  BankC.inl \
  BankS.cpp \
  BankS.h \
  BankS.inl

BankC.cpp BankC.h BankC.inl BankS.cpp BankS.h BankS.inl: Bank-stamp

Bank-stamp: $(srcdir)/Bank.idl $(TAO_IDL_DEP)
	$(TAO_IDL) $(TAO_IDLFLAGS) $(srcdir)/Bank.idl
	@touch $@

noinst_HEADERS = \
  Bank.idl

endif BUILD_EXCEPTIONS

## Makefile.bank_client.am

if BUILD_EXCEPTIONS

noinst_PROGRAMS += client

client_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

client_SOURCES = \
  ../Simple_util.cpp \
  BankC.cpp \
  Bank_Client_i.cpp \
  client.cpp \
  ../Simple_util.h \
  Bank_Client_i.h

client_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PI.la \
  $(TAO_BUILDDIR)/tao/libTAO_CodecFactory.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_AnyTypeCode.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif BUILD_EXCEPTIONS

## Makefile.bank_server.am

if BUILD_EXCEPTIONS

noinst_PROGRAMS += server

server_CPPFLAGS = \
  -I$(ACE_ROOT) \
  -I$(ACE_BUILDDIR) \
  -I$(TAO_ROOT) \
  -I$(TAO_BUILDDIR) \
  -I$(TAO_ROOT)/orbsvcs \
  -I$(TAO_BUILDDIR)/orbsvcs

server_SOURCES = \
  ../Simple_util.cpp \
  AccountManager_i.cpp \
  Account_i.cpp \
  BankC.cpp \
  BankS.cpp \
  server.cpp \
  ../Simple_util.h \
  AccountManager_i.h \
  Account_i.h

server_LDADD = \
  $(TAO_BUILDDIR)/tao/libTAO_Utils.la \
  $(TAO_BUILDDIR)/tao/libTAO_PI.la \
  $(TAO_BUILDDIR)/tao/libTAO_CodecFactory.la \
  $(TAO_BUILDDIR)/tao/libTAO_PortableServer.la \
  $(TAO_BUILDDIR)/tao/libTAO_IORTable.la \
  $(TAO_BUILDDIR)/orbsvcs/orbsvcs/libTAO_CosNaming.la \
  $(TAO_BUILDDIR)/tao/libTAO_AnyTypeCode.la \
  $(TAO_BUILDDIR)/tao/libTAO.la \
  $(ACE_BUILDDIR)/ace/libACE.la

endif BUILD_EXCEPTIONS

## Clean up template repositories, etc.
clean-local:
	-rm -f *~ *.bak *.rpo *.sym lib*.*_pure_* core core.*
	-rm -f gcctemp.c gcctemp so_locations *.ics
	-rm -rf cxx_repository ptrepository ti_files
	-rm -rf templateregistry ir.out
	-rm -rf ptrepository SunWS_cache Templates.DB
