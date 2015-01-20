#ifndef _NSGERRORDEF_H_
#define _NSGERRORDEF_H_

namespace ELogger
{

typedef unsigned long ErrorCode;

// Following is a list of error codes used in EMS.
// Communication Failure range = 0x0000-0x00FF
const ErrorCode CommFailureWithAgent = 0x0000;
const ErrorCode CommFailureWithProxyAgent = 0x0001;
const ErrorCode RestartNetworkElement = 0x0002;
const ErrorCode AMThreadFunctionFailure = 0x0003;
const ErrorCode NEMigrationRequired = 0x0004;
const ErrorCode ResyncNetworkElement = 0x0005;

//TLIaa54672 Added by jhu
//Add more error codes for SGM to notify SysMon about the NE status and server group status
const ErrorCode NEActive = 0x0010;
const ErrorCode NEInActive = 0x0011; 
const ErrorCode NEAgentUp = 0x0012;					// Migration support
const ErrorCode NEPatchedVersion = 0x0013;		// patched version support in SGM
const ErrorCode NEPasswordChange = 0x0014;		// ProxyServer changed NE password
const ErrorCode ServerGroupActive = 0x0020;
const ErrorCode ServerGroupInActive = 0x0021;
const ErrorCode ServerGroupRecovering = 0x0022;


// Database Failure range = 0x0100-0x01FF
const ErrorCode DatabaseServerFailure = 0x0100;
const ErrorCode DatabaseFailure = 0x0101;
const ErrorCode DatabaseUnknownFailure = 0x0102;

const ErrorCode CmAutodiscoveryFailed = 0x0200;
const ErrorCode FmAutodiscoveryFailed = 0x0201;


// Miscellaneous Failure range = 0xFF00-0xFFFF





typedef unsigned long err_type;
const int MAX_ERRORMSG_LENGTH = 1280;

// The error type can be defined using bitwise mask. Each individual
// process using this mask can defined their own definition of trace level.
const err_type TRACE1 =		0x0001;
const err_type TRACE2 =		0x0002;
const err_type TRACE3 =		0x0004;
const err_type TRACE4 =		0x0008;
const err_type TRACE5 =		0x0010;
const err_type TRACE6 =		0x0020;
const err_type TRACE7 =		0x0040;
const err_type TRACE8 =		0x0080;
const err_type TRACE9 =		0x0100;
const err_type TRACE10 =	0x0200;
const err_type TRACE11 =	0x0400;
const err_type ETRACE12 =	0x0800;
const err_type TRACE13 =	0x1000;
const err_type MINOR =		0x2000;
const err_type MAJOR =		0x4000;
const err_type CRITICAL =	0x8000;

};

#endif // _NSGERRORDEF_H_
