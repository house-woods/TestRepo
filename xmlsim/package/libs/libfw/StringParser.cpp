/*******************************************************************************
* COPYRIGHT Feb 28 2001 - 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: StringParser.cpp
* Type: C++ 
* Originator: lzou
* File: StringParser.cpp
* Version: 1.0 
* Description: 
*  This class is used to parse the string list. This list consists of a set of tokens
*  separated by the delimiter.
* 
* Revision History:
*
*     Rev 1.0   Feb 28 2001 16:49:26   lzou
******************************************************************************/

#include "sysErrLog.h"
#include "BaseException.h"
#include "StringParser.h"

using namespace ELogger;

extern const char* sourceName;
static const char* objectName = "StringParser";

/*****************************************************************
 * Default constructor 
 * 
 *****************************************************************/
StringParser::StringParser()
{}

/*****************************************************************
 * Constructor 
 * 
 *****************************************************************/
StringParser::StringParser(const string& a, const string& b)
{
    _list = a;
    _delimitor = b;
    initialize();
}

/*****************************************************************
 * Copy constructor.
 *****************************************************************/
StringParser::StringParser(const StringParser &right)
{}

//******************************************************************************
void StringParser::initialize()
{
    char *token = NULL;
    _tokenL.clear();
    int len = _list.length();

    ErrorLogger::instance()->logVarError(sourceName, objectName, "initizlize", TRACE4,
                                         "Process Token string %s", _list.c_str());
    char* v = new char[len + 1];
    strncpy(v, _list.c_str(), len + 1);
    char* ptrLastArg = NULL;
    token = strtok_r(v, _delimitor.c_str(), &ptrLastArg);
    while (token != NULL)
    {
        _tokenL.push_back(string(token));
        token = strtok_r(NULL, _delimitor.c_str(), &ptrLastArg);
    }
    ErrorLogger::instance()->logVarError(sourceName, objectName, "initizlize", TRACE4,
                                         "Found %d Tokens", _tokenL.size());

    delete [] v;
}

//******************************************************************************
void StringParser::finialize()
{
    _tokenL.clear();
}


//******************************************************************************
// how many token in this string list
int StringParser::getLength()
{
    return _tokenL.size();
}

//******************************************************************************
string StringParser::getTokenbyIndex(const int index)
{
    if (index >= _tokenL.size())
    {
        char msg[255];
        msg[0] = '\0';
        if (_list.length() < 200)
            sprintf(msg, "Index (%d) Out of Bounds, %s has only %d tokens.",
                    index, _list.c_str(), _tokenL.size());
        else
            sprintf(msg, "Index (%d) Out of Bounds, parsed string has only %d tokens.",
                    index, _tokenL.size());

        throw EMS_FW::BaseException(msg, "StringParser", "getTokenbyIndex", ELogger::CRITICAL, EMS_FW::BaseException::NOTRECOVERABLE);
    }

    return _tokenL[index];
}

//******************************************************************************
string StringParser::getLastToken()
{
    return _tokenL.back();
}

//******************************************************************************
// aaali added - returns index of the first occurence of the string 'token'
// within the token list
// returns -1 if none found
//
int StringParser::getIndexOfToken (const string& token)
{
    int position = -1;

    for (int index = 0; index < getLength(); index++)
    {
        if (token == _tokenL[index])
        {
            position = index;
            break; // we should stop at first occurence
        }
    }
    return position;
}

/*****************************************************************
 * Destructor.
 *****************************************************************/
StringParser::~StringParser()
{
    finialize();
}
