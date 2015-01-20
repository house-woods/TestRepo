
/***********************************************************************************************
* COPYRIGHT $Date:   Oct 27 1999 14:47:08  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: FileReader 
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   FileReader.h  $
* Version: $Revision:   1.6  $
* Description: 
* Read from file and parse line by line name=value sequence
* 
* Revision History:
*
* $Log:   P:/nsg_dev/Framework/common/util/inc/FileReader.h_v  $
 * 
 *    Rev 1.6   Oct 27 1999 14:47:08   dtchalak
 * Query Interface updates.
 * 
 *    Rev 1.5.1.0   Sep 20 1999 16:48:14   dtchalak
 * Just to create a branch.
 * 
 *    Rev 1.5   02 Sep 1999 15:51:18   dtchalak
 * New interface support added.
 * 
 *    Rev 1.4   21 Jul 1999 10:35:36   dtchalak
 * Multiple Ne support included.
 * 
 *    Rev 1.3   14 Jun 1999 16:25:24   dtchalak
 * Updates related to new CM.idl and other details.
 * 
 *    Rev 1.2   21 May 1999 09:56:00   dtchalak
 *  
 * 
 *    Rev 1.0   05 Apr 1999 13:50:32   bacuff
 *  
 * 
 *    Rev 1.1   04 Feb 1999 15:16:28   dtchalak
 * ready for IP-5
 * 
 *    Rev 1.0   22 Jan 1999 13:48:46   dtchalak
 *  
**********************************************************************************************/


#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <iostream>
#include <fstream>
#include <string>
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

namespace EMS_FW
{

class FileReader
{

public:

		FileReader(); 
		FileReader(const char* name); 
		
		~FileReader(); 
		bool getLine(string& line);
		bool bad(); 
		bool eof();
		void begin();
		
private:
		ifstream f;

};

};

#endif






