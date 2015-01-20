/***********************************************************************************************
* COPYRIGHT 1999 - 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: AgentCmd
* Type: H 
* Originator: 
* File: AgentCmd.h
* Version: 
* Description: 
* Generic Agent Commands
* 
* Revision History:
*
**********************************************************************************************/


#ifndef _AGENTCMD_H_
#define _AGENTCMD_H_

#include "BaseCmd.h"


namespace EMS_FW
{

class BaseCmdInfo;


// Abstract class defining base interfaces used for implementation of
// Agent command. Agent command is a command used to comunicate with Network Element.
class AgentCmd : public BaseCmd
{

public:
	
	AgentCmd();
	// Default Constructor.

	AgentCmd(EMS_FW::BaseCmdInfo* info);
	// Constructor with argument specified specific operation to be performed on 
	// the Network Element.

	~AgentCmd();
	// Destructor.

	virtual bool hasMoreElements() = 0;
	// Return true if there are more elements in linked reply.
	
	virtual bool getNextElement(EMS_FW::BaseCmdInfo* info) = 0;
	// Method used to access elements in linked reply.

	virtual bool getNextResponse() = 0;

	virtual void reinitializeWithSameConnection(EMS_FW::BaseCmdInfo* info);
	// Reinitialize the same command with different attributes 
	// using the same connection.

protected:

	bool _scoped;
	// true if operation is scope operation.

	virtual void _initialize();
	// Base initlialization of the agent command.

};


// Class specific implementation of the AgentCmd that is not tied to 
// any specific protocol used to comunicate with Network Element.
// It implement the logic to find out appropriate protocol to be used to 
// contact the Network Element.
class GenericAgentCmd : public AgentCmd
{
public:

	~GenericAgentCmd();
	// Destructor.

	GenericAgentCmd(EMS_FW::BaseCmdInfo* info);
	// Constructor with argument specified specific operation to be performed on 
	// the Network Element.

	CmdResult execute();
	// Execute Command specified in constructor Arguments.

	bool hasMoreElements();
	// Return true if there are more elements in linked reply.
	
	bool getNextElement(EMS_FW::BaseCmdInfo* info);
	// Method used to access elements in linked reply.

	void reinitializeWithSameConnection(EMS_FW::BaseCmdInfo* info);
	// Reinitialize the same command with different attributes 
	// using the same connection.

	virtual bool getNextResponse();

protected:

	AgentCmd* _cmd;
	// Pointer to protocol specific command used from generic command.
};

};

#endif
