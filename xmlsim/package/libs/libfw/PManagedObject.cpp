/***********************************************************************************************
* COPYRIGHT $Date:   04 Feb 1999 14:47:44  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name:  
* Type: C Source
* Originator: $Author:   dtchalak  $
* File: $Workfile:   PManagedObject.cpp  $
* Version: $Revision:   1.1  $
* Description: 
* 
* 
* Revision History:
*
* $Log:   P:/nsg_dev/btm/source/cm/server_proj/db/PManagedObject.cpv  $
 
    Rev 1.1   04 Feb 1999 14:47:44   dtchalak
 ready for IP-5
 
    Rev 1.0   22 Jan 1999 13:20:50   dtchalak
  
// 
//    Rev 1.1   Nov 09 1998 17:01:30   dtchalak
//  
// 
//    Rev 1.0   Nov 05 1998 11:36:06   dtchalak
//  
**********************************************************************************************/

#include "PManagedObject.h"
#include "MoAttributePo.h"
#if (OS_MAJOR_RELEASE == 6)
 #define MAKE_LIST(T,D) new(D, os_List<T>::get_os_typespec()) os_List<T>
#else
 #define MAKE_LIST(T,D) &os_List<T>::create(D)
#endif


#define MOI_DELIMETER '@'

/**
 * The constructor ....
 */
PManagedObject::PManagedObject()
{
    _parent = 0;
    //_children = &os_List<PManagedObject*>::create(os_segment::of(this));
    _children = MAKE_LIST(PManagedObject*, os_segment::of(this));
}


/**
 * Virtual destructor....
 */
PManagedObject::~PManagedObject()
{
    delete _children;
}



/**
 * Add a new child ...
 */
void PManagedObject::addChild(PManagedObject *mo)
{
    mo->_parent = this;
    _children->insert(mo);
}


/**
 * Remove an existing child ...
 */
void PManagedObject::removeChild(PManagedObject *mo)
{
    mo->_parent = NULL;
    _children->remove
    (mo);
}

/**
 * Return list containing Children MOI
 */
list<PManagedObject*>* PManagedObject::getChildren() const
{
    //TODO: potental memory leak if list is not deleted later
    list<PManagedObject*>* childrenList = new list<PManagedObject*>;
    os_Cursor<PManagedObject*> cursor(*_children);
    for (PManagedObject* pmo = cursor.first(); cursor.more(); pmo = cursor.next())
    {
        childrenList->push_back(pmo);
    }
    // return Ptr to list to avoid Copy constructor for all elements
    // of the list to be involved.
    return childrenList;
}


/**
 * Remove an existing child ...
 */

string PManagedObject::getParentMoi() const
{
    string moi(_moi.getString());
    string moc(_moc.getString());
    size_t pos = moi.find_last_of(MOI_DELIMETER, moi.length());
    if (pos == osi_npos)
    {
        return "";
    }
    else
    {
        return moi.substr(0, pos);
    }
}


/**
 * set attributes ...
 **/

void PManagedObject::setAttribute(MoAttributePo *attribute)
{
    list<MoAttributePo*> list;
    list.insert(list.end(), attribute);
    setAttributes(list);
}


MoAttributePo* PManagedObject::getAttribute(string name)
{
    return NULL;
}

void PManagedObject::getAttributes(list<MoAttributePo*>& attributes)
{}

void PManagedObject::setAttributes(const list<MoAttributePo*>& attributes)
{}





