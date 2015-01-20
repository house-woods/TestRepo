#ifndef MOCORBALIST_H
#define MOCORBALIST_H

/*************************************************************************
* COPYRIGHT 2005 Tellabs Operations, Inc. 
* ALL RIGHTS RESERVED 
* No Part of this document may be reproduced without 
* the written permission of Tellabs Operations, Inc., 
* 1415 Diehl Road., Naperville, IL 60563
*************************************************************************/
#pragma once

#include <list>
#include "BaseTypesASN1.h"

/**
 * template to generate C++ wrapper for the Corba sequence<> structures.
 *
 * typename T         : C++ wrapper for the Corba type
 * typename TList     : Corba sequence<> typename
 * typename TList_var : Corba generate sequence<> typename
 *
 * Example:
 *    in "MyType.idl"
 *        struct MyCorba
 *        {
 *        };
 *        typedef sequence<MyCorba>   MyCobraList;
 *
 *
 *    in "MyCorbaCpp.cpp" that wraps MyCorba into/from C++
 *        class MyCobraCpp : public MoValue
 *        {
 *            MyCorba_var * _pCorbaEquivalent;
 *        };
 *
 *
 *    to define the sequence:
 *    in "MyCorbaListCpp.h"
 *        typedef MoCorbaList<MyCorbaCpp, MyCobraList, MyCorbaList_var>  MyCorbaListCpp;
 *
 *
 *    to access elements of the list
 *        MyCorbaListCpp myList;
 *        myList._dataList;
 */

template<typename T, typename TList, typename TList_var> struct MoCorbaList : public MoValue
{
    typedef std::list<T *> listType;

    MoCorbaList()
    {
    }

    virtual ~MoCorbaList()
    {
    }

    MoCorbaList& operator=(TList& src)
    {
        return *this;
    }

    static MoCorbaList * CreateNewInstance()
    {
        return new MoCorbaList;
    }

    void toCorba(CORBA::Any& any)
    {
        any <<= toCorbaEquivalent();
    }

    void fromCorba(const CORBA::Any& any)
    {
        TList * dataList;
        any >>= dataList;
        for (unsigned long i = 0; i < dataList->length(); i++)
        {
            T * data = new T();
            *data = (*dataList)[i];
            _dataList.push_back(data);
        }
    }

    TList& toCorbaEquivalent()
    {
        if (!m_pCorbaEquivalent._retn())
        {
            m_pCorbaEquivalent = new TList();
        }

        int i = 0;
        m_pCorbaEquivalent->length(_dataList.size());
        for (std::list<T *>::iterator p = _dataList.begin(); p != _dataList.end(); ++p)
        {
            m_pCorbaEquivalent[i++] = (*p)->toCorbaEquivalent();
        }

        return m_pCorbaEquivalent;
    }

    // This is the list itself
    //std::list<T *> _dataList;
    listType _dataList;

    TList_var m_pCorbaEquivalent;
};
#endif //MOCORBALIST_H
