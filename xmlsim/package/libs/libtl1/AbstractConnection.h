#ifndef _ABSTRACTCONNECTION_H
#define _ABSTRACTCONNECTION_H

#include <string>

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

#include "FWComponent.h"

namespace EMS_FW
{

class AbstractConnection : public FWComponent 
{
public:

	enum ConnectionType {unknown, CmisEvent, CmisCmd};
	enum ProtocolType {undefined , Cmis, TL1, SNMP};

	AbstractConnection() : _type(unknown), _protocol(undefined), _portNumber(0) {}
	virtual ~AbstractConnection() {}

	virtual bool initialize() = 0;
	virtual bool shutdown() = 0;

	bool operator==(const AbstractConnection& c1);

	ConnectionType getType() const { return _type; }
	ProtocolType getProtocolType() const { return _protocol; }
	const char* getHostName() const { return _hostname.c_str(); }
	unsigned getPortNumber() const { return _portNumber; }

protected:

	ConnectionType _type;
	ProtocolType _protocol;
	string _neName;
	string _hostname;
	unsigned _portNumber;

};

};

inline bool
EMS_FW::AbstractConnection::operator==(const EMS_FW::AbstractConnection& c1)
{
	if (c1._type == _type && c1._neName == _neName)
		return true;

	return false;
}


#endif // _ABSTRACTCONNECTION_H
