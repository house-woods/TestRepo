/***********************************************************************************************
* COPYRIGHT $Date:   04 Feb 1999 14:48:52  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: 
* Type: Include File
* Originator: $Author:   dtchalak  $
* File: $Workfile:   PmanagedObject.h  $
* Version: $Revision:   1.1  $
* Description: 
* Persistent managed Object class.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/btm/source/cm/server_proj/db/PmanagedObject.h_v  $
 * 
 *    Rev 1.1   04 Feb 1999 14:48:52   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 13:21:14   dtchalak
 *  
 * 
 *    Rev 1.1   Nov 09 1998 17:02:00   dtchalak
 *  
 * 
 *    Rev 1.0   Nov 05 1998 11:36:22   dtchalak
 *  
*
* File Dependencies: 
* 
************************************************************************************************/

#ifndef _PMANAGED_OBJECT_
#define _PMANAGED_OBJECT_
#include <ostore/ostore.hh>
#include <ostore/coll.hh>
#include <ostore/coll/dict_pt.hh>
#include "PDbObject.h"
#include "OString.h"
#include <string>
#include <list>


class MoAttributePo;

class PManagedObject  : public PDbObject {
public:


	static os_typespec* get_os_typespec();  //  Added  type specifier for objectstore.
	
	PManagedObject(); 
	virtual							~PManagedObject(); 

	void							addChild(PManagedObject *mo);
	void							removeChild(PManagedObject *mo); 
	string							getParentMoi() const;
	void							setAttribute(MoAttributePo *mo);
	list<PManagedObject*>*			getChildren() const;

	// inline member functions...
	string	getMoc() const						{ return _moc.getString();}
	string	getMoi() const						{ return _moi.getString();}
	long	getTimeStamp()	const				{ return _timeStamp;}
	void	setMoc(string moc)					{ _moc.setString(moc.c_str());}
	void	setMoi(string moi)					{ _moi.setString(moi.c_str());}
	void	setTimeStamp(long timeStamp)		{ _timeStamp = timeStamp; }
	void	setValidState(bool validState)		{ _validState = validState; }
	void	lock()								{ _lock = true;}
	void	unlock()							{ _lock = false;}
	bool	isLocked()	const					{ return _lock;}
	PManagedObject *getParent()	const			{ return _parent;}
	bool	getValidState() const				{ return _validState; }
	
	
	/***
	 * These methods have to be implemented by subclasses...
	 */
	// ATTENTION need to be define const getAttribute and getAttributes
	virtual MoAttributePo	*getAttribute(string name);
	virtual void			getAttributes(list<MoAttributePo*>& attributes);
	virtual void			setAttributes(const list<MoAttributePo*>& attributes);
	  
protected:

	OString						_moc;
	OString						_moi;
	long						_timeStamp;
	bool						_lock;
	PManagedObject*				_parent;
	os_List<PManagedObject*>*	_children;
	bool						_validState;

private:

	// Copy constructor and = operator are not allowed
	PManagedObject(const PManagedObject& obj) {}

    PManagedObject& operator=(const PManagedObject& n) { return *this; }


};

#endif

