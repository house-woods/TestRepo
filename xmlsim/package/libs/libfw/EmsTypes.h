/*******************************************************************************
* COPYRIGHT 1999 - 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: EmsTypes
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   EmsTypes.h  $
* Version: $Revision:   1.1  $
* Description: 
* Base Types for MoValue for Ems defined Attributes
* 
* Revision History:
*
*******************************************************************************/


#ifndef _EMSTYPES_H_
#define _EMSTYPES_H_

#include "BaseTypesASN1.h"
#include "ems.hh"
#include "Node.hh"
#include <string>
#include <list>


class PositionType : public MoValue
{

protected:

	EMS::Position _value;

public:

	PositionType() {}
	PositionType(const PositionType& pt) { _value = pt._value; }
	void setValue(const EMS::Position* value) { _value = *value; }
	EMS::Position getValue() { return _value; }
	virtual void fromCorba(const CORBA::Any& any) 
	{
		EMS::Position* pos;
		any >>= pos;
		_value = *pos;
	}

	virtual void toCorba(CORBA::Any& any)
	{
		any <<= _value;
	}


	MoValue* clone() 
	{ // Added
		PositionType* pt = new PositionType();
		pt->_value = _value;
		return pt;
	}

	static PositionType* CreateNewInstance()
	{
		return new PositionType();
	}

};

class EmsStringType : public MoValue
{

protected:
	string _value;


public:

	EmsStringType() {}
	EmsStringType(const EmsStringType& type) { _value = type._value; }
	void setValue(const string& value) { _value = value; }
	const string& getValue() { return _value; }
	virtual void fromCorba(const CORBA::Any& any) 
	{
		const char*  data;
		any >>= data;
		_value = data;
	}

	virtual void toCorba(CORBA::Any& any)
	{
		any <<= _value.c_str();
	}

	static EmsStringType* CreateNewInstance()
	{
		return new EmsStringType();
	}

	MoValue* clone() 
	{
		return new EmsStringType(*this);
	}

};


class EmsLongType : public MoValue
{

protected:

	CORBA::Long _value;

public:

	EmsLongType() {}
	EmsLongType(const EmsLongType& lt) { _value = lt._value; }
	void setValue(long value) { _value = value; }
	long getValue() { return _value; }
	virtual void fromCorba(const CORBA::Any& any) 
	{
		any >>= _value;
	}

	virtual void toCorba(CORBA::Any& any)
	{
		any <<= _value;
	}

	static EmsLongType* CreateNewInstance()
	{
		return new EmsLongType();
	}

	MoValue* clone() 
	{
		return new EmsLongType(*this);
	}

};


class EmsAttributeHolderType : public MoValue
{

protected:

	EMS::EmsAttribute _value;


public:

	EmsAttributeHolderType() {}
	EmsAttributeHolderType(const EmsAttributeHolderType& ht) { _value = ht._value; }
	void setValue(const EMS::EmsAttribute& value) { _value = value; }
	EMS::EmsAttribute getValue() { return _value; }

	static EmsAttributeHolderType* CreateNewInstance()
	{
		return new EmsAttributeHolderType();
	}

	MoValue* clone() 
	{
		return new EmsAttributeHolderType(*this);
	}

};

class NodeSummaryListType : public MoValue
{
public:
	NodeSummaryListType() {}
	NodeSummaryListType(const NodeSummaryListType& rhs) : _summaryList(rhs._summaryList) {}

	void setValue(const Node::NodeSummaryList& summaryList) { _summaryList = summaryList; }
	Node::NodeSummaryList getValue() { return _summaryList; }

	static NodeSummaryListType* CreateNewInstance() { return new NodeSummaryListType; }
	MoValue* clone() { return new NodeSummaryListType(*this); }
	
	//virtual void fromCorba(const CORBA::Any& any) {	any >>= _summaryList; }
	virtual void toCorba(CORBA::Any& any) { any <<= _summaryList;	}
	
protected:
	Node::NodeSummaryList _summaryList;
};


class EmsStringListType : public MoValue
{

protected:
	list<string> _value;
   EMS::StringList_var  m_pCorbaEquivalent;

public:
	EmsStringListType() {}

	EmsStringListType(const EmsStringListType& type) { _value = type._value; }

	void setValue(const list<string>& value) { _value = value; }
	void addItem(const string& item) { _value.push_back(item); }

	const list<string>& getValue() { return _value; }
	virtual void fromCorba(const CORBA::Any& any) 
	{
		EMS::StringList*  data;
		any >>= data;
      for (unsigned long j = 0; j < data->length(); j++)
      {
		   _value.push_back((*data)[j].in());
      }
	}

	virtual void toCorba(CORBA::Any& any)
	{
		any <<= toCorbaEquivalent();
	}

   EMS::StringList& toCorbaEquivalent ()
   {
      if (!m_pCorbaEquivalent._retn())
         m_pCorbaEquivalent = new EMS::StringList();

      int j = 0;
      m_pCorbaEquivalent->length(_value.size());
      list<string>::iterator p;
      for(p = _value.begin(); p != _value.end(); p++)
      {
          m_pCorbaEquivalent[j++] = (*p).c_str();
      }

      return m_pCorbaEquivalent;
   }

	static EmsStringListType* CreateNewInstance()
	{
		return new EmsStringListType();
	}

	MoValue* clone() 
	{
		return new EmsStringListType(*this);
	}

};


#endif
