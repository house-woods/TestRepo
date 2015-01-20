
#ifndef _EMSMOVALUET_H_
#define _EMSMOVALUET_H_

#include "BaseTypesASN1.h"

template <class T>
class EmsMoValueT : public MoValue
{
protected:
    T _value;

public:
    EmsMoValueT (void)
    {}
    void setValue(const T *value)
    {
        _value = *value;
    }
    const T& getValue()
    {
        return _value;
    }
    void toCorba(CORBA::Any& any)
    {
        any <<= _value;
    }
    void fromCorba(const CORBA::Any& any)
    {
        T *ptr;
        any >>= ptr;
        _value = *ptr;
    }

    static EmsMoValueT<T> *CreateNewInstance()
    {
        return new EmsMoValueT<T>;
    }

    virtual MoValue * clone()
    {
        EmsMoValueT<T> *newInstance = CreateNewInstance();
        newInstance->_value = _value;
        return newInstance;
    }
};

template <>
class EmsMoValueT<CORBA::LongLong> : public MoValue
{
protected:
    CORBA::LongLong _value;

public:
    EmsMoValueT (void)
    {}
    void setValue(const CORBA::LongLong *value)
    {
        _value = *value;
    }
    const CORBA::LongLong& getValue()
    {
        return _value;
    }
    void toCorba(CORBA::Any& any)
    {
        any <<= _value;
    }
    void fromCorba(const CORBA::Any& any)
    {
        any >>= _value;
    }

    static EmsMoValueT<CORBA::LongLong> *CreateNewInstance()
    {
        return new EmsMoValueT<CORBA::LongLong>;
    }

    virtual MoValue * clone()
    {
        EmsMoValueT<CORBA::LongLong> *newInstance = CreateNewInstance();
        newInstance->_value = _value;
        return newInstance;
    }
};

template <>
class EmsMoValueT<CORBA::Long> : public MoValue
{
protected:
    CORBA::Long _value;

public:
    EmsMoValueT (void)
    {}
    void setValue(const CORBA::Long *value)
    {
        _value = *value;
    }
    const CORBA::Long& getValue()
    {
        return _value;
    }
    void toCorba(CORBA::Any& any)
    {
        any <<= _value;
    }
    void fromCorba(const CORBA::Any& any)
    {
        any >>= _value;
    }

    static EmsMoValueT<CORBA::Long> *CreateNewInstance()
    {
        return new EmsMoValueT<CORBA::Long>;
    }

    virtual MoValue * clone()
    {
        EmsMoValueT<CORBA::Long> *newInstance = CreateNewInstance();
        newInstance->_value = _value;
        return newInstance;
    }
};

#endif
