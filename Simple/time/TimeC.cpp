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
// be/be_codegen.cpp:292


#include "TimeC.h"
#include "tao/CDR.h"
#include "tao/Exception_Data.h"
#include "tao/Invocation_Adapter.h"
#include "tao/Object_T.h"
#include "tao/Basic_Arguments.h"
#include "ace/OS_NS_string.h"

#if defined (__BORLANDC__)
#pragma option -w-rvl -w-rch -w-ccc -w-aus -w-sig
#endif /* __BORLANDC__ */

#if !defined (__ACE_INLINE__)
#include "TimeC.inl"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:68

// Arg traits specializations.
namespace TAO
{
}


// TAO_IDL - Generated from
// be/be_visitor_interface/interface_cs.cpp:60

// Traits specializations for Time.

Time_ptr
TAO::Objref_Traits<Time>::duplicate (
    Time_ptr p
  )
{
  return Time::_duplicate (p);
}

void
TAO::Objref_Traits<Time>::release (
    Time_ptr p
  )
{
  CORBA::release (p);
}

Time_ptr
TAO::Objref_Traits<Time>::nil (void)
{
  return Time::_nil ();
}

CORBA::Boolean
TAO::Objref_Traits<Time>::marshal (
    Time_ptr p,
    TAO_OutputCDR & cdr
  )
{
  return CORBA::Object::marshal (p, cdr);
}

// Function pointer for collocation factory initialization.
TAO::Collocation_Proxy_Broker * 
(*_TAO_Time_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;

// TAO_IDL - Generated from
// be/be_visitor_operation/operation_cs.cpp:78

CORBA::Long Time::current_time (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  if (!this->is_evaluated ())
    {
      ACE_NESTED_CLASS (CORBA, Object)::tao_object_initialize (this);
    }
  
  if (this->the_TAO_Time_Proxy_Broker_ == 0)
    {
      Time_setup_collocation ();
    }
  
  ACE_DECLARE_NEW_CORBA_ENV;
  TAO::Arg_Traits< ::CORBA::Long>::ret_val _tao_retval;
  
  TAO::Argument *_the_tao_operation_signature [] =
    {
      &_tao_retval
    };
  
  TAO::Invocation_Adapter _tao_call (
      this,
      _the_tao_operation_signature,
      1,
      "current_time",
      12,
      this->the_TAO_Time_Proxy_Broker_
    );
  
  _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (_tao_retval.excp ());
  
  return _tao_retval.retn ();
}

// TAO_IDL - Generated from
// be/be_visitor_operation/operation_cs.cpp:78

void Time::shutdown (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  if (!this->is_evaluated ())
    {
      ACE_NESTED_CLASS (CORBA, Object)::tao_object_initialize (this);
    }
  
  if (this->the_TAO_Time_Proxy_Broker_ == 0)
    {
      Time_setup_collocation ();
    }
  
  ACE_DECLARE_NEW_CORBA_ENV;
  TAO::Arg_Traits< void>::ret_val _tao_retval;
  
  TAO::Argument *_the_tao_operation_signature [] =
    {
      &_tao_retval
    };
  
  TAO::Invocation_Adapter _tao_call (
      this,
      _the_tao_operation_signature,
      1,
      "shutdown",
      8,
      this->the_TAO_Time_Proxy_Broker_,
      TAO::TAO_ONEWAY_INVOCATION
    );
  
  _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

Time::Time (void)
 : the_TAO_Time_Proxy_Broker_ (0)
{
  this->Time_setup_collocation ();
}

void
Time::Time_setup_collocation ()
{
  if (::_TAO_Time_Proxy_Broker_Factory_function_pointer)
    {
      this->the_TAO_Time_Proxy_Broker_ =
        ::_TAO_Time_Proxy_Broker_Factory_function_pointer (this);
    }
}

Time::~Time (void)
{}

Time_ptr
Time::_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL
  )
{
  return
    TAO::Narrow_Utils<Time>::narrow (
        _tao_objref,
        "IDL:Time:1.0",
        _TAO_Time_Proxy_Broker_Factory_function_pointer
        ACE_ENV_ARG_PARAMETER
      );
}

Time_ptr
Time::_unchecked_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL
  )
{
  return
    TAO::Narrow_Utils<Time>::unchecked_narrow (
        _tao_objref,
        "IDL:Time:1.0",
        _TAO_Time_Proxy_Broker_Factory_function_pointer
        ACE_ENV_ARG_PARAMETER
      );
}

Time_ptr
Time::_duplicate (Time_ptr obj)
{
  if (! CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  
  return obj;
}

void
Time::_tao_release (Time_ptr obj)
{
  CORBA::release (obj);
}

CORBA::Boolean
Time::_is_a (
    const char *value
    ACE_ENV_ARG_DECL
  )
{
  if (
      !ACE_OS::strcmp (
          value,
          "IDL:Time:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        )
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return this->ACE_NESTED_CLASS (CORBA, Object)::_is_a (
          value
          ACE_ENV_ARG_PARAMETER
        );
    }
}

const char* Time::_interface_repository_id (void) const
{
  return "IDL:Time:1.0";
}

CORBA::Boolean
Time::marshal (TAO_OutputCDR &cdr)
{
  return (cdr << this);
}

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_cs.cpp:63

CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Time_ptr _tao_objref
  )
{
  CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Time_ptr &_tao_objref
  )
{
  CORBA::Object_var obj;
  
  if (!(strm >> obj.inout ()))
    {
      return false;
    }
  
  typedef ::Time RHS_SCOPED_NAME;
  
  // Narrow to the right type.
  _tao_objref =
    TAO::Narrow_Utils<RHS_SCOPED_NAME>::unchecked_narrow (
        obj.in (),
        _TAO_Time_Proxy_Broker_Factory_function_pointer
      );
    
  return 1;
}
