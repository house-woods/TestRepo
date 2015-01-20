
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:24  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: AgentCmdInfo
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   AgentCmdInfo.cpp  $
* Version: $Revision:   1.6  $
* Description: 
* Data Holder for communications with Agent.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/src/AgentCmdInfo.cpv  $
   
      Rev 1.6   Oct 27 1999 14:35:24   dtchalak
   Query Interface updates.
   
      Rev 1.5.1.0   Sep 20 1999 16:17:14   dtchalak
   Just to create branch.
   
      Rev 1.5   02 Sep 1999 15:43:06   dtchalak
   New interface support added.
   
      Rev 1.4   21 Jul 1999 10:28:02   dtchalak
   Multiple Ne support included.
   
      Rev 1.3   23 Jun 1999 16:47:18   dtchalak
    
   
      Rev 1.2   14 Jun 1999 16:14:38   dtchalak
   Updates related to new CM.idl and other details.
   
      Rev 1.1   04 May 1999 19:02:30   dtchalak
   Comments added.
   
      Rev 1.0   05 Apr 1999 12:33:44   bacuff
    
 * 
 *    Rev 1.1   04 Feb 1999 14:40:12   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 12:24:34   dtchalak
 *  
**********************************************************************************************/

#include "AgentCmdInfo.h"

using namespace EMS_FW;

const string AgentCmdInfo::CM_AGENT_SET = "CM_AGENT_SET";
const string AgentCmdInfo::CM_AGENT_GET = "CM_AGENT_GET";
const string AgentCmdInfo::CM_AGENT_CREATE = "CM_AGENT_CREATE";
const string AgentCmdInfo::CM_AGENT_DELETE = "CM_AGENT_DELETE";
const string AgentCmdInfo::CM_AGENT_ACTION = "CM_AGENT_ACTION";
const string AgentCmdInfo::CM_GET_RESPONSE = "CM_GET_RESPONSE";
const string AgentCmdInfo::CM_SET_RESPONSE = "CM_SET_RESPONSE";
const string AgentCmdInfo::CM_ACTION_RESPONSE = "CM_ACTION_RESPONSE";
const string AgentCmdInfo::CM_CREATE_RESPONSE = "CM_CREATE_RESPONSE";
const string AgentCmdInfo::CM_DELETE_RESPONSE = "CM_DELETE_RESPONSE";
const int AgentCmdInfo::NO_OPERATIONTIMEOUT = -1;
const int AgentCmdInfo::UNDEFINED_TIMEOUT = 0;

AgentScope::AgentScope()
{
    _level = 0;
    _type = AgentScope::undefinedScope;
}

AgentScope::AgentScope(ScopeType type, int level) : _type(type) , _level(level)
{}


void AgentScope::setType(ScopeType type)
{
    _type = type;
}



void AgentScope::setLevel(int level)
{
    _level = level;
}

AgentScope::ScopeType AgentScope::getType() const
{
    return _type;
}

int AgentScope::getLevel() const
{
    return _level;
}

AgentFilter::AgentFilter()
{}


AgentFilter::AgentFilter(const string& condition) : _condition(condition)
{}

void AgentFilter::setCondition(const string& condition)
{
    _condition = condition;
}

const string& AgentFilter::getCondition() const
{
    return _condition;
}

AgentCmdInfo::AgentCmdInfo()
{
    _attribMap = NULL;
    _timeout = UNDEFINED_TIMEOUT;
}

AgentCmdInfo::AgentCmdInfo(const string& moc,
                           const string& moi,
                           const string& type) :
        BaseCmdInfo(moc, moi, type)
{
    _attribMap = NULL;
    _timeout = UNDEFINED_TIMEOUT;
}


AgentCmdInfo::~AgentCmdInfo()
{
    if (_attribMap)
        delete _attribMap;
}



void AgentCmdInfo::setTransparentAttributes(attribMapType* attribMap)
{
    _attribMap = attribMap;
}


bool AgentCmdInfo::isTransparentAttribute(const string& name)
{
    if (_attribMap == NULL)
        return false; // no transparent attributes are set
    if (_attribMap->size() == 0)
        return true; // all attributes are treated as transparent
    attribMapType::iterator p = _attribMap->find(name);
    if (p == _attribMap->end()) // name is not in a map
        return false;
    return true;
}

void AgentCmdInfo::setScope(const AgentScope& sc)
{
    _scope = sc;
}

const AgentScope& AgentCmdInfo::getScope() const
{
    return _scope;
}

void AgentCmdInfo::setFilter(const AgentFilter& filter)
{
    _filter = filter;
}


const AgentFilter& AgentCmdInfo::getFilter() const
{
    return _filter;
}

void AgentCmdInfo::reinitialize(const BaseCmdInfo* orig)
{
    BaseCmdInfo::reinitialize(orig);
    AgentCmdInfo* info = (AgentCmdInfo*) orig;
    setFilter(info->getFilter());
    setScope(info->getScope());
    _attribMap = info->_attribMap;
}




BaseCmdInfo* AgentCmdInfo::clone() const
{
    BaseCmdInfo* info = new AgentCmdInfo();
    info->reinitialize(this);
    return info;
}





