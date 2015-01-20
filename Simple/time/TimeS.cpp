// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html


// TAO_IDL - Generated from 
// be/be_codegen.cpp:588

#ifndef _TAO_IDL_TIMES_CPP_
#define _TAO_IDL_TIMES_CPP_


#include "TimeS.h"
#include "tao/PortableServer/Operation_Table_Dynamic_Hash.h"
#include "tao/PortableServer/Upcall_Command.h"
#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/PortableServer/Basic_SArguments.h"
#include "tao/PortableServer/Object_SArgument_T.h"
#include "tao/PortableServer/Special_Basic_SArguments.h"
#include "tao/PortableServer/UB_String_SArguments.h"
#include "tao/PortableServer/TypeCode_SArg_Traits.h"
#include "tao/PortableServer/Object_SArg_Traits.h"
#include "tao/PortableServer/get_arg.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/UB_String_Arguments.h"
#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/IFR_Client_Adapter.h"
#include "tao/Object_T.h"
#include "tao/Typecode.h"
#include "tao/DynamicC.h"
#include "tao/CDR.h"
#include "tao/operation_details.h"
#include "tao/PortableInterceptor.h"
#include "tao/Basic_Arguments.h"
#include "ace/Dynamic_Service.h"
#include "ace/Malloc_Allocator.h"

#if defined (__BORLANDC__)
#pragma option -w-rvl -w-rch -w-ccc -w-aus
#endif /* __BORLANDC__ */

#if !defined (__ACE_INLINE__)
#include "TimeS.inl"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:68

// Arg traits specializations.
namespace TAO
{
}


// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:68

// Arg traits specializations.
namespace TAO
{
}


static const TAO_operation_db_entry Time_operations [] = {
  {"current_time", &POA_Time::current_time_skel, 0},
  {"shutdown", &POA_Time::shutdown_skel, 0},
  {"_is_a", &POA_Time::_is_a_skel, 0},
  {"_non_existent", &POA_Time::_non_existent_skel, 0},
  {"_component", &POA_Time::_component_skel, 0},
  {"_interface", &POA_Time::_interface_skel, 0}
};

static const CORBA::Long _tao_Time_optable_size = sizeof (ACE_Hash_Map_Entry<const char *, TAO::Operation_Skeletons>) * (18);
static char _tao_Time_optable_pool [_tao_Time_optable_size];
static ACE_Static_Allocator_Base _tao_Time_allocator (_tao_Time_optable_pool, _tao_Time_optable_size);
static TAO_Dynamic_Hash_OpTable tao_Time_optable (
    Time_operations,
    6,
    12,
    &_tao_Time_allocator
  );


// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ss.cpp:853

TAO::Collocation_Proxy_Broker *
_TAO_Time_Proxy_Broker_Factory_function (CORBA::Object_ptr)
{
  return reinterpret_cast<TAO::Collocation_Proxy_Broker *> (0xdead); // Dummy
}

int
_TAO_Time_Proxy_Broker_Factory_Initializer (size_t)
{
  _TAO_Time_Proxy_Broker_Factory_function_pointer = 
    _TAO_Time_Proxy_Broker_Factory_function;
  
  return 0;
}

static int
_TAO_Time_Proxy_Broker_Stub_Factory_Initializer_Scarecrow =
  _TAO_Time_Proxy_Broker_Factory_Initializer (
      reinterpret_cast<size_t> (_TAO_Time_Proxy_Broker_Factory_Initializer)
    );



// TAO_IDL - Generated from 
// be/be_visitor_interface/interface_ss.cpp:103

POA_Time::POA_Time (void)
  : TAO_ServantBase ()
{
  this->optable_ = &tao_Time_optable;
}

POA_Time::POA_Time (const POA_Time& rhs)
  : TAO_Abstract_ServantBase (rhs),
    TAO_ServantBase (rhs)
{
}

POA_Time::~POA_Time (void)
{
}


// TAO_IDL - Generated from
// be/be_visitor_operation/upcall_command_ss.cpp:127

class current_time_Time
  : public TAO::Upcall_Command
{
public:
  inline current_time_Time (
    POA_Time * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }
  
  virtual void execute (ACE_ENV_SINGLE_ARG_DECL)
  {
    TAO::SArg_Traits< ::CORBA::Long>::ret_arg_type retval =
      TAO::Portable_Server::get_ret_arg< ::CORBA::Long, TAO::SArg_Traits< ::CORBA::Long>::ret_arg_type> (
        this->operation_details_,
        this->args_);
    
    
    retval =
      this->servant_->current_time (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      
  }

private:
  POA_Time * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};


// TAO_IDL - Generated from 
// be/be_visitor_operation/operation_ss.cpp:192

void POA_Time::current_time_skel (
    TAO_ServerRequest & server_request,
    void * TAO_INTERCEPTOR (servant_upcall),
    void * servant
    ACE_ENV_ARG_DECL
  )
{
  
  
#if TAO_HAS_INTERCEPTORS == 1
  static CORBA::TypeCode_ptr const * const exceptions = 0;
  static size_t const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */
  
  TAO::SArg_Traits< ::CORBA::Long>::ret_val retval;
  
  TAO::Argument * const args[] =
    {
      &retval
    };
  
  static size_t const nargs = 1;
  
  POA_Time * const impl =
    static_cast<POA_Time *> (servant);

  current_time_Time command (
    impl,
    server_request.operation_details (),
    args);
  
  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
}


// TAO_IDL - Generated from
// be/be_visitor_operation/upcall_command_ss.cpp:127

class shutdown_Time
  : public TAO::Upcall_Command
{
public:
  inline shutdown_Time (
    POA_Time * servant)
    : servant_ (servant)
  {
  }
  
  virtual void execute (ACE_ENV_SINGLE_ARG_DECL)
  {
    this->servant_->shutdown (
      ACE_ENV_SINGLE_ARG_PARAMETER);
    
  }

private:
  POA_Time * const servant_;
};


// TAO_IDL - Generated from 
// be/be_visitor_operation/operation_ss.cpp:192

void POA_Time::shutdown_skel (
    TAO_ServerRequest & server_request,
    void * TAO_INTERCEPTOR (servant_upcall),
    void * servant
    ACE_ENV_ARG_DECL
  )
{
  
  
#if TAO_HAS_INTERCEPTORS == 1
  static CORBA::TypeCode_ptr const * const exceptions = 0;
  static size_t const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */
  
  TAO::SArg_Traits< void>::ret_val retval;
  
  TAO::Argument * const args[] =
    {
      &retval
    };
  
  static size_t const nargs = 1;
  
  POA_Time * const impl =
    static_cast<POA_Time *> (servant);

  shutdown_Time command (
    impl);
  
  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
}

// TAO_IDL - Generated from 
// be/be_visitor_interface/interface_ss.cpp:169



// TAO_IDL - Generated from
// be/be_visitor_operation/upcall_command_ss.cpp:127

class _is_a_Time_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _is_a_Time_Upcall_Command (
    POA_Time * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }
  
  virtual void execute (ACE_ENV_SINGLE_ARG_DECL)
  {
    TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
      TAO::Portable_Server::get_ret_arg< ::ACE_InputCDR::to_boolean, TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type> (
        this->operation_details_,
        this->args_);
    
    TAO::SArg_Traits< CORBA::Char *>::in_arg_type arg_1 =
      TAO::Portable_Server::get_in_arg< CORBA::Char *, TAO::SArg_Traits< CORBA::Char *>::in_arg_type> (
        this->operation_details_,
        this->args_,
        1);
      
    
    retval =
      this->servant_-> _is_a (
        arg_1
        ACE_ENV_ARG_PARAMETER);
      
  }

private:
  POA_Time * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};
void POA_Time::_is_a_skel (
    TAO_ServerRequest & server_request, 
    void * TAO_INTERCEPTOR (servant_upcall),
    void * servant
    ACE_ENV_ARG_DECL
  )
{
  
  
#if TAO_HAS_INTERCEPTORS == 1
  static CORBA::TypeCode_ptr const * const exceptions = 0;
  static size_t const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */
  
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_val retval;
  TAO::SArg_Traits< CORBA::Char *>::in_arg_val _tao_repository_id;
  
  TAO::Argument * const args[] =
    {
      &retval,
      &_tao_repository_id
    };
  
  static size_t const nargs = 2;
  
  POA_Time * const impl =
    static_cast<POA_Time *> (servant);
  
  _is_a_Time_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);
  
  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
}



// TAO_IDL - Generated from
// be/be_visitor_operation/upcall_command_ss.cpp:127

class _non_existent_Time_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _non_existent_Time_Upcall_Command (
    POA_Time * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }
  
  virtual void execute (ACE_ENV_SINGLE_ARG_DECL)
  {
    TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
      TAO::Portable_Server::get_ret_arg< ::ACE_InputCDR::to_boolean, TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type> (
        this->operation_details_,
        this->args_);
    
    
    retval =
      this->servant_-> _non_existent (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      
  }

private:
  POA_Time * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};
void POA_Time::_non_existent_skel (
    TAO_ServerRequest & server_request, 
    void * TAO_INTERCEPTOR (servant_upcall),
    void * servant
    ACE_ENV_ARG_DECL
  )
{
  
  
#if TAO_HAS_INTERCEPTORS == 1
  static CORBA::TypeCode_ptr const * const exceptions = 0;
  static size_t const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */
  
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_val retval;
  
  TAO::Argument * const args[] =
    {
      &retval
    };
  
  static size_t const nargs = 1;
  
  POA_Time * const impl =
    static_cast<POA_Time *> (servant);
  
  _non_existent_Time_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);
  
  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
}



// TAO_IDL - Generated from 
// be/be_visitor_interface/interface_ss.cpp:393



void POA_Time::_interface_skel (
    TAO_ServerRequest & server_request, 
    void * /* servant_upcall */,
    void * servant
    ACE_ENV_ARG_DECL
  )
{
  TAO_IFR_Client_Adapter *_tao_adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );
    
  if (_tao_adapter == 0)
    {
      ACE_THROW (CORBA::INTF_REPOS (CORBA::OMGVMCID | 1,
                                    CORBA::COMPLETED_NO));
    }
  
  POA_Time * const impl =
    static_cast<POA_Time *> (servant);
  CORBA::InterfaceDef_ptr _tao_retval = 
    impl->_get_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  
  server_request.init_reply ();
  TAO_OutputCDR &_tao_out = *server_request.outgoing ();
  
  CORBA::Boolean const _tao_result =
    _tao_adapter->interfacedef_cdr_insert (
        _tao_out,
        _tao_retval
      );
  
  _tao_adapter->dispose (_tao_retval);
  
  if (_tao_result == 0)
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}



// TAO_IDL - Generated from
// be/be_visitor_operation/upcall_command_ss.cpp:127

class _get_component_Time_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _get_component_Time_Upcall_Command (
    POA_Time * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }
  
  virtual void execute (ACE_ENV_SINGLE_ARG_DECL)
  {
    TAO::SArg_Traits< ::CORBA::Object>::ret_arg_type retval =
      TAO::Portable_Server::get_ret_arg< ::CORBA::Object, TAO::SArg_Traits< ::CORBA::Object>::ret_arg_type> (
        this->operation_details_,
        this->args_);
    
    
    retval =
      this->servant_-> _get_component (
        ACE_ENV_SINGLE_ARG_PARAMETER);
      
  }

private:
  POA_Time * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};
void POA_Time::_component_skel (
    TAO_ServerRequest & server_request, 
    void * TAO_INTERCEPTOR (servant_upcall),
    void * servant
    ACE_ENV_ARG_DECL
  )
{
  
  
#if TAO_HAS_INTERCEPTORS == 1
  static CORBA::TypeCode_ptr const * const exceptions = 0;
  static size_t const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */
  
  TAO::SArg_Traits< ::CORBA::Object>::ret_val retval;
  
  TAO::Argument * const args[] =
    {
      &retval
    };
  
  static size_t const nargs = 1;
  
  POA_Time * const impl =
    static_cast<POA_Time *> (servant);
  
  _get_component_Time_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);
  
  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
}

CORBA::Boolean POA_Time::_is_a (
    const char* value
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  return
    (
      !ACE_OS::strcmp (
          value,
          "IDL:Time:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        )
    );
}

const char* POA_Time::_interface_repository_id (void) const
{
  return "IDL:Time:1.0";
}

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ss.cpp:794

void POA_Time::_dispatch (
    TAO_ServerRequest & req,
    void * servant_upcall
    ACE_ENV_ARG_DECL
  )
{
  this->synchronous_upcall_dispatch (req,
                                     servant_upcall,
                                     this
                                     ACE_ENV_ARG_PARAMETER);
}

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ss.cpp:745

Time *
POA_Time::_this (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Stub *stub = this->_create_stub (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  
  TAO_Stub_Auto_Ptr safe_stub (stub);
  CORBA::Object_ptr tmp = CORBA::Object::_nil ();
  
  CORBA::Boolean _tao_opt_colloc =
    stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ();
  
  ACE_NEW_RETURN (
      tmp,
      CORBA::Object (stub, _tao_opt_colloc, this),
      0
    );
  
  CORBA::Object_var obj = tmp;
  (void) safe_stub.release ();
  
  typedef ::Time STUB_SCOPED_NAME;
  return
    TAO::Narrow_Utils<STUB_SCOPED_NAME>::unchecked_narrow (
        obj.in (),
        _TAO_Time_Proxy_Broker_Factory_function_pointer
      );
}

#endif /* ifndef */

