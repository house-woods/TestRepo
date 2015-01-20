/***********************************************************************************************
* COPYRIGHT $Date:   24 May 2001 10:59:40  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: ComManager.h 
* Type: Include File 
* Originator: $Author: $
* File: $Workfile:   ComManager.h  $
* Version: $Revision:     $
* Description: 
* This is the class definition of ComManager. This is a wrapper class for establishing
* various CORBA connection services.
* 
* Revision History:
*
* $Log:    $
*
* File Dependencies: 
*
************************************************************************************************/
#ifndef __COMM_MANAGER_H__
#define __COMM_MANAGER_H__

#include <string>
#include <map>
#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/orbsvcs/CosNamingS.h"
#include "tao/RTPortableServer/RTPortableServer.h"

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

class ComManager
{
public:

    // instance() -- return the instance.
    static ComManager* instance(bool sslSupportEnabled = true);

    void run() const;

    void initializeORB(int argc, char **argv);
    void initializeORB (int argc, char **argv,
                        const string &namingServiceHostname, const string &namingServicePort,
                        const string &orbConfFileName, const string &orbDebugLevel,
                        const string &orbAdditionalParamaters, const string& orbLogFile);

    void initializeRTORB (int argc, char **argv);
    void initializeRTORB (int argc, char **argv, const string &namingServiceHostname, const string &namingServicePort,
                          const string &orbConfFileName, const string &orbDebugLevel,
                          const string &orbAdditionalParamaters, const string &orbLogFile);

    void terminate();
    void finalize();

    //
    // POA policy abstraction.
    //

    typedef CORBA::ULong Policy_Type;
    typedef CORBA::ULong Policy_Value;
    typedef map<Policy_Type, Policy_Value> Policies;

    // Policy types used when creating new POA for the servant.
    //
    enum PolicyTypes
    {
        thread_policy,
        lifespan_policy,
        object_id_uniqueness_policy,
        id_assignment_policy,
        servant_retention_policy,
        request_processing_policy,
        implicit_activation_policy
    };

    enum ThreadPolicy { orb_ctrl_model, single_thread_model };
    enum LifespanPolicy { transient, persistent };
    enum ObjectIdUniquenessPolicy { unique_id, multiple_id };
    enum IdAssignmentPolicy { user_id, system_id };
    enum ServantRetentionPolicy { retain, non_retain };
    enum RequestProcessingPolicy { use_active_object_map_only, use_default_servant, use_servant_manager };
    enum ImplicitActivationPolicy { implicit_activation, no_implicit_activation };

    // use for one way corba call
    enum SyncScopPolicy{SYNC_NONE, SYNC_WITH_TRANSPORT, SYNC_WITH_SERVER, SYNC_WITH_TARGET};

    PortableServer::POA_ptr create_rtpoa_with_properties(
        const string& poa_name,
        const Policies& policies);

    PortableServer::POA_ptr create_poa_with_properties(
        const string& poa_name,
        const Policies& policies);

    CORBA::Object_ptr activate_object_with_name(
        const PortableServer::POA_ptr servant_poa,
        const string& object_name,
        const PortableServer::Servant servant);

    CORBA::Object_ptr activateObject(
        const PortableServer::POA_ptr servant_poa,
        const PortableServer::Servant servant);

    bool deactivateObject(
        const PortableServer::POA_ptr servant_poa,
        CORBA::Object_ptr object);

    bool deactivateObject(
        const PortableServer::POA_ptr servant_poa,
        PortableServer::Servant servant);

    // Naming Service.
    //
    CORBA::Object_ptr resolveObjectName(const string &object_name, string &error = string (""));
    bool bindObject(CORBA::Object_ptr object, const string &object_name);
    bool unbindObject(const string &object_name);
    bool setRootContext(const string &context_name = "");

    // ORB accessor
    //
    CORBA::ORB_ptr getOrb() const
    {
        return _orb;
    };

    // Set the time out and returns a pointer to the reference with the policy set.
    // Use the returned refernce to invoke the method with the timeout
    CORBA::Object_ptr
    SetTimeOut(CORBA::Object_ptr pObj, unsigned long msecs);

    // Set the sync scope policy for one way call and
    // returns a pointer to the reference with the policy set.
    // Use the returned refernce to invoke the method with the sync scope policy
    CORBA::Object_ptr
    SetSyncScopPolicy(CORBA::Object_ptr pObj, int policyType = SYNC_WITH_TRANSPORT);

    //ping Notification Service. If ping is successful then return true otherwise false
    bool pingNotificationService(const string &notifyName);

private:

    static ComManager* _comMgr;  // Singleton instance

    // Constructor/destructor are private, use instance() and terminate().
    ComManager (bool);
    virtual ~ComManager();

    // Assignment and copy operators
    // are intentionally not implemented.
    //
    ComManager( const ComManager&);
    ComManager& operator=( const ComManager&);


    PortableServer::POAManager_var _poa_manager;
    PortableServer::POA_var _root_poa;
    CORBA::ORB_var _orb;
    RTCORBA::RTORB_var _rtOrb;
    RTCORBA::Current_var _current;
    CORBA::Policy_var _threadpool_policy_1;

    static CosNaming::NamingContext_var _naming_context;

    bool _sslSupportEnabled;
};

#endif // __COMM_MANAGER_H__
