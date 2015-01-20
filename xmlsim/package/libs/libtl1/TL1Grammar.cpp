/***********************************************************************************************
* COPYRIGHT 2001 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1Grammar.cpp 
* Description: 
* Implements processing of TL1 grammar 
*    
**********************************************************************************************/

#include <fstream>
#include "assert.h"
#include "TL1Grammar.h"
#include "TL1Parser.h"
#include "TL1ErrCodes.h"
#include "BaseException.h"

using namespace EMS_FW;

static const char *className = "TL1Grammar";

namespace TL1_MA
{

//
//  Implementation of CommandDB
//
namespace CommandDB
{
class Commands
{
public:
    typedef map<string, Command, less<string> > Map;
    Map m_map;

    void insert(const string& searchString, const Command& command)
    {
        m_map[searchString] = command;
    }

    Command* find(const string& name, const string& version)
    {
        string searchString = name;
        // First Search for specific and then for general
        Map::iterator it;
        it = m_map.find(searchString);

        if (version != "")
        {

            if (it == m_map.end())
            {
                searchString += ":";
                searchString += version;
                it = m_map.find(searchString);
            }
            // MR::If the version is 6.1 but we have not changed the tmCommands or
            // tmResponses file to reflect the new version in the commands, then we will
            // not find the command with version 6.1. So, instead of returning an error
            // we can check to see if the commands were supported on 6 and accept them
            // because they have not changed for 6.1
            if (it == m_map.end())
            {
                searchString = name + ":" + version.substr(0, 1); // search on major number
                it = m_map.find(searchString);
            }
        }

        // if we still do not find it then throw an error.
        if (it == m_map.end())
            return 0;
        return &((*it).second);
    }
};

static Commands s_commands;
static bool loaded = false;

Command* searchCommand(const string& name, const string& TL1version)
{
    if ( !loaded )
    {
        ParseTL1::throw_parseError("Command Grammar file not loaded", TL1_SROF);
    }
    return s_commands.find(name, TL1version);
}

void insertCommand(const string& name, const Command& command)
{
    s_commands.m_map[ParseTL1::upper(name)] = command;
}

/////////////////////////////
//Handle the TL1 Responses
/////////////////////////////
class Responses
{
public:
    typedef map<string, Response, less<string> > Map;
    Map m_map;

    void insert(const string& searchString, const Response& response)
    {
        m_map[searchString] = response;
    }

    Response* find(const string& name, const string& version)
    {
        string searchString = name;
        // First Search for specific and then for general
        Map::iterator it;
        it = m_map.find(searchString);
        if (version != "")
        {
            if (it == m_map.end())
            {
                searchString += ":";
                searchString += version;
                it = m_map.find(searchString);
            }
            // MR::If the version is 6.1 but we have not changed the tmCommands or
            // tmResponses file to reflect the new version in the commands, then we will
            // not find the command with version 6.1. So, instead of returning an error
            // we can check to see if the commands were supported on 6 and accept them
            // because they have not changed for 6.1
            if (it == m_map.end())
            {
                searchString = name + ":" + version.substr(0, 1); // search on major number
                it = m_map.find(searchString);
            }
        }
        // if we still do not find it then throw an error.
        if (it == m_map.end())
            return 0;

        return &((*it).second);
    }
};

static Responses s_responses;

Response* searchResponse(const string& name, const string& TL1version)
{
    if ( !loaded )
    {
        ParseTL1::throw_parseError("TL1 Responses Grammar file not loaded", TL1_SROF);
    }
    return s_responses.find(ParseTL1::upper(name), TL1version);
}

void insertResponse(const string& name, const Response& response)
{
    s_responses.m_map[ParseTL1::upper(name)] = response;
}

//
//  ------ Lexer -----
//

struct Token
{
    enum Type {StringWithDashes, Character, End};
    Type m_type;
    string m_value;
    int m_lineCount;
};

class Lexer
{
public:
    enum {Eos = 25};
    istream& m_is;

    Lexer(istream& is)
            : m_is(is),
            m_pushedCharacter(false),
            m_pushedToken(false),
            m_lineCount(1)
    {}

    char m_pushedCharacterValue;
    bool m_pushedCharacter;
    int m_lineCount;

    char getCharacter(void)
    {
        char ch;
        if (m_pushedCharacter)
        {
            m_pushedCharacter = false;
            return m_pushedCharacterValue;
        }
        m_is.get(ch);
        if (m_is.eof() || m_is.fail())
        {
            return (char) Eos;
        }
        if (ch == (char) Eos)
        {
            ch = 0;
        }
        if (ch == 10)
            m_lineCount++;
        return ch;
    }

    void pushCharacter(char ch)
    {
        assert(m_pushedCharacter == false);
        m_pushedCharacter = true;
        m_pushedCharacterValue = ch;
    }

    void skipWS()
    {
        char ch;
        while (1)
        {
            ch = getCharacter();
            if (! isspace(ch))
                break;
        }
        pushCharacter(ch);
    }

    bool isCharInStringWithDashes(char ch)
    {
        return (isalnum(ch) || ch == '-' || ch == '.' || ch == '_');
    }

    string getStringWithDashes(void)
    {
        string ret;
        while (1)
        {
            char ch = getCharacter();
            if (! isCharInStringWithDashes(ch))
            {
                pushCharacter(ch);
                break;
            }
            ret += ch;
        }
        return ret;
    }

    Token m_pushedTokenValue;
    bool m_pushedToken;

    Token getToken(void)
    {
        if (m_pushedToken)
        {
            m_pushedToken = false;
            return m_pushedTokenValue;
        }

        Token ret;
        skipWS();
        ret.m_lineCount = m_lineCount;
        char ch = getCharacter();
        if (isCharInStringWithDashes(ch))
        {
            pushCharacter(ch);
            ret.m_type = Token::StringWithDashes;
            ret.m_value = getStringWithDashes();
            return ret;
        }
        ret.m_value += (char) ch;
        if (ch == Eos)
            ret.m_type = Token::End;
        else
            ret.m_type = Token::Character;
        return ret;
    }

    void pushToken(const Token& token)
    {
        assert(m_pushedToken == false);
        m_pushedToken = true;
        m_pushedTokenValue = token;
    }
};

class Parser
{
public:
    Lexer m_lexer;
    Parser(istream& is)
            : m_lexer(is)
    {}

    void parseVersion(vector<string>& version)
    {
        try
        {
            Token token = m_lexer.getToken();
            if (token.m_type != Token::StringWithDashes)
                ParseTL1::throw_parseError("Looking for StringWithDashes, got " + token.m_value, TL1_SROF);

            while (token.m_type == Token::StringWithDashes
                   || token.m_value == ",")
            {
                if (token.m_value != ",")
                {
                    string temp = ":";
                    temp += token.m_value;
                    version.push_back(temp);
                }
                token = m_lexer.getToken();
            }
            if (token.m_value != "]")
                ParseTL1::throw_parseError("Looking for ']', got " + token.m_value, TL1_SROF);
        }
        catch (ParseTL1::Error& err)
        {
            ParseTL1::throw_parseError(err.m_what, TL1_SROF);
        }
    }

    void parseStringWithDashes(const char* name, string& str)
    {
        Token token = m_lexer.getToken();
        if (token.m_type != Token::StringWithDashes)
            ParseTL1::throw_parseError(
                string("Looking for String ") + name + ", got " + token.m_value, TL1_SROF);
        str = token.m_value;
    }

    void ignoreValue(const string& value)
    {
        Token token = m_lexer.getToken();
        if (token.m_value != value)
            m_lexer.pushToken(token);
    }

    void parseValue(const char* name, const string& value)
    {
        Token token = m_lexer.getToken();
        if (token.m_value != value)
            ParseTL1::throw_parseError(
                string("Looking for Value ") + value + string(" ") + name + ", got " +
                token.m_value, TL1_SROF);
    }

    void parseInt(const char* name, int& val)
    {
        Token token = m_lexer.getToken();
        if (token.m_type != Token::StringWithDashes)
            ParseTL1::throw_parseError(
                string("Looking for Int ") + name + ", got " + token.m_value, TL1_SROF);
        val = atoi(token.m_value.c_str());
    }

    void parseCommandAttributes(Command& command)
    {
        string category;
        CommandAttributes& attributes = command.m_attributes;
        try
        {
            string appendString;
            while (1)
            {
                Token token = m_lexer.getToken();
                if (token.m_value == "}")
                    return ;
                if (token.m_value != ",")
                    m_lexer.pushToken(token);
                CommandAttribute attr;
                parseInt("CommandAttribute:block", attr.m_block);
                ignoreValue(",");
                parseInt("CommandAttribute:position", attr.m_position);
                ignoreValue(",");
                parseStringWithDashes("CommandAttribute:category",
                                      category);
                if (category == "OPTIONAL")
                {
                    attr.m_catagory = CAT_OPTIONAL;
                }
                else if (category == "MANDATORY")
                {
                    attr.m_catagory = CAT_MANDATORY;
                }
                else if (category == "DEPENDENTS")
                {
                    attr.m_catagory = CAT_DEPENDENTS;
                }
                else if (category == "DEPENDENCY")
                {
                    attr.m_catagory = CAT_DEPENDENCY;
                }
                else
                {
                    ParseTL1::throw_parseError(string("Unknown Category: ") + category, TL1_SROF);
                }

                ignoreValue(",");
                parseStringWithDashes("CommandAttribute:name",
                                      attr.m_name);
                attr.m_name = ParseTL1::upper(attr.m_name);
                ignoreValue(",");

                //
                //

                if (attr.m_catagory == CAT_DEPENDENTS)
                {
                    command.m_special = true;
                    appendString = string(":") + attr.m_name;
                }
                else if (appendString.size())
                    attr.m_name += appendString;

                attributes.push_back(attr);
            }
        }
        catch (ParseTL1::Error& err)
        {
            ParseTL1::throw_parseError(err.m_what, TL1_SROF);
        }
    }

    bool parseCommand()
    {
        Command command;
        try
        {
            Token token = m_lexer.getToken();
            if (token.m_type == Token::End)
                return false;
            if (token.m_type != Token::StringWithDashes)
            {
                ParseTL1::throw_parseError("Looking for StringWithDashes, got " + token.m_value, TL1_SROF);
            }
            command.m_name = token.m_value;
            token = m_lexer.getToken();
            if (token.m_value == "<")
            {
                token = m_lexer.getToken();
                command.m_handleFields = ParseTL1::stoi(token.m_value);
                ignoreValue(">");
                token = m_lexer.getToken();
            }

            vector<string> version;
            if (token.m_value == "[")
            {
                parseVersion(version);
                token = m_lexer.getToken();
            }
            else
                //command is FP version non-specific
                version.push_back("");

            if (token.m_value != "{")
                ParseTL1::throw_parseError("Looking for '{', got " + token.m_value, TL1_SROF);

            parseCommandAttributes(command);

            // Place in the DataBase HERE ??
            vector<string>::iterator it;
            for (it = version.begin(); it != version.end(); ++it)
                insertCommand(command.m_name + (*it), command);

        }
        catch (ParseTL1::Error& err)
        {
            ParseTL1::throw_parseError(err.m_what, TL1_SROF);
        }

        return true;
    }
    ////////////////////////
    //Handle the responses
    ////////////////////////
    void parseResponseAttributes(Response& response)
    {
        // A Attribute has three strings with ',' or } to end
        try
        {
            while (1)
            {
                Token token = m_lexer.getToken();
                if (token.m_value == "}")
                    return ;
                if (token.m_value != ",")
                    m_lexer.pushToken(token);
                ResponseAttribute attr;
                string num;
                parseStringWithDashes("ResponseAttribute:block", num);
                attr.m_block = atoi(num.c_str());
                parseValue("ResponseAttribute:block", ",");
                parseStringWithDashes("ResponseAttribute:block", num);
                attr.m_position = atoi(num.c_str());
                parseValue("ResponseAttribute:block", ",");
                parseStringWithDashes("ResponseAttribute:block",
                                      attr.m_name);

                response.m_attributes.push_back(attr);

            }
        }
        catch (ParseTL1::Error& err)
        {
            err.m_what += "\nIn parse ResponseAttributes";
            ParseTL1::throw_parseError(err.m_what, TL1_SROF);
        }

    }

    bool parseResponse()
    {
        Response response;

        try
        {
            Token token = m_lexer.getToken();
            if (token.m_type == Token::End)
                return false;
            if (token.m_type != Token::StringWithDashes)
            {
                ParseTL1::throw_parseError("Looking for StringWithDashes, got " + token.m_value, TL1_SROF);
            }

            response.m_name = token.m_value;
            token = m_lexer.getToken();

            vector<string> version;
            //get version specifics
            if (token.m_value == "[")
            {
                parseVersion(version);
                token = m_lexer.getToken();
            }
            else
            {
                //response is FP version non-specific
                version.push_back("");
            }

            if (token.m_value != "{")
                ParseTL1::throw_parseError("Looking for '{', got " + token.m_value, TL1_SROF);


            parseResponseAttributes(response);

            // Place in the DataBase HERE ??
            vector<string>::iterator it;
            for (it = version.begin(); it != version.end(); ++it)
                insertResponse(response.m_name + (*it), response);

            return true;
        }
        catch (ParseTL1::Error& err)
        {
            err.m_what += "\nIn parse Response";
            ParseTL1::throw_parseError(err.m_what, TL1_SROF);
        }

    }

};

void initResponseDB(const string& fileName)
{
    const char *methodName = "initResponseDB";
    try
    {
        ifstream is;
        is.open(fileName.c_str(), ios::in | ios::nocreate);
        if (is.fail())
            throw BaseException("Unable to open " + fileName, className, methodName);

        Parser parser(is);

        while (parser.parseResponse())
            ;
    }
    catch (ParseTL1::ParseError &err)
    {
        throw BaseException("Error Processing Response Grammar: " + err.m_reason, className, methodName);
    }
}

void initCommandDB(const string &fileName)
{
    const char *methodName = "initCommandDB";

    if ( !loaded )
    {
        try
        {
            ifstream is;
            is.open(fileName.c_str(), ios::in | ios::nocreate);
            if (is.fail())
                throw BaseException("Unable to open " + fileName, className, methodName);

            Parser parser(is);

            while (parser.parseCommand())
                ;
            loaded = true;
        }
        catch (ParseTL1::ParseError &err)
        {
            throw BaseException("Error Processing Command Grammar: " + err.m_reason, className, methodName);
        }
    }
}
}

}
