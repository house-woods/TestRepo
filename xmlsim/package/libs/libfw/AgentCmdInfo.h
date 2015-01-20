
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:30  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: AgentCmdInfo
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   AgentCmdInfo.h  $
* Version: $Revision:   1.6  $
* Description: 
* Data Holder for communications with Agent.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/inc/AgentCmdInfo.h_v  $
 * 
 *    Rev 1.6   Oct 27 1999 14:35:30   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.5.1.0   Sep 20 1999 16:17:26   dtchalak
 * Just to create branch.
 * 
 *    Rev 1.5   02 Sep 1999 15:42:12   dtchalak
 * New interface support added.
 * 
 *    Rev 1.4   21 Jul 1999 10:27:02   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.3   23 Jun 1999 16:46:54   dtchalak
 *  
 * 
 *    Rev 1.2   14 Jun 1999 16:13:34   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.1   04 May 1999 19:01:32   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 12:33:52   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 14:40:12   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 12:24:34   dtchalak
 *  
**********************************************************************************************/


#ifndef _AgentCmdInfo_H_
#define _AgentCmdInfo_H_


#include "BaseCmdInfo.h"
#include <map>



namespace EMS_FW
{


// Class representing Scope for the agent operations.
class AgentScope
{
public:
	enum ScopeType 
	{
		baseObject,
		firstLevel,
		wholeSubtree,
		individualLevel,
		baseToNthLevel,
		undefinedScope
	};
	// Predefined scope types.

	AgentScope(); 
	// Default Constructor.

	AgentScope(ScopeType type, int level = 0); 
	// Constructor accepting Scope type and level as arguments.
	
	void setType(ScopeType type); 
	void setLevel(int level); 
	ScopeType getType() const; 
	int getLevel() const; 

private:

	ScopeType _type;
	int _level;
};

// Generic class encapsulating Agent Filter. 
// Filter is used to specify filter criteria when operation on the agent
// is involved.
class AgentFilter
{
public:

		AgentFilter(); 
		AgentFilter(const string& condition);
		
		void setCondition(const string& condition); 
		
		const string& getCondition() const; 
		

private:

	string _condition;

};

// Base data holder for operations invoked on the agent.
// This class contain all information necessary for invoke operation
// on the agent.
class AgentCmdInfo : public BaseCmdInfo
{
public:

	typedef map<string,string> attribMapType;

	static const string CM_AGENT_SET;
	static const string CM_AGENT_GET;
	static const string CM_AGENT_CREATE;
	static const string CM_AGENT_DELETE;
	static const string CM_AGENT_ACTION;
	static const string CM_GET_RESPONSE;
	static const string CM_SET_RESPONSE;
	static const string CM_ACTION_RESPONSE;
	static const string CM_CREATE_RESPONSE;
	static const string CM_DELETE_RESPONSE;
	static const int	NO_OPERATIONTIMEOUT;
	static const int    UNDEFINED_TIMEOUT;
	// Defines of agent operations.
	
	AgentCmdInfo();

	AgentCmdInfo(const string& moc, const string& moi, const string& type); 

	~AgentCmdInfo();


	void setScope(const AgentScope& sc); 

	const AgentScope& getScope() const; 

	void setFilter(const AgentFilter& filter); 

	const AgentFilter& getFilter() const; 
	
	void setTransparentAttributes(attribMapType* map);
	// Map contains the attributes that need to be transparent on getNextElement().
	// They want be converted using generated library but passes back as a ASN1HolderType.
	//AgentCmdInfo is taking ownership of the attribMap. Map will be deleted at the destruction
	//of the command info.
	bool isTransparentAttribute(const string& name);

	void setTimeOut(int timeout) { _timeout = timeout; }

	int getTimeOut() const { return _timeout; }

	virtual void reinitialize(const BaseCmdInfo* orig);
	// Reinitialize the AgentCmdInfo with the new values.

	virtual BaseCmdInfo* clone() const;
	// Create copy of the original AgentCmdInfo.


private:

	attribMapType* _attribMap;

protected:

	AgentFilter _filter;
	// Filter of the agent operation.

	AgentScope       _scope;
	// Scope of the agent operation.

	int _timeout;
	// Time out in seconds for the operation to complete.

};


};


#endif
