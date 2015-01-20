/***********************************************************************************************
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1Lexer.h
* Description: 
* Implements lexer for TL1 parser
* 
**********************************************************************************************/

#ifndef __TL1LEXER_H__
#define __TL1LEXER_H__
#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
#include <string>
#include <vector>
#include <ostream>

namespace TL1_MA
{

//const size_t NPOS  = (size_t)(-1);

namespace LexerTL1
{

const size_t NPOS = (size_t)( -1);


enum TL1TokenType {
    FormatEffector,  // One of sp, cr, lf, hr, vt, ff
    InnerString,
    TextString,
    Comment,
    CommentNoEnd,    // TITAN5500 Bug
    General,         // ident,symname, ...
    Character,       // One of ':', ';', '>', '<'
    EndToken
};

struct Token
{
    TL1TokenType m_type;
    string m_match;
    string m_value;
};

// Abstract Base Class for getting characters
// This is implemented by the Lexer User

class LexInputSource
{
public:
    // returns zero length string on end of stream
    virtual bool getCharacter(char& ch) = 0;
    virtual ~LexInputSource(void)
    {}
}
;

// Abstract Base Class for the Lexer
// This is implemented by the Lexer

class Lexer
{
public:
    virtual Token getToken(void) = 0;
    virtual void setCommentError(void) = 0;
    virtual void putToken(const Token& token) = 0;
    virtual void setMatch(string& str) = 0;
    virtual ~Lexer(void)
    {}
}
;

Lexer* createLexer(LexInputSource*);

bool tokenIsCharacter(Token& token, char c);
bool tokenIsGeneral(Token& token, const char* str);
bool tokenIsFormatEffector(Token& token, const char* str);
bool tokenIsWS(Token& token);

// }

ostream& operator<< (ostream& os, LexerTL1::Token& token);

///////////////////////////////////////////////////////////////////////////////////////////////
// Test Input Source: from an IO input stream
///////////////////////////////////////////////////////////////////////////////////////////////
class IStreamLexInput : public LexerTL1::LexInputSource
{
public:
    // Constructor
    IStreamLexInput(istream& is);

    // Destructor
    ~IStreamLexInput();

    // Return a character
    bool getCharacter(char& ch);

private:
    // Disable copy constructor
    IStreamLexInput(const IStreamLexInput &lx);
    // Disable assignment operator
    IStreamLexInput& operator=(const IStreamLexInput &lx);

    istream& m_is;     // IO stream
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Input from a String
///////////////////////////////////////////////////////////////////////////////////////////////
class StringLexInput : public LexerTL1::LexInputSource
{
public:
    // Constructor
    StringLexInput(const string& str);

    // Destructor
    ~StringLexInput();

    // Return a character
    bool getCharacter(char& ch);
    void getCurrentStr (string& retStr);


private:

    // Disable Copy Constructor
    StringLexInput(const StringLexInput &lx);
    // Disable assignment operator
    StringLexInput& operator=(const StringLexInput &lx);

    string m_str;    // String
    bool m_eos;    // End of string indicator
    const char* m_pointer;   // Current string pointer
};


///////////////////////////////////////////////////////////////////////////////////////////////
// Input from a character source
///////////////////////////////////////////////////////////////////////////////////////////////
class CharacterSource : public LexerTL1::LexInputSource
{
public:

    // Constructor
    CharacterSource(string& threadData);

    // Destructor
    ~CharacterSource();

    // Return a character
    bool getCharacter(char& ch);

private:

    // Disable copy constructor
    CharacterSource(const CharacterSource &lx);
    // Disable assignment operator
    CharacterSource& operator=(const CharacterSource &lx);

    string m_threadData;   // Data to be parsed as a string
    string* m_currentString; // Data to be parsed as a pointer
    const char* m_currentPtr;  // Current position in string

};


} // LexerTL1 namespace


} // TL1_MA namespace


#endif
