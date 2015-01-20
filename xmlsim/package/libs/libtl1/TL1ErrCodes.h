/***********************************************************************************************
 * COPYRIGHT 2001 BY TELLABS OPERATIONS, INC 
 * ALL RIGHTS RESERVED 
 * NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
 * THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
 * 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
 ************************************************************************************************
 * 
 * Name: Tl1ErrorCodes.h
 * Description: 
 *   Contains Tl1 error code definitions
 **********************************************************************************************/

#ifndef _TL1ERRORCODES_H_
#define _TL1ERRORCODES_H_

#include <string>

namespace TL1_MA
{

static const string TL1_EQWT = "EQWT";		//Specified equipment does not match command
static const string TL1_IBEX = "IBEX";		//The number of parameter blocks in Command exceeds limit
static const string TL1_IBMS = "IBMS";		//The number of parameter blocks in Command is too few
static const string TL1_IBNC = "IBNC";		//The type of parameter block is inconsistent with definition
static const string TL1_ICNC = "ICNC";		//The command verb and modifier are mutually exclusive
static const string TL1_ICNV = "ICNV";		//The command verb or its modifier are invalid
static const string TL1_IDNC = "IDNC";		//Input data in the command is inconsistent
static const string TL1_IDNV = "IDNV";		//An invalid parameter was used in the command
static const string TL1_IDRG = "IDRG";		//Parameter value inconsistent with its defined type
static const string TL1_IISP = "IISP";		//The command syntax is incorrect
static const string TL1_IITA = "IITA";		//The target Identifier field is invalid
static const string TL1_IPEX = "IPEX";		//A valid parameter is incorrectly used
static const string TL1_IPMS = "IPMS";		//A required parameter was ommitted
static const string TL1_IPNC = "IPNC";		//Two valid parameters are mutually exclusive
static const string TL1_IPNV = "IPNV";		//A parameter in the command is invalid
static const string TL1_ISCH = "ISCH";		//A character in the command violates syntax rules
static const string TL1_ISPC = "ISPC";		//A punctuation mark in the command violates syntax rules
static const string TL1_PICC = "PICC";		//Insufficient privilege to execute command
static const string TL1_PIOC = "PIOC";		//The provided password is incorrect
static const string TL1_PIUI = "PIUI";		//The user id is invalid
static const string TL1_RCBY = "RCBY";		//All circuits are currently in use
static const string TL1_RTBY = "RTBY";		//The TAP specification in the command currently in use
static const string TL1_RTEN = "RTEN";		//The TAP specification in the command is invalid
static const string TL1_SAAS = "SAAS";		//The specified object entity is already assigned
static const string TL1_SCNA = "SCNA";		//The executing command does not support command termination
static const string TL1_SCNF = "SCNF";		//The designated command could not be found in queue/executing
static const string TL1_SDNC = "SDNC";		//Input data not consistent with NE database
static const string TL1_SDNR = "SDNR";		//Attempt to retrieve log file during synchronization
static const string TL1_SNVS = "SNVS";		//Invalid state status of the specified entity
static const string TL1_SROF = "SROF";		//A software or hardware problem occurred
static const string TL1_SSNC = "SSNC";		//The supported entity data is not consistent
static const string TL1_SSNV = "SSNV";		//The supported entity is not in a valid state to execute command
static const string TL1_SSRE = "SSRE";		//System resources were exceeded
static const string TL1_SSTP = "SSTP";		//A software or hardware problem occurred


//Error Codes

static const int TL1_EQWT_ERRORCODE = 710;		//Specified equipment does not match command
static const int TL1_IBEX_ERRORCODE = 711;		//The number of parameter blocks in Command exceeds limit
static const int TL1_IBMS_ERRORCODE = 712;		//The number of parameter blocks in Command is too few
static const int TL1_IBNC_ERRORCODE = 713;		//The type of parameter block is inconsistent with definition
static const int TL1_ICNC_ERRORCODE = 714;		//The command verb and modifier are mutually exclusive
static const int TL1_ICNV_ERRORCODE = 715;		//The command verb or its modifier are invalid
static const int TL1_IDNC_ERRORCODE = 716;		//Input data in the command is inconsistent
static const int TL1_IDNV_ERRORCODE = 717;		//An invalid parameter was used in the command
static const int TL1_IDRG_ERRORCODE = 718;		//Parameter value inconsistent with its defined type
static const int TL1_IISP_ERRORCODE = 719;		//The command syntax is incorrect
static const int TL1_IITA_ERRORCODE = 720;      //The target Identifier field is invalid
static const int TL1_IPEX_ERRORCODE = 721;		//A valid parameter is incorrectly used
static const int TL1_IPMS_ERRORCODE = 722;		//A required parameter was ommitted
static const int TL1_IPNC_ERRORCODE = 723;		//Two valid parameters are mutually exclusive
static const int TL1_IPNV_ERRORCODE = 724;		//A parameter in the command is invalid
static const int TL1_ISCH_ERRORCODE = 725;		//A character in the command violates syntax rules
static const int TL1_ISPC_ERRORCODE = 726;		//A punctuation mark in the command violates syntax rules
static const int TL1_PICC_ERRORCODE = 727;		//Insufficient privilege to execute command
static const int TL1_PIOC_ERRORCODE = 728;		//The provided password is incorrect
static const int TL1_PIUI_ERRORCODE = 729;		//The user id is invalid
static const int TL1_RCBY_ERRORCODE = 730;		//All circuits are currently in use
static const int TL1_RTBY_ERRORCODE = 731;		//The TAP specification in the command currently in use
static const int TL1_RTEN_ERRORCODE = 732;		//The TAP specification in the command is invalid
static const int TL1_SAAS_ERRORCODE = 733;		//The specified object entity is already assigned
static const int TL1_SCNA_ERRORCODE = 734;		//The executing command does not support command termination
static const int TL1_SCNF_ERRORCODE = 735;		//The designated command could not be found in queue/executing
static const int TL1_SDNC_ERRORCODE = 736;		//Input data not consistent with NE database
static const int TL1_SDNR_ERRORCODE = 737;		//Attempt to retrieve log file during synchronization
static const int TL1_SNVS_ERRORCODE = 738;		//Invalid state status of the specified entity
static const int TL1_SROF_ERRORCODE = 739;		//A software or hardware problem occurred
static const int TL1_SSNC_ERRORCODE = 740;		//The supported entity data is not consistent
static const int TL1_SSNV_ERRORCODE = 741;		//The supported entity is not in a valid state to execute command
static const int TL1_SSRE_ERRORCODE = 742;		//System resources were exceeded
static const int TL1_SSTP_ERRORCODE = 743;		//A software or hardware problem occurred


static const int TL1_IP_UNREACHABLE = 709;      // unable to reach the IP or port

// Status/Error Codes
static const string TL1_SUCC = "";			// Null place holder for successful completion.

enum TL1_STATUS_CODES
{
	TL1_CMD_ABORTED = 0,					// Indicates command aborted successfully.
	TL1_CMD_NOT_ABORTED = 1,				// Command cannot be aborted (SCNA above).
	TL1_CMD_NOT_FOUND = 2					// Command not found in queue.
};

}

#endif 
