/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:18  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: BaseException 
* Type: C++
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseException.cpp  $
* Version: $Revision:   1.6  $
* Description: 
* Contain Base Internal CmExceptions
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/src/BaseException.cpv  $
 
    Rev 1.6   Oct 27 1999 14:46:18   dtchalak
 Query Interface updates.
 
    Rev 1.5.1.0   Sep 20 1999 16:45:42   dtchalak
 Just to create a branch.
 
    Rev 1.5   02 Sep 1999 15:53:02   dtchalak
 New interface support added.
 
    Rev 1.4   21 Jul 1999 10:37:40   dtchalak
 Multiple Ne support included.
 
    Rev 1.3   14 Jun 1999 16:27:56   dtchalak
 Updates related to new CM.idl and other details.
 
    Rev 1.2   21 May 1999 09:56:22   dtchalak
  
 
    Rev 1.1   04 May 1999 19:15:28   dtchalak
 Comments added.
 
    Rev 1.0   05 Apr 1999 12:26:46   bacuff
  
*
* File Dependencies: 
* <List of include files this file requires> 
************************************************************************************************/

#include "BaseException.h"

using namespace EMS_FW;

const std::string baseExceptionName = "BaseException";

BaseException::BaseException()
{
    // never suppose to be called
}

BaseException::BaseException(const std::string& desc,
                             const std::string& className,
                             const std::string& methodName,
                             const ELogger::err_type severity,
                             const BaseExState type,
                             int errCode,
                             int lineNumber) :
                             
_type(type),
_severity(severity),
_desc(desc),
_errCode(errCode),
_className(className),
_methodName(methodName),
_lineNumber(lineNumber)
{
}


int BaseException::getLineNumber() const
{
    return _lineNumber;
}

const std::string& BaseException::getClassName() const
{
    return baseExceptionName;
}

const std::string& BaseException::getDescription() const
{
    return _desc;
}

ELogger::err_type BaseException::getSeverity() const
{
    return _severity;
}

BaseException::BaseExState BaseException::getType() const
{
    return _type;
}

int BaseException::getErrCode() const
{
    return _errCode;
}

const std::string& BaseException::getExClassName() const
{
    return _className;
}

const std::string& BaseException::getMethodName() const
{
    return _methodName;
}

void BaseException::setType(BaseExState type)
{
    _type = type;
}

void BaseException::setSeverity(ELogger::err_type severity)
{
    _severity = severity;
}

void BaseException::setErrCode(int errCode)
{
    _errCode = errCode;
}

void BaseException::setExClassName(const std::string& className)
{
    _className = className;
}

void BaseException::setMethodName(const std::string& methodName)
{
    _methodName = methodName;
}

void BaseException::setLineNumber(int lineNumber)
{
    _lineNumber = lineNumber;
}

UnresolvedNameEx::UnresolvedNameEx()
{}

UnresolvedNameEx::UnresolvedNameEx(const std::string& name,
                                   const std::string& desc,
                                   const std::string& className,
                                   const std::string& methodName,
                                   const ELogger::err_type severity,
                                   const BaseException::BaseExState type,
                                   int errCode,
                                   int lineNumber) :
        _name(name),BaseException(desc, className, methodName, severity, type, errCode, lineNumber)
{}

const std::string& UnresolvedNameEx::getName() const
{
    return _name;
}

