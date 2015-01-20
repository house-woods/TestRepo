/***********************************************************************************************
* COPYRIGHT $Date:   Feb 28 2001 16:32:44  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: StringParser.h
* Type: Header file 
* Originator: $Author:   lzou  $
* File: $Workfile:   StringParser.h  $
* Version: $Revision:   1.0  $
* Description: 
*  This class is used to parse the string list. This list consists of a set of tokens
*  separated by the delimiter.
* 
* Revision History:
*
* $Log:   $
   
      Rev 1.0   Feb 28 2001 16:49:26   lzou
**********************************************************************************************/
#ifndef StringParser_h
#define StringParser_h 1
#include <list>

class StringParser
{
  
  public:

	  // Copy constructor.
      StringParser(const StringParser &right);
	
	  // Constructs an instance of server with specified string and delimitor
      StringParser (const string& a, const string& b);

	  void initialize();

	  void finialize();

	  // how many token in this string list
	  int getLength();

	  // get the delimitor
	  string getDelimitor(){return _delimitor;}

	  string getTokenbyIndex(const int index);

	  int getIndexOfToken (const string& token);

	  string getLastToken();
	  // Destructor.
      ~StringParser();


  private:
	
	// Default constructor.
    StringParser();

	string _list;
	string _delimitor;
	vector< string > _tokenL;


	
};
#endif
