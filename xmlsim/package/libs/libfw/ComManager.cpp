/***********************************************************************************************
* COPYRIGHT $Date:   Oct 14 1999 10:10:58  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: ComManager
* Type: C++ Source
* Originator: $Author:   dtchalak  $
* File: $Workfile:   ComManager.cpp  $
* Version: $Revision:   1.5  $
* Description:
* Wrapper for setting up CORBA services.
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/com/src/ComManager.cpv  $

Rev 1.5   Oct 14 1999 10:10:58   dtchalak
Orbix3.0 upgrades.

Rev 1.4   03 Sep 1999 11:09:00   aloo
Updated for Framework 1.1: Integration with Sysmon,
and using OrbixNotification.

Rev 1.3   26 Jul 1999 11:04:24   aloo
Updated to use OrbixNotification
//
//    Rev 1.2   03 Mar 1999 10:13:18   aloo
// Added error logging.
**********************************************************************************************/
#include "ComManager.h"
#include <tao/TimeBaseC.h>
#include "tao/Messaging/Messaging.h"
#include <ace/streams.h>
#include  <ace/OS.h>
#include "BaseException.h"
#include <sysErrLog.h>
#include "orbsvcs/SecurityC.h"
#include "CORBAUtil.h"
using namespace ELogger;

#include <iostream>

#include <fstream>
#include "Server_i.h"
#include "tutil.h"


#include "ConfigNames.h"

using namespace EMS_FW;

extern const char *sourceName;
static const char* className = "ComManager";
static const char* defaultEmsId = "ems";

// initialize static members
ComManager* ComManager::_comMgr = 0;
CosNaming::NamingContext_var ComManager::_naming_context = CosNaming::NamingContext::_nil();

// constructor
ComManager::ComManager(bool sslSupportEnabled)
{
    _sslSupportEnabled = sslSupportEnabled;
}

/**
 * Return the single instance of this class
 */
ComManager* ComManager::instance(bool sslSupportEnabled)
{
    //TO DO: Need to make it thread safe
    if (!_comMgr)
    {
        _comMgr = new ComManager(sslSupportEnabled);
    }
    return _comMgr;
}

// destructor
ComManager::~ComManager()
{}


void ComManager::run() const
{
    const char* methodName = "run";

    // Activate the POA Manager to allow new requests to arrive
    //
    _poa_manager->activate();

    // Give control to the ORB to let it process incoming requests
    //
    ErrorLogger::logVarError(sourceName, className, methodName, TRACE2,
                             "Giving control to the ORB to process requests...");

    try
    {
        _orb->run();
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "ORB exception during run.");
    }

    // ORB stopped processing requests. Exiting...
    //
    ErrorLogger::logVarError(sourceName, className, methodName, TRACE2,
                             "ORB stopped processing requests, exit.");

    if ( !CORBA::is_nil(_orb) )
        _orb->destroy();

    exit(0);
}


void ComManager::initializeORB (int argc, char **argv, const string &namingServiceHostname, const string &namingServicePort,
                                const string &orbConfFileName, const string &orbDebugLevel,
                                const string &orbAdditionalParamaters, const string &orbLogFile)
{

    list<string> argList;

    for (int i = 0;i < argc;i++)
        argList.push_back (argv[i]);

    char* arg = NULL;

    if (argv != NULL)
        arg = argv[0];
    else
        arg = (char*)"";

    CORBAUtil::createCORBAArgList (argList, arg, namingServiceHostname, namingServicePort, orbConfFileName, orbDebugLevel, orbAdditionalParamaters, orbLogFile, _sslSupportEnabled);

    int argc2 = argList.size();
    char **argv2 = new char * [argc2];

    int i = 0;
    for (list<string>::iterator iter = argList.begin(); iter != argList.end();iter++)
    {
        string arg = (*iter);
        argv2[i] = (char *) (arg.c_str());
        i++;
    }

    Tutil::renameFile (orbLogFile, orbLogFile + ".old");
    initializeORB (argc2, argv2);
}


void ComManager::initializeORB(int argc, char **argv)
{
    // ORB_init() scans the passed argument vector
    // for command-line options that start with -ORB and
    // removes them.
    //
    const char* methodName = "initializeORB";

    ErrorLogger::logError(sourceName, className, methodName, TRACE1, "Initializing the ORB with the following arguments");

    for (int i = 0;i < argc;i++)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                                 "ORB init argument %d = [%s]", i, argv[i]);
    }

    _orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var poa_obj =
        _orb->resolve_initial_references("RootPOA");

    _root_poa = PortableServer::POA::_narrow(poa_obj);
    assert(!CORBA::is_nil(_root_poa));

    _poa_manager = _root_poa->the_POAManager();
    assert(!CORBA::is_nil(_poa_manager));

    ErrorLogger::logError(sourceName, className, methodName, TRACE1, "Initialized the ORB successfully");

}

void ComManager::initializeRTORB (int argc, char **argv, const string &namingServiceHostname, const string &namingServicePort,
                                  const string &orbConfFileName, const string &orbDebugLevel,
                                  const string &orbAdditionalParamaters, const string &orbLogFile)
{
    initializeORB (argc, argv, namingServiceHostname, namingServicePort, orbConfFileName, orbDebugLevel, orbAdditionalParamaters, orbLogFile);

    ErrorLogger::logError(sourceName, className, "initializeRTORB", TRACE1, "Initializing the (RT)ORB");

    CORBA::Object_var object;

    object = _orb->resolve_initial_references ("RTORB");
    _rtOrb = RTCORBA::RTORB::_narrow (object.in ());

    object = _orb->resolve_initial_references ("RTCurrent");
    _current = RTCORBA::Current::_narrow (object.in ());

    RTCORBA::Priority default_thread_priority = _current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);

    CORBA::ULong stacksize = 0;
    CORBA::Boolean allow_request_buffering = 0;
    CORBA::ULong max_buffered_requests = 0;
    CORBA::ULong max_request_buffer_size = 0;

    RTCORBA::ThreadpoolId threadpool_id_1 = _rtOrb->create_threadpool (stacksize,
                                            1,
                                            100,
                                            default_thread_priority,
                                            allow_request_buffering,
                                            max_buffered_requests,
                                            max_request_buffer_size
                                            ACE_ENV_ARG_PARAMETER);

    _threadpool_policy_1 = _rtOrb->create_threadpool_policy (threadpool_id_1 ACE_ENV_ARG_PARAMETER);

    ErrorLogger::logError(sourceName, className, "initializeRTORB", TRACE1, "Initialized the (RT)ORB successfully");

}

void ComManager::initializeRTORB (int argc, char **argv)
{
    initializeORB (argc, argv);

    ErrorLogger::logError(sourceName, className, "initializeRTORB", TRACE1, "Initializing the (RT)ORB");

    CORBA::Object_var object;

    object = _orb->resolve_initial_references ("RTORB");
    _rtOrb = RTCORBA::RTORB::_narrow (object.in ());

    object = _orb->resolve_initial_references ("RTCurrent");
    _current = RTCORBA::Current::_narrow (object.in ());

    RTCORBA::Priority default_thread_priority = _current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);

    CORBA::ULong stacksize = 0;
    CORBA::Boolean allow_request_buffering = 0;
    CORBA::ULong max_buffered_requests = 0;
    CORBA::ULong max_request_buffer_size = 0;

    RTCORBA::ThreadpoolId threadpool_id_1 = _rtOrb->create_threadpool (stacksize,
                                            1,
                                            100,
                                            default_thread_priority,
                                            allow_request_buffering,
                                            max_buffered_requests,
                                            max_request_buffer_size
                                            ACE_ENV_ARG_PARAMETER);

    _threadpool_policy_1 = _rtOrb->create_threadpool_policy (threadpool_id_1 ACE_ENV_ARG_PARAMETER);

    ErrorLogger::logError(sourceName, className, "initializeRTORB", TRACE1, "Initialized the (RT)ORB successfully");
}

PortableServer::POA_ptr ComManager::create_rtpoa_with_properties(const string &poa_name, const Policies& policies)
{
    // Policies for the firstPOA to be created.

    ErrorLogger::logVarError(sourceName, className, "create_rtpoa_with_properties", TRACE1, "Creating (RT)POA with name [%s]", poa_name.c_str());

    CORBA::PolicyList policy_list (4);
    policy_list.length (4);

    policy_list[0] = _root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION ACE_ENV_ARG_PARAMETER);
    policy_list[1] = CORBA::Policy::_duplicate (_threadpool_policy_1);
    policy_list[2] = _rtOrb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED, 0 ACE_ENV_ARG_PARAMETER);
    policy_list[3] = _root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);


    PortableServer::POA_ptr servant_poa = _root_poa->create_POA( poa_name.c_str(), _poa_manager, policy_list);
    assert(!CORBA::is_nil(servant_poa));

    ErrorLogger::logVarError(sourceName, className, "create_rtpoa_with_properties", TRACE1, "Created (RT)POA with name [%s]", poa_name.c_str());

    return servant_poa;


}

PortableServer::POA_ptr ComManager::create_poa_with_properties(const string &poa_name, const Policies& policies)
{
    const char* methodName = "create_poa_with_properties";
    // Initialize the servant's apartment.
    //
    ErrorLogger::logVarError(sourceName, className, "create_poa_with_properties", TRACE1, "Creating POA with name [%s]", poa_name.c_str());

    // Set the size of the initial buffer allocation.
    //
    CORBA::PolicyList policy_list(policies.size());
    policy_list.length(policies.size());

    int i(0);
    for (Policies::const_iterator iter = policies.begin() ; iter != policies.end(); iter++, i++)
    {
        switch ((*iter).first)
        {
        case thread_policy:
        {
            PortableServer::ThreadPolicyValue thread_policy_value;
            switch ((*iter).second)
            {
            case orb_ctrl_model:
                thread_policy_value = PortableServer::ORB_CTRL_MODEL;
                break;
            case single_thread_model:
                thread_policy_value = PortableServer::SINGLE_THREAD_MODEL;
                break;
            default:
                throw BaseException("Unsupported POA thread policy passed.", className, methodName);
                break;
            }
            policy_list[i] = _root_poa->create_thread_policy(thread_policy_value);
        }
        break;
        case id_assignment_policy:
        {
            PortableServer::IdAssignmentPolicyValue id_assignment_policy_value;
            switch ((*iter).second)
            {
            case user_id:
                id_assignment_policy_value = PortableServer::USER_ID;
                break;
            case system_id:
                id_assignment_policy_value = PortableServer::SYSTEM_ID;
                break;
            default:
                throw BaseException("Unsupported POA id assignment policy passed.", className, methodName);
                break;
            }
            policy_list[i] = _root_poa->create_id_assignment_policy(id_assignment_policy_value);
        }
        break;
        case lifespan_policy:
        {
            PortableServer::LifespanPolicyValue life_span_policy_value;
            switch ((*iter).second)
            {
            case transient:
                life_span_policy_value = PortableServer::TRANSIENT;
                break;
            case persistent:
                life_span_policy_value = PortableServer::PERSISTENT;
                break;
            default:
                throw BaseException("Unsupported POA id assignment policy passed.", className, methodName);
                break;
            }
            policy_list[i] = _root_poa->create_lifespan_policy(life_span_policy_value);
        }
        break;
        default:
            throw BaseException("Unsupported POA policy passed.", className, methodName);
            break;
        }
    }
    policy_list.length(i);


    if (_sslSupportEnabled)
    {
        policy_list.length (i + 1);

        Security::QOP qop = Security::SecQOPNoProtection;
        CORBA::Any no_protection;
        no_protection <<= qop;

        // Create the Security::QOPPolicy.
        CORBA::Policy_var policy = _orb->create_policy (Security::SecQOPPolicy, no_protection);
        policy_list[i] = CORBA::Policy::_duplicate (policy.in ());
    }

    PortableServer::POA_ptr servant_poa = _root_poa->create_POA( poa_name.c_str(), _poa_manager, policy_list);
    assert(!CORBA::is_nil(servant_poa));

    ErrorLogger::logVarError(sourceName, className, "create_poa_with_properties", TRACE1, "Created POA with name [%s] successfully", poa_name.c_str());

    return servant_poa;
}

CORBA::Object_ptr ComManager::activate_object_with_name(const PortableServer::POA_ptr servant_poa,
        const string &object_name,
        const PortableServer::Servant servant)
{

    const char* methodName = "activate_object_with_name";

    PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId( object_name.c_str() );
    CORBA::Object_var obj(CORBA::Object::_nil());

    try
    {
        servant_poa->activate_object_with_id(oid, servant);
        obj = servant_poa->id_to_reference(oid);
    }
    catch (CORBA::SystemException &se )
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "CORBA System Exception caught while activating object: %s", (se._info()).c_str());

    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Exception in object activation: %s", (ex._info()).c_str());
    }
    return obj._retn();

}

CORBA::Object_ptr ComManager::activateObject(const PortableServer::POA_ptr servant_poa,
        const PortableServer::Servant servant)
{

    const char* methodName = "activateObject";

    CORBA::Object_var obj(CORBA::Object::_nil());
    try
    {
        PortableServer::ObjectId_var oid = servant_poa->activate_object(servant);
        obj = servant_poa->id_to_reference(oid);
    }
    catch (CORBA::SystemException &se )
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "CORBA System Exception caught while activating object: %s", (se._info()).c_str());

    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Exception in object activation: %s", (ex._info()).c_str());
    }
    return obj._retn();
}

bool ComManager::deactivateObject(const PortableServer::POA_ptr servant_poa, CORBA::Object_ptr object)
{

    const char* methodName = "deactivateObject(object)";

    ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                             "Started..");
    try
    {
        if (!CORBA::is_nil(servant_poa) && !CORBA::is_nil(object))
        {
            PortableServer::ObjectId_var oid = servant_poa->reference_to_id(object);
            servant_poa->deactivate_object(oid);
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                                     "Finished..");
            return true;
        }
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                                     "Null corba object or servant poa received, could not deactivate object");
            return false;
        }

    }
    catch (CORBA::SystemException &se )
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "CORBA System Exception caught while deactivating object: %s", (se._info()).c_str());
        return false;

    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Exception in object deactivation: %s", (ex._info()).c_str());
        return false;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Unknown exception in deactivating object...");
        return false;
    }


}

bool ComManager::deactivateObject(const PortableServer::POA_ptr servant_poa, PortableServer::Servant servant)
{

    const char* methodName = "deactivateObject(servant)";

    ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                             "Started..");
    try
    {
        if (!CORBA::is_nil(servant_poa) && (servant))
        {
            PortableServer::ObjectId_var oid = servant_poa->servant_to_id(servant);
            servant_poa->deactivate_object(oid);
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                                     "Finished..");
            return true;
        }
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                                     "Null servant object or servant poa received, could not deactivate object");
            return false;
        }
    }
    catch (CORBA::SystemException &se )
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "CORBA System Exception caught while deactivating object: %s", (se._info()).c_str());
        return false;

    }
    catch (CORBA::Exception &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Exception in object deactivation: %s", (ex._info()).c_str());
        return false;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Unknown exception in deactivating object...");
        return false;
    }


}

void ComManager::terminate()
{
    const char* methodName = "terminate";

    // Shut down ORB
    try
    {
        _orb->shutdown(false);
    }
    catch (...)
    {
        throw BaseException("Unknown Exception caught", className, methodName);
    }
}

void ComManager::finalize()
{
    const char* methodName = "finalize";

    try
    {
        _orb->destroy();
    }
    catch (...)
    {
        throw BaseException("Unknown Exception caught", className, methodName);
    }
    delete _comMgr;
    _comMgr = 0;
}

CORBA::Object_ptr ComManager::resolveObjectName(const string &object_name, string& error)
{
    const char* methodName = "resolveObjectName";

    if (CORBA::is_nil(_naming_context))
    {
        ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                              "Reference to Naming Service invalid.");

        return CORBA::Object::_nil();
    }

    if (CORBA::is_nil(_naming_context))
    {
        setRootContext("");
        ErrorLogger::logError(sourceName, className, methodName, TRACE1,
                              "Setting naming context to global.");
    }

    CORBA::Object_ptr object(CORBA::Object::_nil());

    CosNaming::Name_var name;
    name = new CosNaming::Name(1);
    name->length(1);
    name[(CORBA::ULong)0].id = CORBA::string_dup(object_name.c_str());
    name[(CORBA::ULong)0].kind = CORBA::string_dup("");

    string maxConnectRetries = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionRetries", "5");
    string waitBeforeRetry = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionWaitBeforeRetry", "2");

    int tries = atoi (maxConnectRetries.c_str());
    int waitBeforeRetryValue = atoi (waitBeforeRetry.c_str());

    for (int i = 1;i < tries + 1;i++)
    {
        try
        {
            if (CORBA::is_nil(_naming_context))
                setRootContext("");
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE11, "Attempting to resolve object %s with Naming Service", object_name.c_str());
            object = ComManager::_naming_context->resolve(name);
            break;
        }
        catch (CORBA::COMM_FAILURE)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to resolve object %s on attempt %d of %d; will retry after %d seconds", object_name.c_str(), i, tries, waitBeforeRetryValue);
            sleep(waitBeforeRetryValue);
        }
        catch (CosNaming::NamingContext::NotFound)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                     "Naming Service could not find : %s", object_name.c_str());
            error = "NameNotFoundError";
            break;
        }
        catch (CosNaming::NamingContext::CannotProceed)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service CannotProceed exception.");
            break;
        }
        catch (CosNaming::NamingContext::InvalidName)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service InvalidName exception.");
            break;
        }
        catch (...)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Unknown exception cought.");
            break;
        }
    }

    if (CORBA::is_nil(object))
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Null object returned for: %s", object_name.c_str());
    }
    return object;
}

// Register an object in the naming service.  See implementation comments for resolve
bool ComManager::bindObject(CORBA::Object_ptr object, const string &object_name)
{
    const char* methodName = "bindObject";
    bool op_status = false;

    if (CORBA::is_nil(_naming_context))
    {
        setRootContext("");
        ErrorLogger::logError(sourceName, className, methodName, TRACE1,
                              "Setting naming context to global.");
    }

    CosNaming::Name nsName;
    nsName.length(1);
    nsName[0].id = CORBA::string_dup(object_name.c_str());
    nsName[0].kind = CORBA::string_dup("");

    string maxConnectRetries = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionRetries", "5");
    string waitBeforeRetry = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionWaitBeforeRetry", "2");

    int tries = atoi (maxConnectRetries.c_str());
    int waitBeforeRetryValue = atoi (waitBeforeRetry.c_str());

    for (int i = 1;i < tries + 1;i++)
    {
        try
        {
            if (CORBA::is_nil(_naming_context))
                setRootContext("");
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE11, "Attempting to rebind object %s with Naming Service, Try %d of %d", object_name.c_str(), i, tries);
            _naming_context->rebind(nsName, object);
            op_status = true;
            break;
        }
        catch (CORBA::COMM_FAILURE)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to rebind object %s on attempt %d of %d; will retry after %d seconds", object_name.c_str(), i, tries, waitBeforeRetryValue);
            sleep(waitBeforeRetryValue);
        }
        catch (CosNaming::NamingContext::NotFound)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service NotFound exception");
        }
        catch (CosNaming::NamingContext::CannotProceed)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service CannotProceed exception.");
        }
        catch (CosNaming::NamingContext::InvalidName)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service InvalidName exception.");
        }
        catch (...)
        {
            ErrorLogger::logError(sourceName, className, CRITICAL,
                                  "Unknown Exception caught.");
        }
    }
    return op_status;
}


// De-register an object from the naming service.
bool ComManager::unbindObject(const string &object_name)
{
    const char* methodName = "unbindObject";

    bool op_status(false);

    if (CORBA::is_nil(_naming_context))
    {

        ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                              "Reference to Naming Service invalid.");

        return op_status;
    }

    // create a sequence of names for the unbind
    CosNaming::Name nsName;
    nsName.length(1);
    nsName[0].id = CORBA::string_dup(object_name.c_str());
    nsName[0].kind = CORBA::string_dup("");


    string maxConnectRetries = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionRetries", "5");
    string waitBeforeRetry = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionWaitBeforeRetry", "2");

    int tries = atoi (maxConnectRetries.c_str());
    int waitBeforeRetryValue = atoi (waitBeforeRetry.c_str());

    for (int i = 1;i < tries + 1;i++)
    {
        try
        {
            if (CORBA::is_nil(_naming_context))
                setRootContext("");
            ErrorLogger::logVarError(sourceName, className, methodName, TRACE11, "Attempting to unbind object %s from Naming Service, Try %d of %d", object_name.c_str(), i, tries);
            _naming_context->unbind(nsName);
            op_status = true;
            break;
        }
        catch (CORBA::COMM_FAILURE)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to unbind object %s on attempt %d of %d; will retry after %d seconds", object_name.c_str(), i, tries, waitBeforeRetryValue);
            sleep(waitBeforeRetryValue);
        }
        catch (CosNaming::NamingContext::NotFound)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                     "Naming Service could not find : %s", object_name.c_str());
            break;
        }
        catch (CosNaming::NamingContext::CannotProceed)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service CannotProceed exception.");
            break;
        }
        catch (CosNaming::NamingContext::InvalidName)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Naming Service InvalidName exception.");
            break;
        }
        catch (...)
        {
            ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                                  "Unknown exception cought.");
            break;
        }
    }

    return op_status;
}

bool ComManager::setRootContext(const string &context_name)
{
    char* methodName = (char *)"setRootContext";

    string maxConnectRetries = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionRetries", "5");
    string waitBeforeRetry = Server_i::instance()->getDefaultConfigValue("NameServiceConnectionWaitBeforeRetry", "2");

    int tries = atoi (maxConnectRetries.c_str());
    int waitBeforeRetryValue = atoi (waitBeforeRetry.c_str());
    bool connected = false;

    for (int i = 1;i < tries + 1;i++)
    {
        if (CORBA::is_nil(_naming_context))
        {
            try
            {

                CORBA::Object_var naming_context_object =
                    _orb->resolve_initial_references ("NameService");
                _naming_context = CosNaming::NamingContext::_narrow(naming_context_object);
                unsigned long timeout = 60 * 1000;  // 1 minute time out
                CORBA::Object_ptr c_ptr = SetTimeOut(_naming_context, timeout);
                _naming_context = CosNaming::NamingContext::_narrow(c_ptr);
                connected = true;
                break;
            }
            catch (CORBA::COMM_FAILURE)
            {
                if (i < tries)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; CORBA COMM FAILURE; Will retry after %d seconds", i, tries, waitBeforeRetryValue);
                    sleep(waitBeforeRetryValue);
                }
                else
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; CORBA COMM FAILURE; Aborting", i, tries);

            }
            catch (CORBA::UserException &ue)
            {
                if (i < tries)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; CORBA User Exception [%s]; Will retry after %d seconds; ", i, tries, (ue._info()).c_str(), waitBeforeRetryValue);
                    sleep(waitBeforeRetryValue);
                }
                else
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; CORBA User Exception [%s]; Aborting; ", i, tries, (ue._info()).c_str());

            }
            catch (CORBA::SystemException &se )
            {
                if (i < tries)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; CORBA System Exception [%s]; Will retry after %d seconds", i, tries, (se._info()).c_str(), waitBeforeRetryValue);
                    sleep(waitBeforeRetryValue);
                }
                else
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; CORBA System Exception [%s]; Aborting; ", i, tries, (se._info()).c_str());

            }
            catch (...)
            {
                if (i < tries)
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; will retry after %d seconds", i, tries, waitBeforeRetryValue);
                    sleep(waitBeforeRetryValue);
                }
                else
                    ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to to resolve initial references on attempt %d of %d; aborting", i, tries);
            }
        }
    }

    if (context_name.empty() || context_name.compare(defaultEmsId) == 0)
    {
        return connected;
    }


    CosNaming::Name ncName;
    ncName.length(1);
    ncName[0].id = CORBA::string_dup(context_name.c_str());
    ncName[0].kind = CORBA::string_dup("");

    CosNaming::NamingContext_ptr emsContext;
    for (int i = 1;i < tries + 1;i++)
    {
        try
        {
            emsContext = _naming_context->bind_new_context(ncName);
            _naming_context = emsContext;
            return true;
        }
        catch (CORBA::COMM_FAILURE)
        {
            ErrorLogger::logVarError(sourceName, className, methodName, MAJOR, "Could not connect to Naming Service while trying to bind object name %s on attempt %d of %d; will retry after %d seconds", context_name.c_str(), i, tries, waitBeforeRetryValue);
            sleep(waitBeforeRetryValue);
        }
        catch (CosNaming::NamingContext::AlreadyBound)
        {
            CORBA::Object_ptr obj = resolveObjectName(context_name);
            _naming_context = CosNaming::NamingContext::_narrow(obj);
            return true;
        }
        catch (...)
        {
            _naming_context = CosNaming::NamingContext::_nil();
            return false;
        }
    }

    ErrorLogger::logVarError(sourceName, className, methodName, TRACE2,
                             "Set root context with name %s", context_name.c_str());
    return true;
}

CORBA::Object_ptr ComManager::SetTimeOut(CORBA::Object_ptr pObj, unsigned long msecs)
{
    const char* methodName = "SetTimeOut";
    TimeBase::TimeT relative_expiry = (TimeBase::TimeT)msecs * (TimeBase::TimeT)10000L; // in nano seconds
    // Set up the policy for being able to set time out
    CORBA::Any relative_request_timeout_value;
    relative_request_timeout_value <<= relative_expiry;
    CORBA::PolicyList policies(1);
    policies.length(1);
    try
    {
        policies[0] = _orb->create_policy(
                          Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                          relative_request_timeout_value
                      );
        CORBA::Object_ptr pNewRef =
            pObj->_set_policy_overrides(policies, CORBA::ADD_OVERRIDE);
        return pNewRef;
    }
    catch (CORBA::PolicyError& pe)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Policy Error while setting up Time Out Policy: %s", (pe._info()).c_str());

    }
    catch (CORBA::InvalidPolicies& ip)
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Policy Error while setting up Time Out Policy: %s", (ip._info()).c_str());

    }
    catch (CORBA::SystemException& se)
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Policy Error while setting up Time Out Policy: %s", (se._info()).c_str());

    }
    catch (CORBA::Exception &ex)
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Unexpected CORBA exception while setting timeout: %s", (ex._info()).c_str());

    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                              "Unknown Exception while setting up Time Out Policy");

    }
    return CORBA::Object::_nil();

}

CORBA::Object_ptr
ComManager::SetSyncScopPolicy(CORBA::Object_ptr pObj, int policyType)
{
    const char* methodName = "SetSyncScopPolicy";

    ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                             "Started..");

    Messaging::SyncScope syncscope;

    switch (policyType)
    {
    case SYNC_NONE:
        syncscope = Messaging::SYNC_NONE;
    case SYNC_WITH_TRANSPORT:
        syncscope = Messaging::SYNC_WITH_TRANSPORT;
    case SYNC_WITH_SERVER:
        syncscope = Messaging::SYNC_WITH_SERVER;
    case SYNC_WITH_TARGET:
        syncscope = Messaging::SYNC_WITH_TARGET;
    default:
        syncscope = Messaging::SYNC_WITH_TRANSPORT;
    }


    CORBA::Any sync_any;
    sync_any <<= syncscope;

    CORBA::PolicyList policies(1);
    policies.length(1);
    try
    {
        policies[0] = _orb->create_policy(
                          Messaging::SYNC_SCOPE_POLICY_TYPE,
                          sync_any
                      );
        CORBA::Object_ptr pNewRef =
            pObj->_set_policy_overrides(policies, CORBA::ADD_OVERRIDE);

        ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                                 "Finished..");

        return pNewRef;
    }
    catch (CORBA::PolicyError& pe)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Policy Error while setting up Sync Scope Policy: %s", (pe._info()).c_str());

    }
    catch (CORBA::InvalidPolicies& ip)
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Policy Error while setting up Sync Scope Policy: %s", (ip._info()).c_str());

    }
    catch (CORBA::SystemException& se)
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Unexpected CORBA System exception while setting up Sync Scope Policy:: %s", (se._info()).c_str());

    }
    catch (CORBA::Exception &ex)
    {

        ErrorLogger::logVarError(sourceName, className, methodName, MAJOR,
                                 "Unexpected CORBA exception while setting up Sync Scope Policy: %s", (ex._info()).c_str());

    }
    catch (...)
    {

        ErrorLogger::logError(sourceName, className, methodName, MAJOR,
                              "Unknown Exception  while setting up Sync Scope Policy");

    }

    ErrorLogger::logVarError(sourceName, className, methodName, TRACE1,
                             "Finished..");

    return CORBA::Object::_nil();

}
