/***********************************************************************************************
* COPYRIGHT $Date:   04 Feb 1999 14:38:18  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: Attribute Factory managed object source file 
* Type: C++ File
* Originator: $Author:   dtchalak  $
* File: $Workfile:   MoAttributefactory.cpp  $
* Version: $Revision:   1.1  $
* Description: 
* Attribute Factory managed object related functions
*
* Revision History:
*
* $Log:   P:/nsg_dev/btm/source/cm/server_proj/cmmain/MoAttributefactory.cpv  $
 
    Rev 1.1   04 Feb 1999 14:38:18   dtchalak
 ready for IP-5
 
    Rev 1.0   22 Jan 1999 12:22:44   dtchalak
  
// 
//    Rev 1.3   Dec 16 1998 12:47:30   gfreiboth
// changes related to new converter code from UHC
// 
//    Rev 1.2   Nov 09 1998 17:00:22   dtchalak
//  
// 
//    Rev 1.1   Nov 05 1998 15:07:24   dtchalak
//  
// 
//    Rev 1.0   Oct 16 1998 17:34:18   dtchalak
//  
*
************************************************************************************************/




#include "MoAttributefactory.h"

MoAttributeFactory * MoAttributeFactory::_instance = NULL;
