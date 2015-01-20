/***********************************************************************************************
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1Defines
* Type: H
* Originator: $Author $
* File: $Workfile$
* Version: $Revision$
* Description: 
*        Contains TL1 names definitions.
* 
* Revision History:
*
* $Log:   
   
      Rev 1.1   Dec 12 2002            lzou
      Rev 1.0   Oct 27 2002 14:32:48   dtchalak and jhu
*
* $Log$
**********************************************************************************************/

#ifndef _TL1DEFINES_H_
#define _TL1DEFINES_H_
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

#include <string>


class TL1DEF
{
public:

// Follows are used by OTS CM, Applications will define its own MOC
// Should move to OTSDefines in future --- Li Zou
// Managed Object Class Definitions
static	const string MOC_TL1EQUIPMENT;		  // Managed Object Class for equipment such as CCT or PLM.
static	const string MOC_TL1FACILITYPROTGROUP;	  // Managed Object Class for Facility Protection Group.
static  const string MOC_TL1MANAGEDELEMENT;       // Managed Object Class for Managed Element.
static  const string MOC_TL1OPTICALFACILITY;      // Managed Object Class for Facility.
static  const string MOC_TL1EQUIPMENTHOLDER;      // Managed Object Class for equipment holder.
static  const string MOC_TL1TERMINATION;          // Managed Object Class for Termination.
static  const string MOC_TL1CROSSCONNECT;	  // Managed Object Class for CrossConnection.
static  const string MOC_TL1THRESHOLDCROSSALERTS; // Managed Object Class for TCA.
// ***************************************************************************
static const string MOC_TL1SHELF;            // Managed Object Class for Shelf
static const string MOC_TL1USER;

static  const string DELIMITER_HYPHEN;
static  const string NAME_DELIMITER ;		  // Delimiter to separate type name from attr name.

static const string AS_SINGLE_DEFAULT; // default single association

static const string EVT_DBCHG;               // Event Type: DbChg
static const string EVT_EVT;               // Event Type: EVT
static const string EVT_SESSION;             // Event Type: EVT SESSION

static const string EVT_USER_CREATED;
static const string EVT_USER_DELETED;
static const string EVT_USER_MODIFIED;

static const string RES_COMPLETECODE_COMP;	// Completion Code: Complete
static const string RES_COMPLETECODE_DENY;	// Completion Code: Deny
static const string RES_COMPLETECODE_PRTL;	// Completion Code: PRTL

static const string AN_AID;			// Aid for an TL1 Object
static const string AN_AID_CAPITAL;			// Aid for an TL1 Object
static const string AN_CMDCODE;			// Command Name in Response or Autonomous Message
static const string AN_SPECIFICBLOCK;		// Specific Block in Response or Autonomous Message
static const string AN_COMMONBLOCK;		// Common Block in Response or Autonomous Message
static const string AN_STATEBLOCK;		// State Block in Response or Autonomous Message
static const string AN_CONDDESCBLOCK;		// CondDescription Block in REPT EVT
static const string AN_DBCHGHEADER;		// DBCHG Event Header 
static const string AN_DBCHG_TIME;		// Time field in DBCHG event
static const string AN_DBCHG_DATE;		// DATE field in DBCHG event

static const string AN_PRISTATE;		// Primary State
static const string AN_SECSTATE;		// Second State
static const string AN_PRISTATE_CAPITAL;		// Primary State
static const string AN_SECSTATE_CAPITAL;		// Second State

static const string AN_TYPE;           // Entity Type


static const string AV_PRISTATE_IS;		// In Service for Primary State
static const string AV_PRISTATE_OOS;		// Out of Service for Primary State

static const string CMD_RTRV_EQPT;		// Cmd for retreve equipment
static const string CMD_ED_EQPT;		// Cmd for edit equipment

static const string AT_GETALLTL1USERS;
static const string AT_UNREGISTERSMNOTIFICATIONS;

static const string Q3TL1COMMANDRESULT;
static const string Q3TL1DENYCODE;
static const string Q3TL1DENYTEXT;
static const string Q3TL1PRTLMSG;
static const string Q3TL1MESSAGE;
static const string Q3TL1MSGTYPE;
static const string Q3TL1MSGDATE;
static const string Q3TL1MSGTIME;

};


#endif

