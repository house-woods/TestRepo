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

all: bank_IDL bank_server bank_client chat_IDL chat_client chat_server echo_IDL echo_server echo_client grid_IDL grid_client grid_server Simple_Time_Date_Lib Simple_Time_Date_Client Simple_Time_Date_Server time_IDL time_client time_server

REMAINING_TARGETS := $(subst all, , $(TARGETS_NESTED:.nested=)) $(CUSTOM_TARGETS)

$(REMAINING_TARGETS):
	@cd bank && $(MAKE) -f GNUmakefile.bank_IDL $(@)
	@cd bank && $(MAKE) -f GNUmakefile.bank_server $(@)
	@cd bank && $(MAKE) -f GNUmakefile.bank_client $(@)
	@cd chat && $(MAKE) -f GNUmakefile.chat_IDL $(@)
	@cd chat && $(MAKE) -f GNUmakefile.chat_client $(@)
	@cd chat && $(MAKE) -f GNUmakefile.chat_server $(@)
	@cd echo && $(MAKE) -f GNUmakefile.echo_IDL $(@)
	@cd echo && $(MAKE) -f GNUmakefile.echo_server $(@)
	@cd echo && $(MAKE) -f GNUmakefile.echo_client $(@)
	@cd grid && $(MAKE) -f GNUmakefile.grid_IDL $(@)
	@cd grid && $(MAKE) -f GNUmakefile.grid_client $(@)
	@cd grid && $(MAKE) -f GNUmakefile.grid_server $(@)
	@cd time-date && $(MAKE) -f GNUmakefile.Simple_Time_Date_Lib $(@)
	@cd time-date && $(MAKE) -f GNUmakefile.Simple_Time_Date_Client $(@)
	@cd time-date && $(MAKE) -f GNUmakefile.Simple_Time_Date_Server $(@)
	@cd time && $(MAKE) -f GNUmakefile.time_IDL $(@)
	@cd time && $(MAKE) -f GNUmakefile.time_client $(@)
	@cd time && $(MAKE) -f GNUmakefile.time_server $(@)

.PHONY: bank_IDL
bank_IDL:
	@cd bank && $(MAKE) -f GNUmakefile.bank_IDL all

.PHONY: bank_server
bank_server: bank_IDL
	@cd bank && $(MAKE) -f GNUmakefile.bank_server all

.PHONY: bank_client
bank_client: bank_IDL bank_server
	@cd bank && $(MAKE) -f GNUmakefile.bank_client all

.PHONY: chat_IDL
chat_IDL:
	@cd chat && $(MAKE) -f GNUmakefile.chat_IDL all

.PHONY: chat_client
chat_client: chat_IDL
	@cd chat && $(MAKE) -f GNUmakefile.chat_client all

.PHONY: chat_server
chat_server: chat_IDL chat_client
	@cd chat && $(MAKE) -f GNUmakefile.chat_server all

.PHONY: echo_IDL
echo_IDL:
	@cd echo && $(MAKE) -f GNUmakefile.echo_IDL all

.PHONY: echo_server
echo_server: echo_IDL
	@cd echo && $(MAKE) -f GNUmakefile.echo_server all

.PHONY: echo_client
echo_client: echo_IDL echo_server
	@cd echo && $(MAKE) -f GNUmakefile.echo_client all

.PHONY: grid_IDL
grid_IDL:
	@cd grid && $(MAKE) -f GNUmakefile.grid_IDL all

.PHONY: grid_client
grid_client: grid_IDL
	@cd grid && $(MAKE) -f GNUmakefile.grid_client all

.PHONY: grid_server
grid_server: grid_IDL grid_client
	@cd grid && $(MAKE) -f GNUmakefile.grid_server all

.PHONY: Simple_Time_Date_Lib
Simple_Time_Date_Lib:
	@cd time-date && $(MAKE) -f GNUmakefile.Simple_Time_Date_Lib all

.PHONY: Simple_Time_Date_Client
Simple_Time_Date_Client: Simple_Time_Date_Lib
	@cd time-date && $(MAKE) -f GNUmakefile.Simple_Time_Date_Client all

.PHONY: Simple_Time_Date_Server
Simple_Time_Date_Server:
	@cd time-date && $(MAKE) -f GNUmakefile.Simple_Time_Date_Server all

.PHONY: time_IDL
time_IDL:
	@cd time && $(MAKE) -f GNUmakefile.time_IDL all

.PHONY: time_client
time_client: time_IDL
	@cd time && $(MAKE) -f GNUmakefile.time_client all

.PHONY: time_server
time_server: time_IDL time_client
	@cd time && $(MAKE) -f GNUmakefile.time_server all

project_name_list:
	@echo bank_IDL
	@echo bank_client
	@echo bank_server
	@echo chat_IDL
	@echo chat_client
	@echo chat_server
	@echo echo_IDL
	@echo echo_client
	@echo echo_server
	@echo grid_IDL
	@echo grid_client
	@echo grid_server
	@echo Simple_Time_Date_Client
	@echo Simple_Time_Date_Lib
	@echo Simple_Time_Date_Server
	@echo time_IDL
	@echo time_client
	@echo time_server
