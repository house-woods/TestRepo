/***********************************************************************************************
* COPYRIGHT $Date:   August 16 2003 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Info List
* Type: H
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1InfoList.h  $
* Version: $Revision:   1.5  $
* Description: 
* 	The wrapper class for list <BaseCmdInfo*>
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _TL1InfoList_H_
#define _TL1InfoList_H_

#include <list>
#include "BaseCmdInfo.h"

namespace EMS_FW
{
class BaseCmdInfo;

class TL1InfoList
{
public:

	typedef list<EMS_FW::BaseCmdInfo*>::iterator infoListIterator;

	TL1InfoList() { }
	
	~TL1InfoList();
	
	void push_back(EMS_FW::BaseCmdInfo* info) {_infoList.push_back(info);}

	void pop_back() {_infoList.pop_back();}

	void push_front (EMS_FW::BaseCmdInfo* info) {_infoList.push_front(info);}

	void pop_front () {_infoList.pop_front();}

	void clear () {_infoList.clear();}

	size_t size() {return _infoList.size();}

	bool empty() {return _infoList.empty();}

	EMS_FW::BaseCmdInfo* front() {return _infoList.front();}
	EMS_FW::BaseCmdInfo* back() {return _infoList.back();}

	infoListIterator insert ( infoListIterator pos, EMS_FW::BaseCmdInfo* info) {return _infoList.insert(pos, info);}

	infoListIterator begin() { return _infoList.begin();}
	infoListIterator end() { return _infoList.end();}
	infoListIterator erase (infoListIterator i) { return _infoList.erase(i);}
	void remove (EMS_FW::BaseCmdInfo* info) { return _infoList.remove(info); }


	
private:

	list<EMS_FW::BaseCmdInfo*> _infoList;


};


};


#endif



