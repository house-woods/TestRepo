/*******************************************************************************
* COPYRIGHT 1999 - 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: <TL1AutodiscoveryCmd> 
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1AutodiscoveryCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Autodiscover command.
* 
* Revision History:
*
* File Dependencies: 
* <List of include files this file requires> 
*******************************************************************************/



#ifndef _TL1AutodiscoveryCmd_H_
#define _TL1AutodiscoveryCmd_H_


#include "BaseCmd.h"
#include <map>
#include <list>

#include "CmAutodiscoveryCmd.h"


class TL1MoLiteDbMgr;

namespace EMS_FW
{

class BaseCmdInfo;


// Class responsible to synchronize Managed Object database with
// the agent.It is used two ways of autodiscovery. At first it is try
// to resynchronize using synchronize interface that read missing events from a
// log Object stored in the agent. If this could not be complited it read all
// elements directly from the agent and update Managed Object database. During
// the autodiscovery operation it put MO database in autodiscover and interrupted
// mean/corrupted/ state. It is also responsible to maintain and update valid flag
// that represent the state of every element in MO database. If element is not
// resynchronized yet this flag is invalid and after it has been synchronized
// this flag is set to valid state. It is used because of the assumption that
// full scope autodiscovery may take hours to synchronize entire database. During
// this time if the client access elements the server may distinguish which elements
// are valid and get them directly from the database and which are invalid need to
// be get from the agent.Class is currently supporting CM_AUTODISCOVERY and
// CM_FULL_AUTODISCOVERY. Difference is that CM_FULL_AUTODISCOVERY is erasing all
// elements from MO database before start autodiscovery.

class TL1AutodiscoveryCmd : public CmAutodiscoveryCmd
{
public:

    TL1AutodiscoveryCmd(EMS_FW::BaseCmdInfo* info);

    const string& getClassName() const;
    // Helper function return 'CmAutodiscoverCmd'.

protected:


    virtual void _execute();

    virtual void _discover(bool newDb);
    // Synchronize database using full scope autodiscover.
    // Read all elements from the agent.

    virtual void _discoverOneFilter(const string& filter, int objsPerTxn, const string& discoverMethod);
    // Discover the element using the specified filter.

    virtual void _processDiscoveredObjects(list<EMS_FW::BaseCmdInfo*>& objList, const string& tl1Cmd, const string& moc);


};

};

#endif

