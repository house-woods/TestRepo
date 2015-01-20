// to include the declaration of semun from ospace 2.2 instead of ACE --- li
//#include "ospace/file.h"
#include "BaseTypesASN1.h" 
/*#include "tao/Stub.h"
#include "tao/Invocation.h"
#include "tao/PortableInterceptor.h"
 
#if TAO_HAS_INTERCEPTORS == 1
#include "tao/RequestInfo_Util.h"
#include "tao/ClientRequestInfo.h"
#include "tao/ClientInterceptorAdapter.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#if defined (__BORLANDC__)
#pragma option -w-rvl -w-rch -w-ccc -w-aus -w-sig
#endif /* __BORLANDC__ */*/


ANYTABLEPOINTERS Asn1AnyTable::anyPointers;

//ANYTABLEPOINTERS ConverterFuncs::m_specialToGeneric;
//ANYTABLEPOINTERS ConverterFuncs::m_genericToSpecial;



///////////////////////////////////////////////////
/// MoValue


MoValue::MoValue()
{}

MoValue::~MoValue()
{}

MoValue * MoValue::clone()
{
    return new MoValue;
}

const CORBA::TypeCode_ptr MoValue::GetTypeCode()
{
    return (CORBA::TypeCode_ptr)NULL;
}


MoValue* MoValue::CreateNewInstance()
{
    return new MoValue;
}


void MoValue::toCorba(CORBA::Any& any)
{
    return ;
}

void MoValue::fromCorba(const CORBA::Any& any)
{
    return ;
}

const char* MoValue::toString()
{

    return "";
}

void MoValue::fromString(const char* value)
{}


///////////////////////////////////////////////////
/// RealASN1
RealASN1::RealASN1()
{
    _value = 0.0;
}


RealASN1::~RealASN1()
{}


//Padma
RealASN1::RealASN1( RealASN1& src)
{
    _value = src._value;
    return ;
}

RealASN1& RealASN1::operator = ( RealASN1& src)
{
    if (&src == this)
        return * this;

    this->_value = src._value;
    return *this;
}

MoValue * RealASN1::clone()
{
    RealASN1 *p = new RealASN1();
    p->_value = this->_value;
    return p;
}
//Padma


CORBA::Double RealASN1::toCorbaEquivalent()
{
    return (CORBA::Double)_value;
}
void RealASN1::fromCorbaEquivalent(const CORBA::Double val)
{
    _value = val;
}

void RealASN1::toCorba(CORBA::Any &any)
{
    any <<= (CORBA::Double)_value;
}

void RealASN1::fromCorba(const CORBA::Any& any)
{
    any >>= _value;
}


/////////////////////////////////////////////////
/// StringASN1
StringASN1::StringASN1()
{
    Init();
}


StringASN1::~StringASN1()
{
    if (_value)
        delete _value;
}

void StringASN1::Init()
{
    _value = NULL;
}

void StringASN1::SetValue(const char* in)
{
    if (_value)
        delete _value;

    _value = new char[strlen(in) + 1];

    strcpy(_value, in);
}

const char* StringASN1::GetValue()
{
    return _value;
}



//Padma
StringASN1::StringASN1( StringASN1& src)
{
    if (_value)
        delete _value;

    _value = new char[strlen(src.GetValue()) + 1];

    strcpy(_value, src.GetValue());
    return ;
}

StringASN1& StringASN1::operator = ( StringASN1& src)
{
    if (&src == this)
        return * this;

    if (_value)
        delete _value;
    _value = new char[strlen(src.GetValue()) + 1];

    strcpy(this->_value, src.GetValue());
    return *this;
}


MoValue * StringASN1::clone()
{
    StringASN1 *p = new StringASN1();
    if (p)
    {
        p->_value = new char[strlen(this->GetValue()) + 1];

        strcpy(p->_value, this->GetValue());
    }
    return p;
}
//Padma

void StringASN1::toCorba(CORBA::Any& any)
{
    //any <<= CORBA::string_dup(_value);
    //SR: Modified CORBA::string to const char*
    any <<= (const char*)_value;
}


void StringASN1::fromCorba(const CORBA::Any& any)
{
    if (_value)
        delete _value;
    const char* newValue;
    //CORBA::Char* newValue;
    any >>= newValue;

    if (newValue)
    {
        _value = new char[strlen(newValue) + 1];
        strcpy(_value, newValue);
    }
};

char* StringASN1::toCorbaEquivalent()
{
    return CORBA::string_dup(_value);
}

void StringASN1::fromCorbaEquivalent(const CORBA::Char* from)
{
    if (_value)
        delete _value;
    _value = new char[strlen(from) + 1];
    strcpy(_value, from);
}


StringASN1* StringASN1::CreateNewInstance()
{
    return new StringASN1;
}





/////////////////////////////////////////////////
/// IntegerASN1
IntegerASN1::IntegerASN1()
{}
;
IntegerASN1::IntegerASN1(long value)
{
    _value = value;
}

void IntegerASN1::SetValue(long value)
{
    _value = value;
}


//Padma
IntegerASN1::IntegerASN1( IntegerASN1& src)
{
    _value = src._value;
    return ;
}

IntegerASN1& IntegerASN1::operator = ( IntegerASN1& src)
{
    if (&src == this)
        return * this;

    _value = src._value;
    return *this;
}


MoValue * IntegerASN1::clone()
{
    IntegerASN1 *p = new IntegerASN1();
    if (p)
    {
        p->_value = this->_value;
    }
    return p;
}
//Padma

void IntegerASN1::toCorba(CORBA::Any& any)
{
    any <<= (CORBA::Long)_value;
}

void IntegerASN1::fromCorba(const CORBA::Any& any)
{
    any >>= (CORBA::Long)_value;
    //any >>= _value;
};

CORBA::Long IntegerASN1::toCorbaEquivalent()
{
    return (CORBA::Long)_value;
}

void IntegerASN1::fromCorbaEquivalent(const CORBA::Long from)
{
    _value = from;
}

IntegerASN1* IntegerASN1::CreateNewInstance()
{
    return new IntegerASN1;
}


//////////////////////////////////////////////////////////
/// NullASN1
NullASN1::NullASN1()
{
    _value = 0;
}


NullASN1::~NullASN1()
{}


//Padma
NullASN1::NullASN1( NullASN1& src)
{
    _value = src._value;
    return ;
}

NullASN1& NullASN1::operator = ( NullASN1& src)
{
    if (&src == this)
        return * this;

    _value = src._value;
    return *this;
}


MoValue * NullASN1::clone()
{
    NullASN1 *p = new NullASN1();
    if (p)
    {
        p->_value = this->_value;
    }
    return p;
}
//Padma

void NullASN1::toCorba(CORBA::Any& any)
{
    any <<= CORBA::Any::from_char((CORBA::Char)_value);
}

void NullASN1::fromCorba(const CORBA::Any& any)
{
    any >>= CORBA::Any::to_char((CORBA::Char)_value);
};

CORBA::Char NullASN1::toCorbaEquivalent()
{
    return (CORBA::Char)_value;
}

void NullASN1::fromCorbaEquivalent(const CORBA::Char from)
{
    _value = from;
}

NullASN1* NullASN1::CreateNewInstance()
{
    return new NullASN1;
}

//////////////////////////////////////////////////
/// BooleanASN1
BooleanASN1::BooleanASN1()
{
    _value = 0;
};


//Padma
BooleanASN1::BooleanASN1( BooleanASN1& src)
{
    _value = src._value;
    return ;
}

BooleanASN1& BooleanASN1::operator = ( BooleanASN1& src)
{
    if (&src == NULL)
        return * this;
    if (&src == this)
        return *this;

    _value = src._value;
    return *this;
}



MoValue * BooleanASN1::clone()
{
    BooleanASN1 *p = new BooleanASN1();
    if (p)
    {
        p->_value = this->_value;
    }
    return p;
}
//Padma

void BooleanASN1::toCorba(CORBA::Any& any)
{
    CORBA::Boolean tmp = (CORBA::Boolean)_value;
    any <<= CORBA::Any::from_boolean(tmp);
}

void BooleanASN1::fromCorba(const CORBA::Any& any)
{
    CORBA::Boolean tmp;
    any >>= CORBA::Any::to_boolean(tmp);
    _value = (bool)tmp;
};

CORBA::Char BooleanASN1::toCorbaEquivalent()
{
    return (CORBA::Boolean)_value;
}

void BooleanASN1::fromCorbaEquivalent(const CORBA::Boolean from)
{
    _value = from;
}
BooleanASN1* BooleanASN1::CreateNewInstance()
{
    return new BooleanASN1;
}

