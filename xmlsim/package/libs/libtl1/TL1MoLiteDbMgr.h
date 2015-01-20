/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:20:14  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: MoLiteDbMgr.H 
* Type: Include File
* Originator: $Author:   dtchalak  $
* File: $Workfile:   MoLiteDbMgr.h  $
* Version: $Revision:   1.5  $
* Description: 
* Gui specific database manager 
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/cm/db/inc/MoLiteDbMgr.h_v  $
* 
*    Rev 1.5   Oct 27 1999 14:20:14   dtchalak
* Query Interface updates.
* 
*    Rev 1.4.1.0   Sep 20 1999 16:01:18   dtchalak
* No change.
* 
*    Rev 1.4   02 Sep 1999 15:34:20   dtchalak
* New interfaces support added.
* 
*    Rev 1.3   21 Jul 1999 10:22:24   dtchalak
* Multiple Ne support included.
* 
*    Rev 1.2   14 Jun 1999 16:07:44   dtchalak
* Updates related to new CM.idl and other details.
* 
*    Rev 1.1   04 May 1999 18:56:54   dtchalak
* Comments added.
* 
*    Rev 1.0   05 Apr 1999 12:40:46   bacuff
*  
*
* File Dependencies: 
* 
************************************************************************************************/

#ifndef _TL1MoLiteDbMgr_H__
#define _TL1MoLiteDbMgr_H__



#include "FWComponent.h"
#include "BaseCmdInfo.h"

#include <ospace/thread.h>


#include <string>
#include <map>
using namespace std;



namespace EMS_FW
{
class BaseCmdInfo;
class EventHandeler;
class BaseCmdInfoList;
}
//
// This class extends DbMgr class and is responsible for
// Creating, deleting and accessing persistent MoLite objects from the MoUtil database.
// Creating and deleting associations to store persistent MoLite objects and
//  Inserting and removing persistent MoLite objects into/from these associations.
//
//##ModelId=3C9A867603D8
class TL1MoLiteDbMgr : public EMS_FW::FWComponent
{

public:

    //##ModelId=3C9A8677004C
    static const string className;

    //##ModelId=3C9A867700FE
    static TL1MoLiteDbMgr* getInstance(const string& neId = "", const string& name = "");

    //##ModelId=3C9A867700ED
    TL1MoLiteDbMgr()
    {}
    ;

    TL1MoLiteDbMgr(const char* dbName, const string& neId, bool coldStart = false);

    // Get the single instance of MoDbMgr

    //##ModelId=3C9A867700EB
    virtual ~TL1MoLiteDbMgr();
    // The virtual destructor.


    //##ModelId=3C9A8677009C
    void abortUpdates(const EMS_FW::BaseCmdInfo* info);
    // Notification to the DataBase to notify all database Listeners
    // that transaction was aborted.


    //##ModelId=3C9A8677009A
    void commitUpdates(const EMS_FW::BaseCmdInfo* info);
    // Notification to the DatBase to notify all database Listeners that
    // transaction was commited.

    //##ModelId=3C9A86770093
    void notifyListeners(const EMS_FW::BaseCmdInfo* info);

    void notifyListeners(const EMS_FW::BaseCmdInfoList* infoList);

    // Notify all MoDb listeners for specified event

    //##ModelId=3C9A86770092
    void invalidateDb();
    // Put database in invalid state, mean that database content is not
    // synchronized with Agent.

    //##ModelId=3C9A86770091
    bool isDbEmpty();
    // Return true if database is empty no MangedObjects.

    //##ModelId=3C9A86770090
    void validateDb();
    // Put database in valid state, mean that database state is synchronized
    // with Agent.

    //##ModelId=3C9A86770089
    bool isValid();
    // Return true if database is in valid state.


    //##ModelId=3C9A8677007E
    bool getValidState();
    // Return value of the valid state for the elements in database.

    //##ModelId=3C9A8677007C
    void setValidState(bool state);
    // Set Valid state for the elements in the database.

    //##ModelId=3C9A86770075
    void setAutodiscoverMode(bool mode);
    // Put database in on/off autodiscover mode.

    //##ModelId=3C9A86770074
    bool isAutodiscoverMode();
    // Return true if database is in autodiscover mode.

    //##ModelId=3C9A86770072
    void registerListener(EMS_FW::EventHandeler* handeler);
    // Register listener to the database events.'

    //##ModelId=3C9A86770069
    void unregisterListener(EMS_FW::EventHandeler* handeler);

    //##ModelId=3C9A86770061
    void _setInterrupted(bool interrupted);

    bool createEntity(const EMS_FW::BaseCmdInfo* info);
	
	bool createRamanEntity(const EMS_FW::BaseCmdInfo* info);

    bool deleteEntity(const EMS_FW::BaseCmdInfo* info);
	
	bool deleteRamanEntity(const EMS_FW::BaseCmdInfo* info);




protected:


    //##ModelId=3C9A8677005E
    virtual void initialize(bool coldStart);
    // Performs the initialization for this class.


private:

    //##ModelId=3C9A86770041
    bool _autodiscoverMode;

    //##ModelId=3C9A86770040
    bool _validState;

    //##ModelId=3C9A8677003B
    bool _interrupted;

    typedef osi_sem_lock<osi_mutex_semaphore> ListenerLock;

    //##ModelId=3C9A86770038
    osi_mutex _listenerMutex;

    //##ModelId=3C9A8677002E
    list<EMS_FW::EventHandeler*> _listeners;


    //##ModelId=3C9A86770024
    string _neId;
    // Name of the Utility Object used to store state of MoDatabase.

    typedef multimap<string, EMS_FW::BaseCmdInfo*> maptype;
    typedef maptype::value_type mapValueType;
    maptype _orphanMap;


    bool _updateRelations(EMS_FW::BaseCmdInfo* info);

    void _assignNotAssignedObjects(const EMS_FW::BaseCmdInfo* info);


    //##ModelId=3C9A86770056
    void _notifyListeners(const EMS_FW::BaseCmdInfo* info);

    void _notifyListeners(const EMS_FW::BaseCmdInfoList* infoList);


    static const string _TL1MoLiteRoot;


};




#endif // _TL1MoLiteDbMgr_H__

