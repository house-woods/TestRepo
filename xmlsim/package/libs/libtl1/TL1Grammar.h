/***********************************************************************************************
 * COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
 * ALL RIGHTS RESERVED 
 * NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
 * THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
 * 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
 ************************************************************************************************
 * 
 * Name: TL1Grammar.h 
 * Description: 
 *	Implements processing of TL1 Command/Response grammar 
 * 
 **********************************************************************************************/

#ifndef __TL1CMDGRAMMAR_H__
#define __TL1CMDGRAMMAR_H__
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

#include <string>
#include <vector>
#include <map>

namespace TL1_MA {

enum TL1_PAYLOAD_CATEGORIES {
	CAT_OPTIONAL = 0,
	CAT_MANDATORY = 1,
	CAT_DEPENDENTS = 2,
	CAT_DEPENDENCY = 3
};

struct CommandAttribute
{
    int    m_block;
    int    m_position;
    int    m_catagory;
    string m_name;
};

struct ResponseAttribute
{
    int    m_block;
    int    m_position;
    string m_name;
};

namespace CommandDB
{
    typedef vector<CommandAttribute> CommandAttributes;

    struct Command
    {
        string             m_name;
        bool               m_special;
        CommandAttributes  m_attributes;
	int		   m_handleFields;
        Command() : m_special(false), m_handleFields(0) {}
    };

    typedef vector<ResponseAttribute> ResponseAttributes;
    
    struct Response
    {
        string              m_name;
        ResponseAttributes  m_attributes;
    };
    
    //  --- Calls

    void      initCommandDB(const string& fileName);
    void      initResponseDB(const string& fileName);
    
    Command* searchCommand(const string& name, const string& TL1version);
    Response* searchResponse(const string& name, const string& TL1version);
}

}

#endif
