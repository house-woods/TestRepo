/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 BaseCmdInfo List
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1InfoList.h  $
* Version: $Revision:   1.5  $
* Description: 
* 
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#include "TL1InfoList.h"

using namespace EMS_FW;

TL1InfoList::~TL1InfoList()
{
    infoListIterator p;
    for (p = _infoList.begin();p != _infoList.end();p++)
    {
        if (*p)
            delete (*p);
    }
    _infoList.clear();
}

