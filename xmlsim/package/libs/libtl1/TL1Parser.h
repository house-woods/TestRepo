/***********************************************************************************************
 * COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
 * ALL RIGHTS RESERVED 
 * NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
 * THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
 * 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
 ************************************************************************************************
 * 
 * Name: TL1Parser.h
 * Description: 
 *	Implements TL1 Command parser 
 *
 **********************************************************************************************/

#ifndef __TL1PARSER_H__
#define __TL1PARSER_H__

#include <list>
#include <vector>
#include "TL1Message.h"

namespace TL1_MA {

namespace LexerTL1 {
	class Lexer;
}

namespace ParseTL1
{
//////////////////////////////////////////////////////////////////////////////////////////////
// Provide Name Value pairs for parsed results
//////////////////////////////////////////////////////////////////////////////////////////////
class NameValue 
{
public:

	// Constructors
	NameValue(void);
	NameValue(const string& name, const string& value, const int position, const int block);
	NameValue(const string& name, const string& value);

	// Copy Constructor
	NameValue(const NameValue &nv);

	// Destructor
	virtual ~NameValue();

	// Get name 
	string& name(void) const;

	// Set name
	void name(const string& name);

	// Get value
	string& value(void) const;

	// Set value
	void value(const string& value);

	// Get position
	int position(void) const;

	// Set position
	void position(const int position);

	// Get block
	int block(void) const;

	// Set block
	void block(const int block);

	// return name value pair as string
	string asString(void);

protected:
	string m_name;						// Name
	string m_value;						// Value
	int m_position;						// Position
	int m_block;						// Block
};

///////////////////////////////////////////////////////////////////////////////////////////////
//  A class containing a vector of namevalues
///////////////////////////////////////////////////////////////////////////////////////////////
class NameValues : public vector<NameValue>
{
public:
	// Constructor
	NameValues();

	//Destructor 
	~NameValues();

	// Copy constructor
	NameValues(const NameValues &nvs);

	// Insert functions
	void insert(const string& namex, const string& valuex);
	void insert(const string& namex, const string& valuex, const int position, const int block);

	// Finder functions
	const string& findValue(const string& namex) const;

	//Find value based on position
	const string& findValue(const int position) const;

	// Replace a value
	bool replaceValue(const string& name, const string& value);

	// Remove a name/value pair
	string remove(const string& target);

	// Determine if a name is  present
	bool present(const string& namex);

	//Return name value pairs as a comma seperated string
	string asString(void);

private:

	string m_blankString;						// Empty string

};

//////////////////////////////////////////////////////////////////////////////////////////////
// Parse Error Structure
//////////////////////////////////////////////////////////////////////////////////////////////
struct ParseError
{
	string m_reason;
	string m_code;
	ParseError(void)
	{}
	ParseError(const string& str, const string& code)
            : m_reason(str), m_code(code)
	{}
};

// Parse error exception
void throw_parseError(const string& reason, const string &code);

// String to integer
int stoi(const string& strm, int pos = 0);

// Integer to string
string itos(int i, int leadingZero = 0);

//////////////////////////////////////////////////////////////////////////////////////////////
// Generic Error Structure
//////////////////////////////////////////////////////////////////////////////////////////////
struct Error
{
	string m_what;
	Error(const string& reason)
		: m_what(reason)
	{
	}
	
	const char* what(void)
	{
		return m_what.c_str();
	}
};

// Return a lower case string
inline string lower(const string& str)
{
	string ret;
	ret = str;
	for (unsigned  int i = 0; i < ret.size(); ++i)
	{
		ret[i] = tolower(ret[i]);
	}
	return ret;
}

// Return an upper case string
inline string upper(const string& str)
{
	string ret;
	ret = str;
	for (unsigned int i = 0; i < ret.size(); ++i)
	{
		ret[i] = toupper(ret[i]);
	}
	return ret;
}

// encode a string
string encodeStringValue(const string& value);

// determine if an encoded string is necessary
bool needsStringEncode(const string& value);

struct TL1PayloadAttribute
{
	int  m_block;
	int  m_position;  // 0:-No position
	string m_name;
	string m_value;
	TL1PayloadAttribute() {};
	TL1PayloadAttribute(int block, int position,
		const string& name, const string& value)
		: m_block(block),
		m_position(position),
		m_name(name),
		m_value(value)
	{}
};

typedef list<TL1PayloadAttribute> PayloadList;

// Structure that contains parsed results
struct TL1CommandParseBuffer
{
	string		m_command;
	string		m_tid;
	NameValues	m_aid;
	string		m_ctag;
	string		m_gb;
	NameValues	m_params;
	PayloadList	m_attributes;
};

// Abstract parser class
class Parser
{
public:
	virtual TL1Message* getTL1Message(void) = 0;
        virtual TL1Message* getTL1Autonomous(void) = 0;	
	virtual ~Parser() {};
};

// Create a parser instance
Parser* createParser(LexerTL1::Lexer*);

}

}

#endif
