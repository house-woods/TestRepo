# -*- makefile -*-
#----------------------------------------------------------------------------
#       GNU ACE Workspace
#
# $Id: GNUACEWorkspaceCreator.pm 77499 2007-03-02 18:19:05Z elliott_c $
#
# This file was generated by MPC.  Any changes made directly to
# this file will be lost the next time it is generated.
#
# MPC Command:
# /export/anduriltmp/bczar/DOC_ROOT/stage-29192/ACE_wrappers/bin/mwc.pl -type gnuace -exclude TAO/TAO_*.mwc,TAO/CIAO/CIAO_*.mwc -recurse -hierarchy -relative ACE_ROOT=/export/anduriltmp/bczar/DOC_ROOT/stage-29192/ACE_wrappers -relative TAO_ROOT=/export/anduriltmp/bczar/DOC_ROOT/stage-29192/ACE_wrappers/TAO -relative CIAO_ROOT=/export/anduriltmp/bczar/DOC_ROOT/stage-29192/ACE_wrappers/TAO/CIAO
#
#----------------------------------------------------------------------------

MAKEFILE = GNUmakefile

ifeq ($(findstring k,$(MAKEFLAGS)),k)
  KEEP_GOING = -
endif

include $(ACE_ROOT)/include/makeinclude/macros.GNU

all: bank_IDL bank_server bank_client

REMAINING_TARGETS := $(subst all, , $(TARGETS_NESTED:.nested=)) $(CUSTOM_TARGETS)

$(REMAINING_TARGETS):
	@$(MAKE) -f GNUmakefile.bank_IDL $(@)
	@$(MAKE) -f GNUmakefile.bank_server $(@)
	@$(MAKE) -f GNUmakefile.bank_client $(@)

.PHONY: bank_IDL
bank_IDL:
	@$(MAKE) -f GNUmakefile.bank_IDL all

.PHONY: bank_server
bank_server: bank_IDL
	@$(MAKE) -f GNUmakefile.bank_server all

.PHONY: bank_client
bank_client: bank_IDL bank_server
	@$(MAKE) -f GNUmakefile.bank_client all

project_name_list:
	@echo bank_IDL
	@echo bank_client
	@echo bank_server
