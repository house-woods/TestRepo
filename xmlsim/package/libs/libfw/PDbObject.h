
/***********************************************************************************************
* COPYRIGHT $Date$ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: PObject
* Type: H  
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description: 
* Base Persistent class.
* 
* Revision History:
*
* $Log$
*
* File Dependencies: 
* <List of include files this file requires> 
************************************************************************************************/

#ifndef _PDBOBJECT_H_
#define _PDBOBJECT_H_

#include <ostore/ostore.hh>
#include <string>

class PDbObject
{
public:
	//  Added  type specifier for objectstore.
	static os_typespec* get_os_typespec();

	virtual string getClassName() const { return "PDbObject"; }

};


#endif


