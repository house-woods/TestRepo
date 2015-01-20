/***********************************************************************************************
 * COPYRIGHT 2001 BY TELLABS OPERATIONS, INC 
 * ALL RIGHTS RESERVED 
 * NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
 * THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
 * 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
 ************************************************************************************************
 * 
 * Name: Tl1Response.h 
 * Description: 
 *	Implements TL1 Response 
 * 
 **********************************************************************************************/

#ifndef __TL1RESPONSE_H__
#define __TL1RESPONSE_H__

#include <string>
#include <vector>

namespace TL1_MA {

static const string TL1_DENY	= "DENY";
static const string TL1_COMPLD	= "COMPLD";

// Implements TL1 Response
class Tl1Response
{
public:
	// Constructor
	Tl1Response();

	// Destructor
	~Tl1Response();

	// Set the deny response
	void setDeny(const string &ctag, const string &reason);

	// Set the completion response
	void setComplete(const string &ctag, const string &reason);

	// Set the EMS id
	void setTid(const string &tid);

	// Add a message to the response
	void addMsg(const string &msg);

	// view the response as a string
	string asString();

	// generate the header of a TL1 response
	string header();

private:
	vector<string> _msgBody;		// Vector containing messages for the response
	string _ctag;					// Ctag of the TL1 command
	string _reason;					// Reason code (error case)
	string _compCode;				// Command completion code
	string _tid;					// EMS id

	// Disable copy constructor and assignment operator.
	Tl1Response(const Tl1Response& cmd);
	Tl1Response& operator=(const Tl1Response& cmd);
};

}

#endif
