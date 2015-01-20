/**********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:35:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
***********************************************************************************************
* 
* Name: Base Command
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseCmd.cpp  $
* Version: $Revision:   1.5  $
* Description: 
* Base Command.
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/cmd/src/BaseCmd.cpv  $
 
    Rev 1.5   Oct 27 1999 14:35:34   dtchalak
 Query Interface updates.
 
    Rev 1.4.1.0   Sep 20 1999 16:17:38   dtchalak
 Just to create branch.
 
    Rev 1.4   02 Sep 1999 15:43:18   dtchalak
 New interface support added.
 
    Rev 1.3   21 Jul 1999 10:28:16   dtchalak
 Multiple Ne support included.
 
    Rev 1.2   14 Jun 1999 16:14:50   dtchalak
 Updates related to new CM.idl and other details.
 
    Rev 1.1   04 May 1999 19:02:40   dtchalak
 Comments added.
 
    Rev 1.0   05 Apr 1999 12:34:40   bacuff
  
 
    Rev 1.1   04 Feb 1999 14:41:08   dtchalak
 ready for IP-5
 
    Rev 1.0   22 Jan 1999 12:24:54   dtchalak
  
// 
//    Rev 1.1   Nov 09 1998 16:58:30   dtchalak
//  
// 
//    Rev 1.0   Oct 16 1998 17:30:30   dtchalak
//  
**********************************************************************************************/



#include "BaseCmd.h"
#include "BaseCmdInfo.h"

using namespace EMS_FW;

#define UNDEFINED_ERRCODE 0

CmdResult::CmdResult() : _errCode(UNDEFINED_ERRCODE), _multiPart(false)
{}

CmdResult::CmdResult(const bool& stat, const string& result, const long& timestamp)
        : _errCode(UNDEFINED_ERRCODE) , stat_(stat), timestamp_(timestamp), result_(result), _multiPart(false)
{}


CmdResult::~CmdResult()
{}


bool CmdResult::getStatus() const
{
    return stat_;
}

bool CmdResult::getMultiPartFlag() const
{
    return _multiPart;
}

long CmdResult::getTimeStamp() const
{
    return timestamp_;
}

const string& CmdResult::getResultInfo() const
{
    return result_;
}

void CmdResult::setStatus(bool stat)
{
    stat_ = stat;
}

void CmdResult::setMultiPartFlag(bool multiPart)
{
    _multiPart = multiPart;
}

void CmdResult::setTimeStamp(long timestamp)
{
    timestamp_ = timestamp;
}


void CmdResult::setResultInfo(const string& result)
{
    result_ = result;
}

void CmdResult::setStrErrCode(const string& strErrCode)
{
    if (!_multiPart)
        _strErrCode = strErrCode;
    else
        _strErrCode = _strErrCode + " " + strErrCode;
}

//------------------------------------------------------------------------------
BaseCmd::BaseCmd() : _cancelRequest(false), _canceled(false) , info_(NULL)
{}

BaseCmd::~BaseCmd()
{}

BaseCmd::BaseCmd(EMS_FW::BaseCmdInfo* info) : _cancelRequest(false), _canceled(false) , info_(info)
{}

BaseCmdInfo* BaseCmd::getInfo()
{
    return info_.get();
}

void BaseCmd::cancelRequest()
{
    _cancelRequest = true;
}

bool BaseCmd::isCanceled()
{
    return _canceled;
}



