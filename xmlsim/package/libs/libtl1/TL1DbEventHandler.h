/*******************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:28:48  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: TL1DbEventHandler
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1DbEventHandler.h  $
* Version: $Revision:   1.5  $
* Description: 
* Process Notifications to update Mo Database
* 
* Revision History:
*
*
* File Dependencies: 
* <List of include files this file requires> 
*******************************************************************************/

#ifndef _TL1DbEventHandler_H_
#define _TL1DbEventHandler_H_


#include "DbEventHandeler.h"



namespace EMS_FW
{

class BaseCmdInfo;
class DbMgr;
}

// Class listener for agent events and sending this events to Managed Object database 
// for database updates.

class TL1DbEventHandler : public EMS_FW::DbEventHandeler
{
public:

	TL1DbEventHandler();

	~TL1DbEventHandler();

	virtual EMS_FW::DbMgr* getDbMgr();

	EMS_FW::DbEventHandeler* clone();

	virtual bool handleEvent(const EMS_FW::BaseCmdInfo* info);
	
	void handleCreateEvent(const EMS_FW::BaseCmdInfo* info);
	// Send create Event to Managed Object database.

	void handleDeleteEvent(const EMS_FW::BaseCmdInfo* info);
	// Send delete event to Managed Object database.

	void handleAVCEvent(const EMS_FW::BaseCmdInfo* info);
	// Send attribute value change event to Managed Object database.

   // No implementation needed for the autodiscovery list versions.
	virtual bool handleEvent(const EMS_FW::BaseCmdInfoList* infoList);
	void handleCreateEvent(const EMS_FW::BaseCmdInfoList* infoList) {}
	void handleDeleteEvent(const EMS_FW::BaseCmdInfoList* infoList) {}
	void handleAVCEvent(const EMS_FW::BaseCmdInfoList* infoList) {}

	void handleSCEvent(const EMS_FW::BaseCmdInfo* info);
	// Send state change event to Managaed Object database.

	virtual void abortUpdates(const EMS_FW::BaseCmdInfo* info);
	// Notification to the DataBase to notify all database Listeners 
	// that transaction was aborted.


	virtual void commitUpdates(const EMS_FW::BaseCmdInfo* info);
	// Notification to the DatBase to notify all database Listeners that 
	// transaction was commited.



private:
	
	void setAttributes(const EMS_FW::BaseCmdInfo* info);
	// Invoke set attributes on Managed Object database.

};



#endif
