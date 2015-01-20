/***********************************************************************************************
*
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1ErrCodeResolver
* Type: H 
* Originator: $Author lzou$
* File: $Workfile$
* Version: $Revision$
* Description: 
*       TL1ErrCodeResolver class will resolve EMS error code by the TL1 code defined in TL1ErrCode.h.
* 
* Revision History:
*
**********************************************************************************************/

#ifndef _TL1ErrCodeResolver_H_
#define _TL1ErrCodeResolver_H_
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
#include <map>
#include <string>

class TL1ErrCodeResolver 
{
public:
	
	TL1ErrCodeResolver();	
	static TL1ErrCodeResolver* instance();

	const string& getClassName() const;
    	int resolveTL1ErrCode ( const string& errName);

protected:

	bool _initialize();	

private:
	static TL1ErrCodeResolver* _instance;
    	map <string, int> _errCodeMap;
    	bool _initTL1ErrorCode();
	
};

#endif
