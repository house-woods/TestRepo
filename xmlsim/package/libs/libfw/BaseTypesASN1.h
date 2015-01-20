#ifndef _INCLUDE_BASETYPESASN1_H_
#define _INCLUDE_BASETYPESASN1_H_

#pragma warning(disable:4786)

#include <string>
#include <map>

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
#ifndef _ODI_OSSG_
#include "ASN1Types.hh"
#endif

//using namespace std;

typedef map<string, int> STRING2INT;

typedef map<string, void*> ANYTABLEPOINTERS;
enum DataType {NONE, DT_NULL, DT_CHAR, DT_STRING, DT_INTEGER, DT_DOUBLE, DT_BOOL, DT_DATE, DT_VECTOR};



typedef map<string, long> STRING2LONG;
typedef map<long, string> LONG2STRING;

//the mother of all ASN1 classes
class MoValue
{
#ifndef _ODI_OSSG_
public:

    MoValue();
    virtual ~MoValue();
    //#ifndef _ODI_OSSG_
    virtual const CORBA::TypeCode_ptr GetTypeCode();
    //#endif


    static MoValue* CreateNewInstance();

    //#ifndef _ODI_OSSG_
    virtual void toCorba(CORBA::Any& any);

    virtual void fromCorba(const CORBA::Any& any);

    virtual void deleteAny(CORBA::Any* any)
    {
        delete any;
    }
    //#endif
    // clone:
    // Makes an identical clone of the MoValue
    // The user is responsible for deallocating  the MoValue.
    virtual MoValue * clone() ; //Padma

    const char* toString();
    void fromString(const char* value);

    virtual DataType getDataType()
    {
        return NONE;
    };
    virtual string toDbFormat()
    {
        return "";
    };



protected:
    MoValue( MoValue& src)
    {} //Padma
    MoValue& operator = ( MoValue& src)
    {
        return * this;
    } //Padma
#endif
};

typedef MoValue* (*CreateInstFunction)();
typedef MoValue BaseASN1;

class Asn1AnyTable
{
protected:

    static ANYTABLEPOINTERS anyPointers;

public:

    //   Asn1AnyTable();

    static BaseASN1* GetTrueType(const string& oid)
    {
        CreateInstFunction func;

        ANYTABLEPOINTERS::iterator iter = anyPointers.find(oid);
        if (iter != anyPointers.end())
        {
            func = (CreateInstFunction)(*iter).second;
            return func();
        }
        else
            return NULL;
    }

    static int RegisterAnyPointer(const string& oid, void* pointer)
    {
        anyPointers.insert(ANYTABLEPOINTERS::value_type(oid, pointer ));
        return 0; //dummy
    }
};


class RealASN1: public BaseASN1
{
protected:


public:
    double _value; //Padma
    RealASN1();
    virtual ~RealASN1();

    RealASN1( RealASN1& src);
    RealASN1& operator = ( RealASN1& src);

    // clone:
    // Makes an identical clone of the RealASN1
    // The user is responsible for deallocating  the RealASN1.
    virtual MoValue * clone() ; //Padma
#ifndef _ODI_OSSG_

    CORBA::Double toCorbaEquivalent();
    void fromCorbaEquivalent(const CORBA::Double val);

    virtual void toCorba(CORBA::Any &any);

    virtual void fromCorba(const CORBA::Any& any);
#endif
};

class StringASN1: public BaseASN1
{

protected:


public:
    char* _value; //Padma
    StringASN1();

    virtual ~StringASN1();

    virtual void Init();
    void SetValue(const char* in);
    const char* GetValue();


    StringASN1( StringASN1& src);
    StringASN1& operator = ( StringASN1& src);

    // clone:
    // Makes an identical clone of the StringASN1
    // The user is responsible for deallocating  the StringASN1.
    virtual MoValue * clone() ; //Padma

#ifndef _ODI_OSSG_

    virtual void toCorba(CORBA::Any& any);
    virtual void fromCorba(const CORBA::Any& any);

    char* toCorbaEquivalent();
    void fromCorbaEquivalent(const CORBA::Char* from);
#endif

    static StringASN1* CreateNewInstance();


};
typedef StringASN1 IA5StringASN1;
typedef StringASN1 NumericStringASN1;
typedef StringASN1 PrintableStringASN1;
typedef StringASN1 TeletexStringASN1;
typedef StringASN1 T61StringASN1;
typedef StringASN1 VideoTexStringASN1;
typedef StringASN1 VisibleStringASN1;
typedef StringASN1 GeneralizedTimeASN1;
typedef StringASN1 UTCTimeASN1;

class IntegerASN1: public BaseASN1
{
protected:


public:
    CORBA::Long _value; //Padma
    IntegerASN1();
    IntegerASN1(long value);


    void SetValue(long value);


    IntegerASN1( IntegerASN1& src);
    IntegerASN1& operator = ( IntegerASN1& src);

    // clone:
    // Makes an identical clone of the IntegerASN1
    // The user is responsible for deallocating  the IntegerASN1.
    virtual MoValue * clone() ; //Padma
#ifndef _ODI_OSSG_

    virtual void toCorba(CORBA::Any& any);

    virtual void fromCorba(const CORBA::Any& any);
    CORBA::Long toCorbaEquivalent();

    void fromCorbaEquivalent(const CORBA::Long from);
#endif

    static IntegerASN1* CreateNewInstance();

};


class NullASN1: public BaseASN1
{
protected:


public:
    char _value; //Padma
    NullASN1();


    virtual ~NullASN1();

    NullASN1( NullASN1& src);
    NullASN1& operator = ( NullASN1& src);

    // clone:
    // Makes an identical clone of the NullASN1
    // The user is responsible for deallocating  the NullASN1.
    virtual MoValue * clone() ; //Padma
#ifndef _ODI_OSSG_

    virtual void toCorba(CORBA::Any& any);
    virtual void fromCorba(const CORBA::Any& any);

    CORBA::Char toCorbaEquivalent();

    void fromCorbaEquivalent(const CORBA::Char from);
#endif

    static NullASN1* CreateNewInstance();
};

class BooleanASN1: public BaseASN1
{
protected:


public:
    bool _value; //Padma
    BooleanASN1();

    BooleanASN1( BooleanASN1& src);
    BooleanASN1& operator = ( BooleanASN1& src);

    // clone:
    // Makes an identical clone of the BooleanASN1
    // The user is responsible for deallocating  the BooleanASN1.
    virtual MoValue * clone() ; //Padma
#ifndef _ODI_OSSG_

    virtual void toCorba(CORBA::Any& any);
    virtual void fromCorba(const CORBA::Any& any);

    CORBA::Char toCorbaEquivalent();

    void fromCorbaEquivalent(const CORBA::Boolean from);
#endif

    static BooleanASN1* CreateNewInstance();
};
#endif
