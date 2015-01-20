#ifndef _NSGABSTRACTNOTIFICATION_H
#define _NSGABSTRACTNOTIFICATION_H

#include "orbsvcs/CosNotifyChannelAdminC.h"

namespace EMS_FW
{

class NsgAbstractNotification
{
public:

	enum NotificationId {btm_alarm = 0, btm_modified_alarm, ots_alarm, ots_modified_alarm};

	NsgAbstractNotification() {}
	virtual ~NsgAbstractNotification() {}

	CosNotification::StructuredEvent& getStructuredEvent() { return _se; }

protected:

	CosNotification::StructuredEvent _se; 
};

};

#endif // _NSGABSTRACTNOTIFICATION_H
