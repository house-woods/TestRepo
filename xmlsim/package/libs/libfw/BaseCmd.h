/***********************************************************************************************
* COPYRIGHT 1999 - 2004 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: Base Command
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Base Command.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/inc/BaseCmd.h_v  $
 * 
 *    Rev 1.5   Oct 27 1999 14:35:38   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.4.1.0   Sep 20 1999 16:17:48   dtchalak
 * Just to create branch.
 * 
 *    Rev 1.4   02 Sep 1999 15:42:22   dtchalak
 * New interface support added.
 * 
 *    Rev 1.3   21 Jul 1999 10:27:14   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.2   14 Jun 1999 16:13:46   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.1   04 May 1999 19:01:44   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 12:34:48   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 14:41:20   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 12:24:58   dtchalak
 *  
 * 
 *    Rev 1.2   Nov 09 1998 16:58:30   dtchalak
 *  
 * 
 *    Rev 1.1   Nov 05 1998 11:20:50   dtchalak
 *  
 * 
 *    Rev 1.0   Oct 16 1998 17:30:32   dtchalak
 *  
**********************************************************************************************/





#ifndef _BaseCmd_H_
#define _BaseCmd_H_

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
//#include "CmGlobals.h"
#include <string>
#include <memory>




namespace EMS_FW
{

class BaseCmdInfo;

// Class data holder containing minimum information about command complition.
// If the user need to get back some parameters from a command 
// it need to use some of the methods specified at the command level
// like 'getNextElement'.
class CmdResult
{
public:

	CmdResult(); 
	CmdResult(const bool& stat, const string& result, const long& timestamp = 0); 
	~CmdResult(); 
	bool getMultiPartFlag() const;
	// Return true if command response is multi-part.

	bool getStatus() const;
	// Return true if command finish succesfully.

	long getTimeStamp() const; 
	// Return time Stamp of the command response.

	const string& getResultInfo() const; 
	// Return description of the command complition.

	void setStatus(bool stat);
	// Set status for command complition.

	void setMultiPartFlag(bool multiPart);
	// Set status for command complition.
	
	void setTimeStamp(long timestamp); 
	// Set time stamp included in command response.

	void setResultInfo(const string& result); 
	// Set description of the command completion.

	void setErrCode(long errCode) { _errCode = errCode; }
	// Set Error Code for the operation.

	long getErrCode() const { return _errCode; }

	void setStrErrCode(const string& strErrCode);
	// Set Error Code for the operation.

	const string& getStrErrCode() const { return _strErrCode; }

protected:
 
	bool stat_;
	// Flag representing command complition.

	bool _multiPart;
	// Flag representing Multi Part Response.
        
	long _errCode;
	// Error Code for the operation.

	long timestamp_;
	// Time stamp representing command response timestamp like response timestamp from
	// the agent.

	string result_;
	// Description of the command complition.

	string _strErrCode;
	//Error Code in string format

};

// Base abstract command class implementation of Command Design Patern.
class BaseCmd
{
public:
	
	virtual ~BaseCmd();

	virtual CmdResult execute() = 0;

	BaseCmdInfo* getInfo();
	// Used just for test purposes.

	void cancelRequest();

	bool isCanceled();

protected:
	
	BaseCmd();
	BaseCmd(EMS_FW::BaseCmdInfo* info); 

	auto_ptr<BaseCmdInfo> info_;
	// Pointer to the command parameters.

	bool _cancelRequest;
	bool _canceled;

};

};  // Bottom of "namespace EMS_FW"

#endif



