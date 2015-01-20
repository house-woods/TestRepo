/***********************************************************************************************
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1Lexer.cpp
* Description: 
* Implements lexer for TL1 parser
*    
**********************************************************************************************/

#include "TL1Lexer.h"
#include "TL1Parser.h"
#include "assert.h"

namespace TL1_MA
{

namespace LexerTL1
{

ostream& operator<< (ostream& os, LexerTL1::Token& token)
{
    os << "[";
    switch (token.m_type)
    {
    case LexerTL1::FormatEffector:
        os << "FormatEffector " << (int) token.m_value[0];
        break;
    case LexerTL1::InnerString:
        os << "InnerString " << token.m_value;
        break;
    case LexerTL1::TextString:
        os << "TextString " << token.m_value;
        break;
    case LexerTL1::General:
        os << "General " << token.m_value;
        break;
    case LexerTL1::Character:
        os << "Character " << token.m_value;
        break;
    case LexerTL1::EndToken:
        os << "EndToken";
        break;
    case LexerTL1::Comment:
        os << "Comment " << token.m_value;
        break;
    case LexerTL1::CommentNoEnd:
        os << "CommentNoEnd " << token.m_value;
        break;
    default:
        os << "UnknownToken";
    }
    os << "]";
    return os;
}

//namespace LexerTL1 {

// Determine if token is a character
bool tokenIsCharacter(Token& token, char c)
{
    if (token.m_type != Character)
        return false;
    return (token.m_value[0] == c);
}

// Determine if token is general
bool tokenIsGeneral(Token& token, const char* str)
{
    if (str == NULL)
        return false;
    if (token.m_type != General)
        return false;
    return (token.m_value == str);
}

// Determine if token is a format effector
bool tokenIsFormatEffector(Token& token, const char* str)
{
    if (str == NULL)
        return false;
    if (token.m_type != FormatEffector)
        return false;
    return (token.m_value == str);
}

// Determine if token is white space
bool tokenIsWS(Token& token)
{
    if (token.m_type != FormatEffector)
        return false;

    if (token.m_value.find('\r') != NPOS)
        return false;

    if (token.m_value.find('\n') != NPOS)
        return false;
    return true;
}

// Character types supported by lexer
enum TL1CharacterType
{
    End,     // End character
    Ign,     // Ignore
    Fef,     // Format Effector
    Alf,     // Alfa
    Num,     // Numeric
    Sep,     // ;,:,<,>
    Cha,     // Character
    Oth   // Other
};

// Character type table
static TL1CharacterType types[128] =
    { //      0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
        Ign, Ign, Ign, Ign, Ign, Ign, Ign, Ign, Ign, Fef, Fef, Fef, Fef, Fef, Ign, Ign,    // 0
        End, Ign, Ign, Ign, Ign, Ign, Ign, Ign, Fef, Ign, Ign, Ign, Ign, Ign, Ign, Ign,    // 1
        Fef, Cha, Cha, Cha, Cha, Cha, Cha, Cha, Cha, Cha, Cha, Cha, Sep, Cha, Cha, Cha,    // 2
        Num, Num, Num, Num, Num, Num, Num, Num, Num, Num, Sep, Sep, Sep, Cha, Sep, Cha,    // 3
        Cha, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf,    // 4
        Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Cha, Cha, Cha, Cha, Cha,    // 5
        Cha, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf,    // 6
        Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Alf, Cha, Cha, Cha, Cha, Cha  // 7
    };

// End of string
const int Eos = 0x10;

// Return the character type
TL1CharacterType characterType(int ch)
{
    if (ch == Eos)
        return End;
    if (ch > 127)
        return Ign;
    return types[ch];
}


// Lexer implementation for charachtes
class LexCharacters
{
public:
    //Constructor
    LexCharacters(LexInputSource* source)
            : m_source(source),
            m_pushed(0),
            m_matchString(0)
    {
        // Intentionally Empty
    }

    // Set matching string
    void setMatch(string& str)
    {
        m_matchString = &str;
    }

    // Return a character
    int getACharacter(void)
    {
        if (m_pushed)
        {
            m_pushed--;
            if (m_matchString)
                (*m_matchString) += (char) m_pushedCharacters[m_pushed];
            return m_pushedCharacters[m_pushed];
        }

        char x;
        if (! m_source->getCharacter(x))
        {
            if (m_matchString)
                (*m_matchString) += (char) Eos;
            return Eos;
        }

        unsigned char u_x = (unsigned char) x;

        if (u_x == Eos)
        {
            u_x = 0;  // Replace with '0'
        }
        if (m_matchString)
            (*m_matchString) += (char) u_x;

        return (int) u_x;
    }

    // Put a character
    void putACharacter(int ch)
    {
        m_pushedCharacters[m_pushed++] = ch;
    }

    // Return a TL1 character type
    int getTL1Character(void)
    {
        while (1)
        {
            int ch = getACharacter();
            if (characterType(ch) == Ign)
            {
                continue;
            }
            return ch;
        }
    }

private:
    LexInputSource* m_source;
    string* m_matchString;
    int m_pushed;
    int m_pushedCharacters[10];
};

// TL1 Command Lexer Implementation
class LexImplementation : public Lexer
{
public:
    // Constructor
    LexImplementation(LexInputSource* source)
            : m_chars(source),
            m_commentError(false),
            m_pushedToken(false)
    {
        // Intentionally Empty
    }

    // Destructor
    ~LexImplementation()
    {
        // Intentionally Empty
    }

    // Set comment error
    void setCommentError(void)
    {
        m_commentError = true;
    }

    // Set string match
    void setMatch(string& str)
    {
        m_chars.setMatch(str);
    }

    // Return an inner string
    string getInnerString(string& match)
    {
        string ret;
        while (1)
        {
            int ch = m_chars.getTL1Character();
            if (ch == Eos)
            {
                m_chars.putACharacter(ch);
                return ret;
            }
            match += (char) ch;
            if (ch == '\\')
            {
                ch = m_chars.getTL1Character();
                if (ch == Eos)
                {
                    m_chars.putACharacter(ch);
                    return ret;
                }
                match += (char) ch;
                if (ch == '\\')
                {
                    ret += (char) ch;
                    continue;
                }
                if (ch != '"')
                {
                    ret += ' ';
                    m_chars.putACharacter(ch);
                    continue;
                }
                return ret; // Lexed an InnerString
            }

            if (ch == '"')
            {
                ch = m_chars.getTL1Character();
                if (ch == Eos)
                {
                    m_chars.putACharacter(ch);
                    return ret;
                }
                match += (char) ch;
                if (ch == '"')
                {
                    ret += '"';
                    continue;
                }
                continue;
            }
            ret += (char) ch;
        }
    }

    // Return an Inner Inner string
    string getInnerInnerString(string& match)
    {
        string ret;
        while (1)
        {
            int ch = m_chars.getTL1Character();
            if (ch == Eos)
            {
                m_chars.putACharacter(ch);
                return ret;
            }
            match += (char) ch;
            ret += (char) ch;
            if (ch == '\\')
            {
                ch = m_chars.getTL1Character();
                if (ch == Eos)
                {
                    m_chars.putACharacter(ch);
                    return ret;
                }
                match += (char) ch;
                ret += (char) ch;
                if (ch == '\\')
                {
                    continue;
                }
                if (ch != '"')
                {
                    m_chars.putACharacter(ch);
                    continue;
                }
                return ret; // Lexed an InnerInnerString
            }
        }
    }


    // Get a comment
    string getComment(string& match, bool& titan5500CommentNotEnded)
    {
        int savedChar;
        int nestedCommentCounter = 1; //Because first comment starting '/*' is already found, that's why it came into this function
        enum {InComment, ExpectFSlash, ExpectTitanBug, ExpectStar}
        state = InComment;
        titan5500CommentNotEnded = false;
        string ret;
        while (1)
        {
            int ch = m_chars.getTL1Character();
            if (ch == Eos)
            {
                m_chars.putACharacter(ch);
                return ret;
            }
            match += (char) ch;
            switch (state)
            {
            case InComment:
                if (ch == '*')
                {
                    state = ExpectFSlash;
                    savedChar = ch;
                }
                else if (ch == '/')
                {
                    state = ExpectStar;
                    savedChar = ch;
                }
                else if (ch == '\n' || ch == '\f')
                {
                    state = ExpectTitanBug;
                    savedChar = ch;
                }
                else
                    ret += (char) ch;
                break;

            case ExpectFSlash:
                if (ch == '/')
                {
                    nestedCommentCounter--;
                    if (nestedCommentCounter == 0)
                        return ret;
                }
                state = InComment;
                ret += (char) savedChar;

                if (ch == '*')
                {
                    state = ExpectFSlash;
                    savedChar = ch;
                }
                else if (ch == '\n' || ch == '\f')
                {
                    state = ExpectTitanBug;
                    savedChar = ch;
                }
                else
                    ret += (char) ch;
                break;

            case ExpectStar:
                ret += (char) savedChar;
                ret += (char) ch;
                if (ch == '*')
                {
                    nestedCommentCounter++;
                    state = InComment;
                }
                else if (ch == '/')
                {
                    state = ExpectStar;
                }
                else
                {
                    state = InComment;
                }
                break;

            case ExpectTitanBug:
                if (ch == '>')
                {
                    // Sigh : there is also a \n to be removed
                    titan5500CommentNotEnded = true;
                    return ret;
                }
                state = InComment;
                ret += savedChar;

                if (ch == '*')
                {
                    state = ExpectFSlash;
                    savedChar = ch;
                }
                else if (ch == '\n' || ch == '\f')
                {
                    state = ExpectTitanBug;
                    savedChar = ch;
                }
                else
                    ret += (char) ch;
                break;
            }
        }
    }

    // Get a text string
    string getTextString(string& match)
    {
        string ret;
        // This gets TL1 characters until Eos or " reached
        // " can be escaped with
        // \ can be escaped with
        while (1)
        {
            int ch = m_chars.getTL1Character();
            if (ch == Eos)
            {
                m_chars.putACharacter(ch);
                return ret;
            }
            match += (char) ch;
            if (ch == '"')
            {
                return ret;  // Found End of Text String
            }
            if (ch == '\\')
            {
                ch = m_chars.getTL1Character();
                if (ch == Eos)
                {
                    m_chars.putACharacter(ch);
                    return ret;
                }
                match += (char) ch;
                if (ch == '\\')
                    ret += (char) ch;
                else if (ch == '"')
                {
                    // MR365: INNER STRING in a string
                    //  containing a string
                    ret += (char) '\\';
                    ret += (char) ch;
                    // This is an inner string
                    ret += getInnerInnerString(match);
                }
                else
                {
                    ret += ' ';
                }
                continue;
            }
            ret += (char) ch;
        }
        return ret;
    }

    // Get a general string
    string getGeneralString(string& match)
    {
        string ret;
        while (1)
        {
            int ch = m_chars.getTL1Character();
            TL1CharacterType t = characterType(ch);
            if ((t != Alf) && (t != Cha) && (t != Num))
            {
                m_chars.putACharacter(ch);
                return ret;
            }
            match += (char) ch;
            ret += (char) ch;
        }
    }

    // Get a format effector string
    string getFeFString(string& match)
    {
        string ret;
        while (1)
        {
            int ch = m_chars.getTL1Character();
            TL1CharacterType t = characterType(ch);
            if (t != Fef)
            {
                m_chars.putACharacter(ch);
                return ret;
            }
            match += (char) ch;
            ret += (char) ch;
        }
    }

    // Put a token back in the lex stream
    void putToken(const Token& token)
    {
        assert(m_pushedToken == false);
        m_pushedTokenValue = token;
        m_pushedToken = true;
    }

    // Get a token from the lex stream
    Token getToken(void)
    {
        if (m_pushedToken)
        {
            m_pushedToken = false;
            return m_pushedTokenValue;
        }
        Token ret;
        if (m_commentError)
        {
            m_commentError = false;
            ret.m_type = Comment;
            bool titanCommentBug;
            ret.m_value = getComment(ret.m_match, titanCommentBug);
            if (titanCommentBug)
                ret.m_type = CommentNoEnd;
            return ret;
        }
        while (1)
        {
            int ch = m_chars.getTL1Character();
            if (ch == Eos)
            {
                ret.m_type = EndToken;
                return ret;
            }
            if (characterType(ch) == Fef)
            {
                ret.m_type = FormatEffector;
                m_chars.putACharacter(ch);
                ret.m_value = getFeFString(ret.m_match);
                return ret;
            }

            if (characterType(ch) == Sep)
            {
                // The Sep Characters are :, ;, < and >
                ret.m_type = Character;
                ret.m_match = (char) ch;
                ret.m_value = (char) ch;
                return ret;
            }

            if (ch == '"')
            {
                ret.m_type = TextString;
                ret.m_match = (char) ch;
                ret.m_value = getTextString(ret.m_match);
                return ret;
            }

            if (ch == '/')
            {
                bool titanCommentBug;
                ret.m_match = (char) ch;
                ch = m_chars.getTL1Character();
                if (ch != '*')
                {
                    m_chars.putACharacter(ch);
                    ret.m_type = Character;
                    ret.m_value = (char) ch;
                }
                ret.m_type = Comment;
                ret.m_value = getComment(ret.m_match, titanCommentBug);
                if (titanCommentBug)
                    ret.m_type = CommentNoEnd;
                return ret;
            }
            if (ch == '\\')
            {
                ret.m_match = (char) ch;
                ch = m_chars.getTL1Character();
                if (ch != '"')
                {
                    m_chars.putACharacter(ch);
                    ret.m_type = Character;
                    ret.m_value = (char) ch;
                    return ret;
                }
                ret.m_type = InnerString;
                ret.m_value = getInnerString(ret.m_match);
                return ret;
            }

            TL1CharacterType t = characterType(ch);
            assert((t == Alf) || (t == Cha) || (t == Num));
            ret.m_type = General;
            m_chars.putACharacter(ch);
            ret.m_value = getGeneralString(ret.m_match);
            return ret;
        }

    }
private:

    LexCharacters m_chars;     // Lex character source
    bool m_commentError;   // Comment error encountered?
    Token m_pushedTokenValue;  // Pushed token value
    bool m_pushedToken;   // Pushed token?

};

//////////////////////////////////////////////////////////////////////////////////////////////
// Return a TL1 Lexer instance
//////////////////////////////////////////////////////////////////////////////////////////////
Lexer* createLexer(LexInputSource* source)
{
    if ( source == NULL )
        return (Lexer*)0;

    return new LexImplementation(source);
}

// }

//////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////////////////
IStreamLexInput::IStreamLexInput(istream& is)
        : m_is(is)
{
    // Intentionally Empty
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////////////////////
IStreamLexInput::~IStreamLexInput()
{
    // Intentionally Empty
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Get a character
//////////////////////////////////////////////////////////////////////////////////////////////
bool IStreamLexInput::getCharacter(char& ch)
{
    m_is.get(ch);
    if (m_is.eof() || m_is.fail())
        return false;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////////////////
StringLexInput::StringLexInput(const string& str)
        : m_str(str), m_pointer(str.c_str())
{
    // Intentionally Empty
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////////////////////
StringLexInput::~StringLexInput()
{
    // Intentionally Empty
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Get a character
//////////////////////////////////////////////////////////////////////////////////////////////
bool StringLexInput::getCharacter(char& ch)
{
    if (! (*m_pointer))
        return false;
    ch = *m_pointer++;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Get string currently pointed by m_pointer
//////////////////////////////////////////////////////////////////////////////////////////////
void StringLexInput::getCurrentStr (string& retStr)
{
    if ((m_pointer) && (*m_pointer))
        retStr = string (m_pointer);
    else
        retStr = "";
    return ;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////////////////
CharacterSource::CharacterSource(string& threadData)
        : m_currentPtr(0)
{
    m_threadData = ParseTL1::upper(threadData);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////////////////////
CharacterSource::~CharacterSource()
{
    // Intentionally Empty
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Get a character
//////////////////////////////////////////////////////////////////////////////////////////////
bool CharacterSource::getCharacter(char& ch)
{
    if (! m_currentPtr)
    {
        m_currentString = &m_threadData;
        m_currentPtr = m_currentString->c_str();
        if (m_currentString->size() == 0)
        {
            m_currentPtr = 0;
            return false;
        }
    }
    else if (*m_currentPtr == 0)
    {
        return false;
    }
    ch = *m_currentPtr++;

    return true;
}

} // LexerTL1 namespace

} // TL1_MA namespace
