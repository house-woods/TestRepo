/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:22  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: BaseException 
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseException.h  $
* Version: $Revision:   1.7  $
* Description: 
* Contain Base Internal CmExceptions
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/BaseException.h_v  $
 * 
 *    Rev 1.7   Oct 27 1999 14:46:22   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.6.1.0   Sep 20 1999 16:45:54   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.6   08 Sep 1999 13:05:40   dtchalak
 * USE_ALTERNATE_STD defines added.
 * 
 *    Rev 1.5   02 Sep 1999 15:50:12   dtchalak
 * New interface support added.
 * 
 *    Rev 1.4   21 Jul 1999 10:35:00   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.3   14 Jun 1999 16:24:46   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.2   21 May 1999 09:55:38   dtchalak
 * Small modifications.
 * 
 *    Rev 1.1   04 May 1999 19:13:04   dtchalak
 * Comments added.
 * 
 *    Rev 1.0   05 Apr 1999 12:26:56   bacuff
 *  
*
* File Dependencies: 
* <List of include files this file requires> 
************************************************************************************************/

#ifndef _BASEEXCEPTION_H_
#define _BASEEXCEPTION_H_

#include "NsgErrorDef.h"

#define UNKNOWN_ERROR_CODE 0
#define MAX_DESCRIPTION_SIZE 500

#include <string>

namespace EMS_FW
{
// Base Exception Class.
class BaseException
{
public:

	enum BaseExState { RECOVERABLE, NOTRECOVERABLE };
	// Define if Exception is recoverable mean operation could be repeated
	// and there are chance that it want happen again, like 
	// lost of network connection.

	BaseException(const std::string& desc,
				 const std::string& className = "UNKNOWN",
				 const std::string& methodName = "UNKNOWN",
				 const ELogger::err_type severity = ELogger::CRITICAL,
				 const BaseExState type = NOTRECOVERABLE,
				 int errCode = UNKNOWN_ERROR_CODE,
				 int lineNumber = -1);
	// Constructor.
	
	virtual ~BaseException() {}

    const std::string& getDescription() const; 
	// Return exception description.

	BaseExState getType() const;
	// Return Type of Exception Recoverable/NotRecoverable.

	ELogger::err_type getSeverity() const;
	// Return exception Severity.

	int getErrCode() const;
	// Return Err Code for exception.

	const std::string& getExClassName() const;
	// Return class name where exception was thrown.

	const std::string& getMethodName() const;
	// Return method name where exception was thrown.

	int getLineNumber() const;
	// Return Line Number where exception was thrown.

	void setType(BaseExState type);
	// Set Type for the Exception.

	void setSeverity(ELogger::err_type severity);
	// Set exception Severity.

	void setErrCode(int errCode);
	// Set exception Error Code.

	void setExClassName(const std::string& className);
	// Set class Name where exception was thrown.

	void setMethodName(const std::string& methodName);
	// Set method name where exception was thrown.

	void setLineNumber(int lineNumber);
	// Set line number where exception was thrown.

	virtual  const std::string& getClassName() const;
	// Return Class Name for BaseException. Helper function.

protected:

	std::string _desc;
	// Exception description.
	std::string _className;
	// Class where exception was thrown.
	std::string _methodName;
	// Method where exception was thrown.
	BaseExState _type;
	// Exception type.
	ELogger::err_type _severity;
	// Exception Severity.
	int _errCode;
	// Exception Error Code.
	int _lineNumber;
	// Line Number where exception was thrown.

	BaseException();
	// Default Constructor
};


// Class derived from BaseException and holding information of unresolved name
// in BaseName resolver.
class UnresolvedNameEx :public BaseException
{
public:

	UnresolvedNameEx(const std::string& name,
					 const std::string& desc = "UNKNOWN",
					 const std::string& className = "UNKNOWN",
				     const std::string& methodName = "UNKNOWN",
					 const ELogger::err_type severity = ELogger::CRITICAL,
					 const EMS_FW::BaseException::BaseExState type = NOTRECOVERABLE,
				     int errCode = UNKNOWN_ERROR_CODE,
				     int lineNumber = -1);
	// Constructor.

	const std::string& getName() const; 
	// Return unresolved name.

private:
	std::string _name;
	// Unresolved name.

	UnresolvedNameEx(); 
	// Default constructor.
};

// MessageQueueOverflow is an exception thrown when `maxSize` is exceeded.
class CmMessageQueueOverflow {};

};

#endif

