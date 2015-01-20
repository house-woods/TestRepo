
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:46:32  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: BaseNameResolver
* Type: H 
* Originator: $Author:   dtchalak  $
* File: $Workfile:   BaseNameResolver.h  $
* Version: $Revision:   1.6  $
* Description: 
* Resolve Names. 
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/BaseNameResolver.h_v  $
 * 
 *    Rev 1.6   Oct 27 1999 14:46:32   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.5.1.0   Sep 20 1999 16:46:16   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.5   02 Sep 1999 15:50:22   dtchalak
 * New interface support added.
 * 
 *    Rev 1.4   21 Jul 1999 10:35:12   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.3   14 Jun 1999 16:25:00   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.2   21 May 1999 09:55:50   dtchalak
 *  
 * 
 *    Rev 1.1   04 May 1999 19:13:14   dtchalak
 * Comments added.
**********************************************************************************************/


#ifndef _BASENAMERESOLVER_H_
#define _BASENAMERESOLVER_H_

#include "BaseException.h"
#include <string>
#include <list>
#include <map>

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

namespace EMS_FW
{



// Class responsible for storing map using for resolving names base on a key.
class BaseNameResolver
{
public:

	BaseNameResolver() { MAPDELIM = "="; }

	void setMapDelim(const string& delim) { MAPDELIM = delim; }

	virtual bool initialize(const string& fileName);
	// Load resolver map from specified file.

	bool isInitialized() const { return _initialized; }
	// Return true if map is properly initialized.

   virtual string resolveName(const string& name) const;
	// Return name associated to specified key.

	bool checkMap(const list<string>& mapList, string& missingStrings) const;
	// Check that map contain all entries specified in mapList.

	void print() const;
	// Helper function print entire map content.

	void first();

	bool more();

	void next(string& key , string& value);
	// Helper iterator methods.

	virtual const string& getClassName() const;
	// Helper function returning class name 'BaseNameResolver'.

protected:
	typedef map<string, string> maptype;

	maptype _nameMap;
	// Map containing key-name pairs.

	maptype::iterator _p;
	// Iterator to the map

	bool _initialized;
	// Flag true if map is properly initialized.

	string MAPDELIM;
};

};

#endif
