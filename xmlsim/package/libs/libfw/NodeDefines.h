#ifndef _NODEDEFINES_H_
#define _NODEDEFINES_H_

#include <string>
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

class NODEDEF
{

public:
	// ATTENTION Temporary here cleint Types definitions

static const string	 Root;				//root  type
static const string	 Region;			//region  type
static const string	 Site;				//site  type
static const string	 Node;				//NE type
static const string	 Name;
static const string  CLLI;
static const string	 NodeIPAddress;
static const string	 NodePortNumber;
static const string	 NodeKeepAlive;
static const string	 Location;
static const string	 Description;
static const string	 NodeId;
static const string	 NumNesLicensed;	// Number of NEs this EMS licensed to manage
static const string  NeLicenseKey;		// The License key
static const string  NumNesInDB;		// The number of NEs in the DB at the current time.


static const string FromNode;
static const string ToNode;
static const string TheNode;


// The following are specific to the Dual GNE
// This is what these values mean
// For a DIR NE:or GNEA
// NodeIPAddress = ip of the NE, NodePortNumber=TL1 port (as before)
//
// For GNEB
// NodeIPAddress1 = ip of the NE, NodePortNumber1=TL1 port (as before)
// NodeTID1 = tid of the the selected GNEA
//
// For RNE
// NodeIPAddress and NodePortNumber are NOT used
// NodeTID1 = tid of the primary GNE
// NodeTID2 = tid of seconday GNE

// can be one of DIR, GNEA, GNEB, RNE
// Dual GNE
static const string  NODEDEF::EonType;
static const string  NODEDEF::EonTypeDIR;
static const string  NODEDEF::EonTypeGNEA;
static const string  NODEDEF::EonTypeGNEB;
static const string  NODEDEF::EonTypeRNE;
static const string  NODEDEF::NetworkType;  //Private or Public
static const string  NODEDEF::NwTypePrivate;
static const string  NODEDEF::NwTypePublic;

// Dual GNE Eon Types
static const string  NODEDEF::HWEonType;
static const string  NODEDEF::HWEonTypeDIR_OSC;
static const string  NODEDEF::HWEonTypeDIR_NOOSC;
static const string  NODEDEF::HWEonTypeGFNE;
static const string  NODEDEF::HWEonTypeRFNE;
static const string  NODEDEF::HWEonTypeGNE_1;
static const string  NODEDEF::HWEonTypeGNE_2;
static const string  NODEDEF::HWEonTypeRNE;

// GNEB
static const string  NODEDEF::NodeIPAddress1;
static const string  NODEDEF::NodePortNumber1;

// RNE
static const string  NODEDEF::NodeTid1;
static const string  NODEDEF::NodeTid2;




};


#endif
