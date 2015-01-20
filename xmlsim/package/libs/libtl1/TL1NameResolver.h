/***********************************************************************************************
*
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1NameResolver
* Type: H 
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description: 
*       TL1NameResolver class will resolve application specific TL1 names.
* 
* Revision History:
*
**********************************************************************************************/

#ifndef _TL1NameResolver_H_
#define _TL1NameResolver_H_


#include "CmNameResolver.h"


class TL1NameResolver : public EMS_FW::CmNameResolver
{
public:
	

	TL1NameResolver();	

	const string& getClassName() const;

protected:

	bool _initialize();	
	
};

#endif
