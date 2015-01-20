/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:56  $ BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
*
* Name: Data Holder Class
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseCmdInfo.h  $
* Version: $Revision:   1.5  $
* Description:
* Data Holder Class.
*
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/inc/BaseCmdInfo.h_v  $
*
*    Rev 1.5   Oct 27 1999 14:35:56   dtchalak
* Query Interface updates.
*
*    Rev 1.4.1.0   Sep 20 1999 16:18:36   dtchalak
* Just to create branch.
*
*    Rev 1.4   02 Sep 1999 15:42:44   dtchalak
* New interface support added.
*
*    Rev 1.3   21 Jul 1999 10:27:40   dtchalak
* Multiple Ne support included.
*
*    Rev 1.2   14 Jun 1999 16:14:12   dtchalak
* Updates related to new CM.idl and other details.
*
*    Rev 1.1   04 May 1999 19:02:06   dtchalak
* Comments added.
*
*    Rev 1.0   05 Apr 1999 12:35:26   bacuff
*
*
*    Rev 1.1   04 Feb 1999 14:42:04   dtchalak
* ready for IP-5
*
*    Rev 1.0   22 Jan 1999 12:25:12   dtchalak
*
*
*    Rev 1.2   Nov 09 1998 16:58:38   dtchalak
*
*
*    Rev 1.1   Nov 05 1998 11:20:52   dtchalak
*
*
*    Rev 1.0   Oct 16 1998 17:30:34   dtchalak
*
**********************************************************************************************/



#ifndef _BaseCmdInfo_H_
#define _BaseCmdInfo_H_

#include <string>
#include <list>

#include "BaseCmd.h"

class MoAttributePo;

namespace EMS_FW
{

// Base data holder for internal transfer of data.
// Class is storing type of the event, moi/Managed Object Instance/
// for the affected element, moc/Managed Object Class/ for the affected element,
// list of attributes for specified element, timestamp representing event time.
// Event time is a time when specific event is happen/ like notification event time/.
class BaseCmdInfo
{

public:

    static const string FM_ALARM_EVENT;
    static const string CM_AVC_EVENT;
    static const string CM_SC_EVENT;
    static const string CM_CREATE_EVENT;
    static const string CM_DELETE_EVENT;
    static const string CM_AUTODISCOVERY;
    static const string CM_FULL_AUTODISCOVERY;
    static const string CM_ONDEMAND_AUTODISCOVERY;
    static const string CM_REQUESTED_AUTODISCOVERY;
    static const string CM_SUSPEND_EVENTQUEUE_DELAY_DETECTION;
    static const string CM_RESUME_EVENTQUEUE_DELAY_DETECTION;
    static const string CM_ABORT;
    static const string CM_COMMIT;
    static const string CM_ERASE_DB;
    static const string CM_REBUILD_DB;
    static const string CM_COMMIT_AUTODISCOVERY;
    static const string CM_INVALID_EVENT;
    static const string CM_SCOPED_AUTODISCOVERY;
    static const string CM_SCOPED_AUTODISCOVERY_COMPLETED;
    static const string CM_SCOPED_AUTODISCOVERY_FAILED;
    static const string CM_SCOPED_SYNCH_AUTODISCOVERY;
    static const string CM_LEVEL_RESYNCHRONIZATION;
    static const string CM_DISCOVERY_EVENT;
    // Base type of Events.

    BaseCmdInfo();

    BaseCmdInfo(const string& moc, const string& moi,
                const list<MoAttributePo*>& attrib,
                const long& timestamp = 0,
                const string& type = "");

    BaseCmdInfo(const string& moc, const string& moi,
                const string& type = "", const long& timestamp = 0);


    BaseCmdInfo(const string& moc, const string& moi,
                const string& type,
                const long& timestamp,
                const list<MoAttributePo*>& attrib);


    virtual ~BaseCmdInfo();

    void addAttribute(MoAttributePo* atr);
    // Add attribute to the list of attributes required for the operation.

    void setAttributes(const list<MoAttributePo*>& attrib);
    // Set attribute list required for the operation.

    bool removeAttribute(MoAttributePo* const& atr);
    // Remove attribute from the internal list of attributes.

    bool removeAttribute(const string& attrName);
    // Remove attribute from the internal list of attributes by name

    bool removeAttributes(const list<MoAttributePo*>& atrlist);
    // Remove specified attributes from an internal list of attributes.

    void deleteAllAttributes();
    // Remove all attributes.

    const MoAttributePo* getAttribute(const string& name) const;
    // return pointer to the attribute inside of the attribute list or NULL

    const list<MoAttributePo*>& getAttributes() const;
    // Return reference to internal list of attributes.

    const string& getMoc() const;

    void setMoc(const string& moc);

    const string& getMoi() const;

    void setMoi(const string& moi);

    const string& getType() const;

    const string& getOriginalEventType() const;

    void setType(const string& type);

    void setOriginalEventType(const string& type);

    long getTimeStamp() const;

    void setTimeStamp( const long& timestamp);

    void setNeId(const string& neId);

    const string& getNeId() const;

    virtual void reinitialize(const BaseCmdInfo* orig);
    // Reinitialize the BaseCmdInfo with the new values. Old attributes will be deleted.

    virtual BaseCmdInfo* clone() const;
    // Create copy of the original BaseCmdInfo.

    int attributesNumber() const
    {
        return _attrib.size();
    }

    bool isNeActive() const;

    const string& getAddlInfo() const;

    void setAddlInfo(const string& addlInfo);



protected:

    string _moi;
    string _moc;
    string _type;
    string _addlInfo;
    string _originalEventType;

    list<MoAttributePo*> _attrib;
    long _timestamp;

    string _neId;
    // Node Id tied to the operation. Generic command is using it to find
    // the connection information.

private:


    // Copy constructor and = operator are not allowed
    BaseCmdInfo(const BaseCmdInfo& n);

    BaseCmdInfo& operator=(const BaseCmdInfo& n);
};

class BaseCmdInfoList
{
public:
    BaseCmdInfoList()
    {}
    const list<BaseCmdInfo*>& getBaseCmdInfoList() const
    {
        return _baseCmdInfoList;
    }
    void setBaseCmdInfoList(list<BaseCmdInfo*>& baseCmdInfoList)
    {
        _baseCmdInfoList = baseCmdInfoList;

    }
    void setType(const string& type)
    {
        _type = type;
    }
    const string& getType() const
    {
        return _type;
    }
    const string& getMoc() const
    {
        return _moc;
    }

    void setMoc(const string& moc)
    {
        _moc = moc;
    }

    const string& getMoi() const
    {
        return _moi;
    }

    void setMoi(const string& moi)
    {
        _moi = moi;
    }

private:
    list<BaseCmdInfo*> _baseCmdInfoList;
    string _type;
    string _moc;
    string _moi;
};


class NotificationInfo : public BaseCmdInfo
{
public:

    NotificationInfo() : _persistEvent(true), _notify(true), _invocationId( -1)
    {}

    NotificationInfo(const string& moc, const string& moi,
                     const string& type = "", const long& timestamp = 0) :
            BaseCmdInfo(moc, moi, type, timestamp) , _persistEvent(true), _notify(true), _invocationId( -1)
    {}

    const string& getTimeString() const
    {
        return _timeString;
    }

    void setTimeString(const string& timeString)
    {
        _timeString = timeString;
    }

    bool persistEvent()
    {
        return _persistEvent;
    }

    void setPersistEvent(bool persist = true)
    {
        _persistEvent = persist;
    }

    void doNotNotifyDb()
    {
        _notify = false;
    }

    bool notifyDb()
    {
        return _notify;
    }

    void setInvocationId(int invocationId)
    {
        _invocationId = invocationId;
    }

    int getInvocationId()
    {
        return _invocationId;
    }

protected:
    int _invocationId;

private:

    string _timeString;
    bool _persistEvent;
    bool _notify;

};



};

#endif
