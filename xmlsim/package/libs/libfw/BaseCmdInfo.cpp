/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:52  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: Data Holder Class
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseCmdInfo.cpp  $
* Version: $Revision:   1.5  $
* Description:
* Data Holder Class.
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/src/BaseCmdInfo.cpv  $

   Rev 1.5   Oct 27 1999 14:35:52   dtchalak
Query Interface updates.

   Rev 1.4.1.0   Sep 20 1999 16:18:24   dtchalak
Just to create branch.

   Rev 1.4   02 Sep 1999 15:43:40   dtchalak
New interface support added.

   Rev 1.3   21 Jul 1999 10:28:44   dtchalak
Multiple Ne support included.

   Rev 1.2   14 Jun 1999 16:15:14   dtchalak
Updates related to new CM.idl and other details.

   Rev 1.1   04 May 1999 19:03:04   dtchalak
Comments added.

   Rev 1.0   05 Apr 1999 12:35:16   bacuff


   Rev 1.1   04 Feb 1999 14:41:54   dtchalak
ready for IP-5

   Rev 1.0   22 Jan 1999 12:25:08   dtchalak

//
//    Rev 1.3   Dec 16 1998 12:46:50   gfreiboth
// changes related to new converter code from UHC
//
//    Rev 1.2   Nov 09 1998 16:58:36   dtchalak
//
//
//    Rev 1.1   Nov 05 1998 11:20:52   dtchalak
//
//
//    Rev 1.0   Oct 16 1998 17:30:34   dtchalak
//
**********************************************************************************************/





#include "BaseCmdInfo.h"
#include "MoAttributePo.h"
#include "tutil.h"
#include "Server_i.h"
#include "NetworkElement.h"

using namespace EMS_FW;

const string BaseCmdInfo::FM_ALARM_EVENT = "FM_ALARM_EVENT";
const string BaseCmdInfo::CM_AVC_EVENT = "CM_AVC_EVENT";
const string BaseCmdInfo::CM_SC_EVENT = "CM_SC_EVENT";
const string BaseCmdInfo::CM_CREATE_EVENT = "CM_CREATE_EVENT";
const string BaseCmdInfo::CM_DELETE_EVENT = "CM_DELETE_EVENT";
const string BaseCmdInfo::CM_AUTODISCOVERY = "CM_AUTODISCOVERY";
const string BaseCmdInfo::CM_FULL_AUTODISCOVERY = "CM_FULL_AUTODISCOVERY";
const string BaseCmdInfo::CM_ONDEMAND_AUTODISCOVERY = "CM_ONDEMAND_AUTODISCOVERY";
const string BaseCmdInfo::CM_REQUESTED_AUTODISCOVERY = "CM_REQUESTED_AUTODISCOVERY";
const string BaseCmdInfo::CM_SUSPEND_EVENTQUEUE_DELAY_DETECTION = "CM_SUSPEND_EVENTQUEUE_DELAY_DETECTION";
const string BaseCmdInfo::CM_RESUME_EVENTQUEUE_DELAY_DETECTION = "CM_RESUME_EVENTQUEUE_DELAY_DETECTION";
const string BaseCmdInfo::CM_ABORT = "CM_ABORT";
const string BaseCmdInfo::CM_COMMIT = "CM_COMMIT";
const string BaseCmdInfo::CM_ERASE_DB = "CM_ERASE_DB";
const string BaseCmdInfo::CM_REBUILD_DB = "CM_REBUILD_DB";
const string BaseCmdInfo::CM_COMMIT_AUTODISCOVERY = "CM_COMMIT_AUTODISCOVERY";
const string BaseCmdInfo::CM_INVALID_EVENT = "CM_INVALID_EVENT";
const string BaseCmdInfo::CM_SCOPED_AUTODISCOVERY = "CM_SCOPED_AUTODISCOVERY";
const string BaseCmdInfo::CM_SCOPED_AUTODISCOVERY_COMPLETED = "CM_SCOPED_AUTODISCOVERY_COMPLETED";
const string BaseCmdInfo::CM_SCOPED_AUTODISCOVERY_FAILED = "CM_SCOPED_AUTODISCOVERY_FAILED";
const string BaseCmdInfo::CM_SCOPED_SYNCH_AUTODISCOVERY = "CM_SCOPED_SYNCH_AUTODISCOVERY";
const string BaseCmdInfo::CM_LEVEL_RESYNCHRONIZATION = "CM_LEVEL_RESYNCHRONIZATION";
const string BaseCmdInfo::CM_DISCOVERY_EVENT = "CM_DISCOVERY_EVENT";


BaseCmdInfo::BaseCmdInfo()
{}

BaseCmdInfo::BaseCmdInfo(const string& moc, const string& moi,
                         const list<MoAttributePo*>& attrib,
                         const long& timestamp,
                         const string& type)
        : _moc(moc) , _moi(moi), _timestamp(timestamp),
        _type(type)
{
    _attrib = attrib;
    _addlInfo = "";
}

BaseCmdInfo::BaseCmdInfo(const string& moc, const string& moi,
                         const string& type , const long& timestamp)
        : _moc(moc) , _moi(moi), _type(type),
        _timestamp(timestamp)
{}

BaseCmdInfo::BaseCmdInfo(const string& moc, const string& moi,
                         const string& type,
                         const long& timestamp,
                         const list<MoAttributePo*>& attrib)
        : _moc(moc) , _moi(moi), _type(type), _timestamp(timestamp)
{
    _attrib = attrib;
    _addlInfo = "";
}


BaseCmdInfo::BaseCmdInfo(const BaseCmdInfo& n)
{}

BaseCmdInfo& BaseCmdInfo::operator=(const BaseCmdInfo & n)
{
    return * this;
}

void BaseCmdInfo::deleteAllAttributes()
{
    Tutil::deleteAttributeList(_attrib);
}

BaseCmdInfo::~BaseCmdInfo()
{
    deleteAllAttributes();
}


void BaseCmdInfo::addAttribute(MoAttributePo* atr)
{
    _attrib.push_back(atr);
}

void BaseCmdInfo::setAttributes(const list<MoAttributePo*>& attrib)
{
    _attrib = attrib;
}

const list<MoAttributePo*>& BaseCmdInfo::getAttributes() const
{
    return _attrib;
}

const MoAttributePo* BaseCmdInfo::getAttribute(const string& name) const
{
    return Tutil::getAttribute(name, _attrib);
}

const string& BaseCmdInfo::getMoc() const
{
    return _moc;
}

void BaseCmdInfo::setMoc(const string& moc)
{
    _moc = moc;
}

const string& BaseCmdInfo::getMoi() const
{
    return _moi;
}

void BaseCmdInfo::setMoi(const string& moi)
{
    _moi = moi;
}

const string& BaseCmdInfo::getType() const
{
    return _type;
}

const string& BaseCmdInfo::getOriginalEventType() const
{
    return _originalEventType;
}

void BaseCmdInfo::setType(const string& type)
{
    _type = type;
}

void BaseCmdInfo::setOriginalEventType(const string& type)
{
    _originalEventType = type;
}

long BaseCmdInfo::getTimeStamp() const
{
    return _timestamp;
}

void BaseCmdInfo::setTimeStamp( const long& timestamp)
{
    _timestamp = timestamp;
}

const string& BaseCmdInfo::getAddlInfo() const
{
    return _addlInfo;
}

void BaseCmdInfo::setAddlInfo(const string& addlInfo)
{
    _addlInfo = addlInfo;
}



bool BaseCmdInfo::removeAttribute(MoAttributePo* const& attr)
{
    _attrib.remove(attr);
    return true;
}

bool BaseCmdInfo::removeAttribute(const string& attrName)
{
    Tutil::deleteAttribute(attrName, _attrib);

    return true;
}

bool BaseCmdInfo::removeAttributes(const list<MoAttributePo*>& atrlist)
{
    list<MoAttributePo*>::const_iterator p;
    for (p = atrlist.begin(); p != atrlist.end(); p++)
    {
        removeAttribute(*p);
    }
    return true;
}


void BaseCmdInfo::setNeId(const string& neId)
{
    _neId = neId;
}


const string& BaseCmdInfo::getNeId() const
{
    return _neId;
}

void BaseCmdInfo::reinitialize(const BaseCmdInfo* orig)
{
    setMoi(orig->getMoi());
    setMoc(orig->getMoc());
    setType(orig->getType());
    setTimeStamp(orig->getTimeStamp());
    setNeId(orig->getNeId());
    setAddlInfo(orig->getAddlInfo());
    deleteAllAttributes();
    MoAttributePo* attribute;
    MoValue* value;
    list<MoAttributePo*>::const_iterator p;
    const list<MoAttributePo*>& attribList = orig->getAttributes();
    for (p = attribList.begin(); p != attribList.end(); p++)
    {
        // clone all MoAttributes
        attribute = new MoAttributePo();
        attribute->setName((*p)->getName());
        value = (*p)->getValue();
        if (value)
            attribute->setValue(value->clone());
        else
            attribute->setValue(NULL);
        addAttribute(attribute);
    }
}

BaseCmdInfo* BaseCmdInfo::clone() const
{
    BaseCmdInfo* newInfo = new BaseCmdInfo();
    newInfo->reinitialize(this);
    return newInfo;
}

bool BaseCmdInfo::isNeActive() const
{
    NetworkElement* ne = (NetworkElement*)Server_i::instance()->locateNetworkElement(_neId);
    if (ne)
    {
        if (ne->isReadOnly())
            return false;
        else
            return true;
    }

    return false;
}
