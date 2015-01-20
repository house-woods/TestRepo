


/***********************************************************************************************
* COPYRIGHT $Date$ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: UserException
* Type: H 
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description: 
* Exception class containing user specified attributes. 
* 
* Revision History:
*
* $Log$
**********************************************************************************************/


#ifndef _USEREXCEPTION_H_
#define _USEREXCEPTION_H_

#include "BaseException.h"
#include "tutil.h"
#include "MoAttributePo.h"

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

namespace EMS_FW
{

	class UserException :public BaseException
	{
		public:

			UserException(const string& desc = "UNKNOWN",
				int errCode = UNKNOWN_ERROR_CODE) : BaseException(desc)
			{
				setErrCode(errCode);
			}

			UserException(const UserException& ex)
			{
				_fill(ex);
			}

			UserException& operator=(const UserException& ex)
			{
				_fill(ex);
				return *this; 
			}

			~UserException() 
			{
				Tutil::deleteAttributeList(_attrList);
			}

			void setAttributeList(const list<MoAttributePo*> lst)
			{
				_attrList = lst;
			}

			const list<MoAttributePo*>& getAttributeList() { return _attrList; }

			void addAttribute(MoAttributePo* attr)
			{
				_attrList.push_back(attr);
			}

		private:
			list<MoAttributePo*> _attrList;
			void _fill(const UserException& ex)
			{
				_desc = ex.getDescription();
				_className = ex.getClassName();
				_methodName = ex.getMethodName();
				_type = ex.getType();
				_severity = ex.getSeverity();
				_errCode = ex.getErrCode();
				_lineNumber = ex.getLineNumber();
				Tutil::deleteAttributeList(_attrList); // delete previos attributes if any
				list<MoAttributePo*>::const_iterator p;
				for(p = ex._attrList.begin(); p != ex._attrList.end(); p++)
				{ // clone attributes
					MoAttributePo* newAttr = new MoAttributePo((*p)->getName(),
															   (*p)->getValue()->clone());
					addAttribute(newAttr);
				}
			}


	};
};

#endif



					  
	// Constructor.
