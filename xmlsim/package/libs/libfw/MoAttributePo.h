// Copyright (c) 1995,1996 by ObjectSpace, Inc.  All rights reserved.
// Email: sales@objectspace.com, ccs.support@objectspace.com
// Last Modified: $Date:   04 Feb 1999 14:38:08  $
#ifndef _MO_ATTRIBUTE_PO_
#define _MO_ATTRIBUTE_PO_

// #include <ospace/std/iostream>
// #include <ospace/thread.h>
/*
#ifndef OS_NO_STD_NAMESPACE
  using namespace OS_STD_NAMESPACE;
#endif
*/

//#include <string>
#include "BaseTypesASN1.h"

// #include "MoLong.h"
//#include "MoString.h"
//#include "MoAlarmStatus.h"
// #include "MoCurrentProblem.h"
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

class MoAttributePo  {

public:
#ifndef _ODI_OSSG_
	/**
	 * The default constructor .....
	 */
	MoAttributePo() : _value(0) {}
	
	/**
	 * The constructor .....
	 */
	MoAttributePo(const string& name, MoValue* value) : _name(name) , _value(value)  {}

	~MoAttributePo() 
	{
		if(_value)
			delete _value;
	}

	/**
	 * Returns the name
	 **/
	string getName() const { return _name;}

	/**
	 * returns value
	 * suppose to return const MoValue&  or const MoValue* 
	 * because it is point to internal structure.
	 * but currently in MoValue const methods are not defined.
	 **/
	MoValue *getValue() const { return _value;}


	/**
	 * sets name
	 **/
	void setName(const string& name) { _name = name; }

	/**
	 * sets value
	 **/
	void setValue(MoValue *value) { _value = value; }

	friend bool operator<( const MoAttributePo& src, const MoAttributePo& dest  ) {return false;}



private:
	string				_name;
	MoValue*			_value;

	// Copy constructor and = operator are not allowed
	// temporary when clone() is implemented in MoValue this will be uncommented

	MoAttributePo(const MoAttributePo& obj) {}

    MoAttributePo& operator=(const MoAttributePo& n) { return *this; }
#endif
};


#endif
