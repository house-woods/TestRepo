/***********************************************************************************************
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1Parser.cpp
* Description: 
* Implements TL1 Command parser 
* 
**********************************************************************************************/

#include <stdio.h>
#include <map>
#include <string>
#include "TL1Lexer.h"
#include "TL1Parser.h"
#include "TL1Grammar.h"
#include "TL1Resp.h"
#include "TL1ErrCodes.h"




ostream& operator<< (ostream& os, TL1Message::Type& type)
{
    switch (type)
    {
    case TL1Message::TestCommand:
        os << "TestCommand";
        break;
    case TL1Message::None:
        os << "None";
        break;
    case TL1Message::System:
        os << "System";
        break;
    case TL1Message::Log:
        os << "Log";
        break;
    case TL1Message::Acknowledgement:
        os << "Acknowledgement";
        break;
    case TL1Message::Response:
        os << "Response";
        break;
    case TL1Message::Autonomous:
        os << "Autonomous";
        break;
    case TL1Message::ParseError:
        os << "ParseError";
        break;
    case TL1Message::Ignore:
        os << "Ignore";
        break;
    case TL1Message::End:
        os << "End";
        break;
    case TL1Message::Err:
        os << "Err";
        break;
    case TL1Message::Timeout:
        os << "Timeout";
        break;
    case TL1Message::Rejected:
        os << "Rejected";
        break;
    default:
        os << "TYPEINVALID";
    }
    return os;
}

ostream& operator<< (ostream& os, TextBlock::Type type)
{
    switch (type)
    {
    case TextBlock::None:
        os << "None";
        break;
    case TextBlock::Comment:
        os << "Comment";
        break;
    case TextBlock::CommentNoEnd:
        os << "CommentNoEnd";
        break;
    case TextBlock::Quoted:
        os << "Quoted";
        break;
    case TextBlock::Unquoted:
        os << "Unquoted";
        break;
    default:
        os << "TYPEINVALID";
    }
    return os;
}

ostream& operator<< (ostream& os, TextBlock& textBlock)
{
    os << "TYPE " << textBlock.m_type << endl;
    os << textBlock.m_value << endl;
    return os;
}

ostream& operator<< (ostream& os, TL1Message& msg)
{
    os << endl
    << "-------------- TL1Message " << msg.m_type << " ----------"
    << endl
    << msg.m_match
    << endl
    << "-------- Decoded ";
    os << (msg.m_finished ? "" : "PARTIAL") << " ----------"
    << endl;
    switch (msg.m_type)
    {
    case TL1Message::Response:
        os << " CTAG: " << msg.m_tag
        << " COMPLETECODE: " << msg.m_completeCode;
        break;
    case TL1Message::Autonomous:
        os << " ATAG: " << msg.m_tag
        << " CODE: " << msg.m_code;
        break;
    case TL1Message::Acknowledgement:
        os << " CTAG: " << msg.m_tag
        << " CODE: " << msg.m_code;
        break;
    case TL1Message::TestCommand:
        os << " CTAG: " << msg.m_tag
        << " COMMAND: " << msg.m_code;
        break;
    case TL1Message::Timeout:
        break;
    case TL1Message::Rejected:
        os << " Reason: " << msg.m_reason;
        break;
    }
    os << endl;
    for (int i = 0; i < msg.m_textBlocks.size(); i++)
    {
        os << "TextBlock [" << i << "] ";
        os << msg.m_textBlocks[i];
    }

    os << "---------------------------"
    << endl;
    return os;
}

namespace TL1_MA
{

namespace ParseTL1
{
static string g_version = "";  // Global version of commands to be read from grammar file

const int TIDPOS = 8;  // Tid required bit identifier
const int AIDPOS = 4;  // Aid required bit identifier
const int CTAGPOS = 2;  // Ctag required bit identifier
const int GBPOS = 1;  // Genblock required bit identifier

///////////////////////////////////////////////////////////////////////////////////////////////
// Split a string based on provided delimiter
///////////////////////////////////////////////////////////////////////////////////////////////
vector<string> split(const string& str, char splitter)
{
    vector<string> ret;
    if (str.size() == 0)
        return ret;
    int pos = 0;
    int endPos;
    while (1)
    {
        endPos = str.find(splitter, pos);
        if (endPos == -1)
            endPos = str.size();
        ret.push_back(string());
        string& a = ret.back();
        a.assign(str, pos, endPos - pos);
        if ((unsigned int)endPos == str.size())
            break;
        pos = endPos + 1;
    }
    return ret;
}

bool tokenIsCmdCode(LexerTL1::Token& token)
{
    if (token.m_type != LexerTL1::General)
        return false;
    return token.m_value == "CMD";
}

bool tokenIsAckCode(LexerTL1::Token& token)
{
    if (token.m_type != LexerTL1::General)
        return false;
    if (token.m_value == "IP"
        || token.m_value == "OK"
        || token.m_value == "PF"
        || token.m_value == "NA"
        || token.m_value == "NG"
        || token.m_value == "RL"
       )
        return true;
    return false;
}

bool tokenIsLogCode(LexerTL1::Token& token)
{
    return (token.m_value == "L");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Determine whether token is a TL1 Command identifier
///////////////////////////////////////////////////////////////////////////////////////////////
CommandDB::Command *tokenIsTL1Command(LexerTL1::Token& token)
{
    string g_version;
    CommandDB::Command *result = NULL;

    if (token.m_type == LexerTL1::General)
    {
        try
        {
            result = CommandDB::searchCommand(token.m_value, g_version);
        }
        catch (ParseError &err)
        {
            throw err;
        }
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// TL1 Parser Implementation
///////////////////////////////////////////////////////////////////////////////////////////////
class ParserImplementation : public Parser
{
public:
    // Constructor
    ParserImplementation(LexerTL1::Lexer* source)
            : m_lexer(source)
    {}


private:
    // Parser attribute description
    struct AttributeDesc
    {
        string m_name;
        int m_block;
        int m_position;
        int m_seen;
        int m_catagory;
        AttributeDesc(const CommandAttribute& desc)
                : m_name(desc.m_name),
                m_block(desc.m_block),
                m_position(desc.m_position),
                m_catagory(desc.m_catagory),
                m_seen(0)
        {}
        ;
        AttributeDesc() : m_seen(0)
        {}
    }
    ;

    // Attribute map
    typedef map<string, AttributeDesc, less<string> > Map;

    // general purpose parse
    void parseGeneral(const char* name, string& genString)
    {
        LexerTL1::Token token = m_lexer->getToken();
        if (token.m_type != LexerTL1::General)
        {
            throw_parseError(string("parsing ") + name, TL1_IISP);
        }
        genString = token.m_value;
    }

    // parse a Tid, Aid, Ctag, and GenBlock
    void parseTL1Field(const char *name, string &genString, int manditory)
    {
        genString = "";

        // Determine if token is preceded with a colon
        LexerTL1::Token token = m_lexer->getToken();
        if (token.m_value == ":")
        {
            // Make sure that the next token is not a ":"
            token = m_lexer->getToken();
            if (token.m_type == LexerTL1::Character)
            {

                // We did not find the specific field.  Was it mandatory?
                m_lexer->putToken(token);
                if (manditory)
                {
                    // Parse error has occurred
                    throw_parseError(string("Missing ") + name, TL1_IPMS);
                }
            }
            else
            {
                // Save the results of the parse
                genString = token.m_value;
            }
        }
        else
        {
            // Did not find field.  If it was mandatory report a parse error
            m_lexer->putToken(token);
            if (manditory)
            {
                throw_parseError(string("Missing ") + name, TL1_IPMS);
            }
        }
    }

    // parse a TL1 payload
    void parseTL1Payload()
    {
        int block = 0;
        int position = 0;
        string genString = "";

        LexerTL1::Token token = m_lexer->getToken();

        // Is there anything left to parse
        if (token.m_type == LexerTL1::EndToken)
        {
            throw_parseError("Statement does not end with ;", TL1_IISP);
        }

        // Loop over the blocks of the payload
        while (token.m_value == ":")
        {
            block++;
            position = 1;
            token = m_lexer->getToken();

            while (token.m_value != ":")
            {

                // Determine if the end of the statement was reached
                if (token.m_value == ";")
                {
                    break;
                }

                // Process anything but a comma
                if (token.m_value != ",")
                {

                    // Assume that a name value pair was passed
                    vector<string> parm = split(token.m_value, '=');
                    vector<string>::iterator it = parm.begin();

                    //set-profile-user and ed-chan commands are Name-Defined, others are position defined
                    if (message->m_command == "SET-PROFILE-USER" || message->m_command == "ED-CHAN")
                    {
                        // Determine if a name value pair was passed
                        if (parm.size() == 2)
                        {
                            message->m_params.insert((*it), (*(it + 1)), -position, block);
                        }
                        else
                        {
                            message->m_params.insert("", (*it), position, block);
                        }
                    }
                    else
                    {
                        // Determine if a name value pair was passed
                        if (parm.size() == 2)
                        {
                            message->m_params.insert((*it), (*(it + 1)), position, block);
                        }
                        else
                        {
                            message->m_params.insert("", (*it), position, block);
                        }
                    }
                }
                position++;

                // Get the next token and validate whether there is any more data to parse
                token = m_lexer->getToken();
                if (token.m_type == LexerTL1::EndToken)
                {
                    throw_parseError("Statement does not end with ;", TL1_IISP);
                }
            }
        }

        // Validate the payload
        if ( !validateAttributes() )
        {
            throw_parseError("Payload Invalid", TL1_IISP);
        }
    }

    // Convert a string to a list of name value pairs
    void toNameValue(string buffer, NameValues &nv)
    {
        if (buffer != "")
        {

            // Split the strings by commas
            vector<string> pairs = split(buffer, ',');

            // Loop over each string created
            for (vector<string>::iterator it = pairs.begin(); it != pairs.end(); it++)
            {
                // Split the string by equal signs
                vector<string> pair = split((*it), '=');
                vector<string>::iterator it2 = pair.begin();

                // deternube if a namevalue pair was returned.
                if (pair.size() != 1)
                {
                    nv.insert((*it2), (*(it2 + 1)));
                }
                else
                {
                    // Special treatment for AID
                    if (pairs.size() == 1)
                    {
                        nv.insert("AID", (*it2));
                    }
                    else
                    {
                        nv.insert("", (*it2));
                    }
                }
            }
        }
        else
        {
            nv.insert("", "");
        }
    }

    // Run steps to parse a TL1 Command;
    void parseTL1Command()
    {
        string buffer;
        try
        {
            // Ge the command id
            parseGeneral("TL1:Command", message->m_command);

            // Get the tid, if present
            parseTL1Field("Tid", message->m_tid, TIDPOS & m_commandDesc->m_handleFields);

            // Get the aid, if present
            parseTL1Field("Aid", buffer, AIDPOS & m_commandDesc->m_handleFields);

            // Convert the aid to name value pairs
            toNameValue(buffer, message->m_aid);

            // Get the Ctag, if present
            parseTL1Field("Ctag", message->m_ctag, CTAGPOS & m_commandDesc->m_handleFields);

            // Get the GenBlock, if present
            parseTL1Field("Gb", message->m_gb, GBPOS & m_commandDesc->m_handleFields);

            // Get the payload, if present
            parseTL1Payload();
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    // Insert payload attributes in the proper locations
    void addOutAttribute(const TL1PayloadAttribute& attribute)
    {
        PayloadList::iterator it ;

        for (it = message->m_attributes.begin();
             it != message->m_attributes.end();
             ++it)
        {
            if ((*it).m_block > attribute.m_block)
                break;
            if ((*it).m_block < attribute.m_block)
                continue;

            if ((*it).m_position > attribute.m_position)
                break;
        }
        message->m_attributes.insert(it, attribute);
    }

    // Fill in the command description
    void fillDesc(void)
    {
        CommandDB::CommandAttributes::iterator it;
        for (it = m_commandDesc->m_attributes.begin();
             it != m_commandDesc->m_attributes.end(); ++it)
        {
            m_descriptionMap[(*it).m_name] = AttributeDesc(*it);
        }
    }

    // Get the command description
    AttributeDesc* getDescription(const string& name)
    {
        string search = name + m_specialPostfix;
        Map::iterator it = m_descriptionMap.find(search);
        if (it == m_descriptionMap.end())
            return 0;
        return &(*it).second;
    }

    // Get attribute name for a payload item position
    string *getAttributeName(int position, int block, int newpos)
    {
        CommandDB::CommandAttributes::iterator it;
        if (position > 2)
            position = newpos;

        for (it = m_commandDesc->m_attributes.begin();
             it != m_commandDesc->m_attributes.end(); ++it)
        {
            if ((*it).m_block == block && (*it).m_position == position)
            {
                return &(*it).m_name;
            }
        }
        return 0;
    }

    // Get special handling information
    AttributeDesc* getSpecial()
    {
        for (NameValues::iterator it = message->m_params.begin();
             it != message->m_params.end();
             ++it)
        {
            AttributeDesc* attrDesc = getDescription((*it).name());
            if (attrDesc)
            {
                m_specialPostfix = ":" + attrDesc->m_name;
                message->m_params.erase(it);
                return attrDesc;
            }
        }
        return 0;
    }

    // Validate a TL1 payload based on the command attributes
    bool validateAttributes(void)
    {
        fillDesc();
        processAttributes();
        CommandDB::CommandAttributes::iterator it;
        for (it = m_commandDesc->m_attributes.begin();
             it != m_commandDesc->m_attributes.end(); ++it)
        {
            AttributeDesc *desc = getDescription((*it).m_name);
            if (desc != NULL)
            {
                if (desc->m_catagory == CAT_MANDATORY)
                {
                    if ( !desc->m_seen )
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    // Process a TL1 command payload
    void processAttributes(void)
    {
        AttributeDesc* desc;
        if (m_commandDesc->m_special)
        {
            desc = getSpecial();
            if (! desc)
                throw_parseError("Unable to find special", TL1_IISP);
            desc->m_seen++;
            addOutAttribute(
                TL1PayloadAttribute(desc->m_block, desc->m_position,
                                    "", desc->m_name));
        }

        int size = message->m_params.size();
        int paramCnt = 0;

        for (NameValues::iterator it = message->m_params.begin();
             it != message->m_params.end();
             ++it)
        {
            try
            {
                //Check if the command is Name-Defined
                if ((*it).position() < 0)
                {
                    ++paramCnt; //Number of parameters
                    NameValues::iterator it2 = message->m_params.begin(); //iterator to compare against

                    //Check for the duplicate parameter starting from the 2nd
                    if (paramCnt != 1)
                    {
                        //compare it.name against the names in it2 to check for any duplicate parameter
                        for (int i = 1; i < paramCnt; i++)
                        {
                            //Check if the parameter already exists in the payload
                            if ((*it).name() == (*it2).name())
                            {
                                throw_parseError(string("Command: ") + message->m_command +
                                                 string(" - Duplicate parameter ") + (*it).name(), TL1_IPNC);
                            }

                            ++it2; //Move the iterator to the next position to compare
                        }
                    }

                    desc = getDescription((*it).name());

                    if (desc->m_block != (*it).block())
                    {
                        throw_parseError(string("Command: ") + message->m_command +
                                         string(" - parameter in wrong block ") + (*it).name(), TL1_IPNV);
                    }
                    if (desc->m_position != 0 && desc->m_position != -(*it).position())
                    {
                        throw_parseError(string("Command: ") + message->m_command +
                                         string(" - parameter in wrong position ") + (*it).name(), TL1_IPNV);
                    }
                }
                else
                {
                    int count = 0;
                    int newpos = 0;
                    //check for previous values in the list for recalculating the positions
                    if ((*it).position() == 3)
                    {
                        int n = (*it).position() - 1;
                        for (n = n; n != 0; n--)
                        {
                            string val = message->m_params.findValue(n);
                            if (val.size())
                                ++count;
                        }
                        newpos = (*it).position() - count;
                    }

                    if (((*it).position() == 4) || ((*it).position() == 5))
                    {
                        int n = (*it).position() - 1;
                        for (n = n; n != 0; n--)
                        {
                            string val = message->m_params.findValue(n);
                            if (val.size())
                                ++count;
                        }
                        newpos = (*it).position() - count;
                    }

                    if ((*it).position() >= 6)
                    {
                        newpos = 4;
                    }

                    string *name = getAttributeName((*it).position(), (*it).block(), newpos);
                    if (*name != (*it).name() && (*it).name() != "")
                    {
                        throw_parseError(string("Command: ") + message->m_command +
                                         string(" - invalid Payload position ") + (*it).name(), TL1_IPNV);

                    }
                    desc = getDescription(*name);
                }
                if (!desc)
                {
                    throw_parseError(string("Command: ") + message->m_command +
                                     string(" - unknown  attribute ") + (*it).name(), TL1_IPNV);
                }
            }
            catch (ParseError &err)
            {
                throw err;
            }
            catch (...)
            {
                throw_parseError(string("Command: ") + message->m_command +
                                 string(" - invalid format ") + (*it).name(), TL1_IPNV);
            }
            desc->m_seen++;
            addOutAttribute(
                TL1PayloadAttribute(desc->m_block, desc->m_position,
                                    desc->m_name, (*it).value()));
        }
    }
    //////////////////////////////////////////////////////////////////////////////

    void parseFormatEffector(const char* name)
    {

        LexerTL1::Token token = m_lexer->getToken();
        if (token.m_type != LexerTL1::FormatEffector)
        {
            throw_parseError(string("parsing ") + name, "No FEF");
        }
    }

    void eatFormatEffector(const char* name)
    {

        LexerTL1::Token token = m_lexer->getToken();
        if (token.m_type != LexerTL1::FormatEffector)
        {
            m_lexer->putToken(token);
        }
    }

    void parseCharacter(const char* name, char ch)
    {

        LexerTL1::Token token = m_lexer->getToken();
        if (! LexerTL1::tokenIsCharacter(token, ch))
        {
            throw_parseError(string("parsing ") + name, "No Char");
        }
    }

    void parseDate(const char* name, DateTime& dateTime)
    {
        string dateField;

        try
        {
            parseGeneral("Date::field", dateField);
            vector<string> fields = split(dateField, '-');
            if (fields.size() != 3)
                throw_parseError("Date::numberoffields-" + dateField, "No Fields");
            dateTime.m_year = stoi(fields[0]);
            dateTime.m_mon = stoi(fields[1]);
            dateTime.m_day = stoi(fields[2]);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseTime(const char* name, DateTime& dateTime)
    {
        string x;

        try
        {
            parseGeneral("Time:DD1", x);
            dateTime.m_hour = stoi(x);
            parseCharacter("Time:Sep1", ':');
            parseGeneral("Time:DD2", x);
            dateTime.m_min = stoi(x);
            parseCharacter("Time:Sep2", ':');
            parseGeneral("Time:DD2", x);
            dateTime.m_sec = stoi(x);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseAcknowledgement(TL1Message& msg)
    {
        string x;


        // Acknowledgement is
        //  <AckCode> <sp> <ctag> <cr> <lf> '<'
        // This code assumes that ACK code is general two characters

        try
        {
            parseGeneral("Ack:Code", msg.m_code);
            parseFormatEffector("Ack:FEF:1");
            parseGeneral("Ack:Ctag", msg.m_tag);
            parseFormatEffector("Ack:FEF:2");
            parseCharacter("Ack:End", '<');
        }
        catch (ParseError& err)
        {
            throw err;
        }

    }


    void parseHeader(TL1Message& msg)
    {


        // Header is <cr> <lf> <lf> <sp> <sp> <sp> <sid>
        //           <sp> <date> <sp> <time>

        string x;
        try
        {
            // parseFormatEffector("Header:FEF:1");
            parseGeneral("Header:SID", msg.m_sid);
            parseFormatEffector("Header:FEF:2");
            parseDate("Header:Date", msg.m_dateTime);
            parseFormatEffector("Header:FEF:3");
            parseTime("Header:Time", msg.m_dateTime);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseResponseId(TL1Message& msg)
    {

        try
        {
            parseFormatEffector("ResponseId:FEF:1");
            parseGeneral("ResponseId:Ctag", msg.m_tag);
            parseFormatEffector("ResponseId:FEF:2");
            parseGeneral("ResponseId:Code", msg.m_completeCode);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseAutoId(TL1Message& msg)
    {

        try
        {
            parseFormatEffector("ResponseId:FEF:1");
            parseGeneral("ResponseId:Atag", msg.m_tag);
            parseFormatEffector("ResponseId:FEF:2");
            // There can be up to three Idents
            parseGeneral("ResponseId:OuputCode:Ident:1",
                         msg.m_outputCode1);
            LexerTL1::Token token = m_lexer->getToken();
            if (! LexerTL1::tokenIsFormatEffector(token, " "))
            {
                m_lexer->putToken(token);
                return ;
            }

            parseGeneral("ResponseId:OuputCode:Ident:2",
                         msg.m_outputCode2);
            token = m_lexer->getToken();
            if (! LexerTL1::tokenIsFormatEffector(token, " "))
            {
                m_lexer->putToken(token);
                return ;
            }

            parseGeneral("ResponseId:OuputCode:Ident:2",
                         msg.m_outputCode3);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseResponseOrAutonomousId(TL1Message::Type& type,
                                     TL1Message& msg)
    {

        try
        {
            // Next is either response id or auto id
            // both start with <cr> <lf> <code>
            eatFormatEffector("ResponseOrAutonomousId:FEF:1");
            parseGeneral("ResponseOrAutonomousId:Code", msg.m_code);
            if (msg.m_code == "M")
            {
                type = TL1Message::Response;
                parseResponseId(msg);
            }
            else
            {
                type = TL1Message::Autonomous;
                parseAutoId(msg);
            }
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseOutputUnit(string& parsedString)
    {
        string x;

        try
        {
            // Currently general includes '=' and && stuff
            parseGeneral("OutputUnit:param:1", x);
            parsedString += x;
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseUnquotedTextBlock(TL1Message& msg)
    {
        string parsedString;

        try
        {
            while (1)
            {
                parseOutputUnit(parsedString);
                // Read the next token, if it is a "," Skip it
                // Note: this is currently not needed as "," is in general
                LexerTL1::Token token = m_lexer->getToken();
                if (token.m_value == ",")
                {
                    parsedString += token.m_match;
                    token = m_lexer->getToken();
                }
                // Error in TITAN TL1
                if (token.m_value == ":")
                {
                    parsedString += token.m_match;
                    token = m_lexer->getToken();
                }
                // If the token is not WS
                if (! LexerTL1::tokenIsWS(token) &&
                    ! (token.m_type == LexerTL1::General) // TITAN 5500 Bug
                   )
                {
                    msg.m_textBlocks.push_back(
                        TextBlock(TextBlock::Unquoted, parsedString));
                    m_lexer->putToken(token);
                    return ;
                }
                if (token.m_type == LexerTL1::General)
                    m_lexer->putToken(token);
                else
                    parsedString += token.m_match;
            }
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseTextBlock(TL1Message& msg)
    {

        try
        {
            // There are three different type of text block
            // Comment, Quoted and Unquoted

            LexerTL1::Token token = m_lexer->getToken();
            if (token.m_type == LexerTL1::Comment)
            {
                msg.m_textBlocks.push_back(
                    TextBlock(TextBlock::Comment, token.m_value));
                return ; // Parsed a comment
            }
            if (token.m_type == LexerTL1::TextString)
            {
                msg.m_textBlocks.push_back(
                    TextBlock(TextBlock::Quoted, token.m_value));
                return ; // Parsed a textString :: Need to parse this later
            }
            m_lexer->putToken(token);
            parseUnquotedTextBlock(msg);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseTextBlocks(TL1Message& msg)
    {

        try
        {
            while (1)
            {
                parseFormatEffector("ParseTextBlocks:FEF:1");
                LexerTL1::Token token = m_lexer->getToken();
                if (token.m_type == LexerTL1::CommentNoEnd)
                {
                    // s_responseComments.insert(msg.m_tag);
                    msg.m_textBlocks.push_back(
                        TextBlock(TextBlock::CommentNoEnd, token.m_value));
                    msg.m_finished = false;
                    return ;
                }
                if (LexerTL1::tokenIsCharacter(token, '>'))
                {
                    msg.m_finished = false;
                    return ;
                }
                if (LexerTL1::tokenIsCharacter(token, ';'))
                {
                    return ;
                }
                m_lexer->putToken(token);
                // Must be a TextBlock
                parseTextBlock(msg);
            }
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseResponseOrAutonomous(TL1Message::Type& type, TL1Message& msg)
    {

        try
        {
            parseHeader(msg);
            parseResponseOrAutonomousId(type, msg);
            // Fix: for TITAN COMMENT BUG
            /*****
            if (s_responseComments.present(msg.m_tag))
                   {
                       s_responseComments.remove(msg.m_tag);
                       m_lexer->setCommentError();
                       LexerTL1::Token token = m_lexer->getToken();
                       if (token.m_type == LexerTL1::CommentNoEnd)
                       {
                           s_responseComments.insert(msg.m_tag);
                           msg.m_textBlocks.push_back(
                               TextBlock(TextBlock::CommentNoEnd,
                                         token.m_value));
                           msg.m_finished = false;
                           return;
                       }
                       if (token.m_type != LexerTL1::Comment)
                       {
                           throw_parseError("COMMENT BUG:","No End Found");
                       }
                       msg.m_textBlocks.push_back(
                           TextBlock(TextBlock::Comment,
                                     token.m_value));
                   }
            ***/
            parseTextBlocks(msg);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    // parseAutonomous: this is a message in a rtrv-ao command

    void parseAutonomous(TL1Message::Type& type, TL1Message& msg)
    {

        try
        {
            parseHeader(msg);
            parseResponseOrAutonomousId(type, msg);
            eatFormatEffector("autonomous:FEF");
            parseTextBlock(msg); // Only One assummed
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseSystemMessage(TL1Message& msg)
    {

        try
        {
            TL1Message::Type type;
            parseResponseOrAutonomousId(type, msg);
            parseTextBlocks(msg);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseInputCmd(const char* name)
    {

        try
        {
            // "Command text as input by the client application" ???
            // --- parse until ";" initial
            while (1)
            {
                LexerTL1::Token token = m_lexer->getToken();
                if (token.m_type == LexerTL1::EndToken)
                {
                    throw_parseError(string("Error: ") + name, "Missing ;");
                }
                if (token.m_value == ";")
                    break;
            }
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseLogCmd(void)
    {
        DateTime dummyDateTime;
        string dummyString;

        try
        {
            parseFormatEffector("LogCmd:FEF:1");
            parseGeneral("LogCmd:Date", dummyString);
            parseFormatEffector("LogCmd:FEF:2");
            parseTime("LogCmd:Time", dummyDateTime);
            parseFormatEffector("LogCmd:FEF:3");
            parseGeneral("LogCmd:OpChan", dummyString);
            parseInputCmd("LogCmd:InputCmd");
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseLogAuto(void)
    {
        string x;

        try
        {
            parseFormatEffector("LogResp:FEF:1");
            parseGeneral("LogResp:AllorOpChan", x);

            // Now the command :- Get the FormatEffector here
            parseFormatEffector("LogResp:FEF:4");
            TL1Message::Type type;
            TL1Message msg;
            parseResponseOrAutonomous(type, msg);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseLogResp(void)
    {
        DateTime dummyDateTime;
        string x;

        try
        {
            parseFormatEffector("LogResp:FEF:1");
            parseGeneral("LogResp:Date", x);
            parseFormatEffector("LogResp:FEF:2");
            parseTime("LogResp:Time", dummyDateTime);
            parseFormatEffector("LogResp:FEF:3");
            parseGeneral("LogResp:OpChan", x);

            // Now the command :- Get the FormatEffector here
            parseFormatEffector("LogResp:FEF:4");
            TL1Message::Type type;
            TL1Message msg;
            parseResponseOrAutonomous(type, msg);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseTestCommand(TL1Message& msg)
    {
        string x;

        try
        {
            parseGeneral("TestCommand:Cmd", x);
            parseFormatEffector("TestCommand:FEF:1");
            parseGeneral("TestCommand:command", msg.m_code);
            parseFormatEffector("TestCommand:FEF:2");
            parseGeneral("TestCommand:ctag", msg.m_tag);
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    void parseLogMessage(void)
    {
        string x;

        try
        {
            parseGeneral("LogMessage:L", x);
            parseFormatEffector("LogMessage:FEF:1");
            parseGeneral("LogMessage:Type", x);
            if (x == "CMD")
                parseLogCmd();
            else if (x == "AUTO")
                parseLogAuto();
            else if (x == "RESP")
                parseLogResp();
            else
            {

                throw_parseError(string("Error:") + x, "Unexpected Log Type");
            }
        }
        catch (ParseError& err)
        {
            throw err;
        }

    }

    void parseErrorMessage(TL1Message&)
    {
        string x;

        try
        {
            while (1)
            {
                LexerTL1::Token token = m_lexer->getToken();
                if (token.m_value == ";")
                    return ;
            }
        }
        catch (ParseError& err)
        {
            throw err;
        }
    }

    bool tokenIsSystemMessage(LexerTL1::Token& token)
    {
        // The TITAN Sometimes sends system messages without a "header"
        return (token.m_value == "S");
    }

    bool tokenIsErrMessage(LexerTL1::Token& token)
    {
        return (token.m_value == "ERR");
    }

    TL1Message* getTL1Message(void)
    {

        TL1Message* ret = new TL1Message;
        try
        {
            m_lexer->setMatch(ret->m_match);

            LexerTL1::Token token;
            token = m_lexer->getToken();
            if (token.m_type == LexerTL1::EndToken)
            {
                ret->m_type = TL1Message::End;
                return ret;
            }

            if (token.m_type == LexerTL1::FormatEffector)
            {
                ret->m_type = TL1Message::Ignore;
                return ret;
            }

            if (tokenIsAckCode(token))
            {
                m_lexer->putToken(token);
                parseAcknowledgement(*ret);
                ret->m_type = TL1Message::Acknowledgement;
                return ret;
            }

            if (tokenIsCmdCode(token))
            {
                m_lexer->putToken(token);
                parseTestCommand(*ret);
                ret->m_type = TL1Message::TestCommand;
                return ret;
            }

            if (tokenIsLogCode(token))
            {
                m_lexer->putToken(token);
                parseLogMessage();
                ret->m_type = TL1Message::Log;
                return ret;
            }

            if (tokenIsSystemMessage(token))
            {
                m_lexer->putToken(token);
                parseSystemMessage(*ret);
                ret->m_type = TL1Message::System;
                return ret;
            }

            if (tokenIsErrMessage(token))
            {
                m_lexer->putToken(token);
                parseErrorMessage(*ret);
                ret->m_type = TL1Message::Err;
                return ret;
            }

            if (token.m_type == LexerTL1::General)
            {
                m_lexer->putToken(token);
                parseResponseOrAutonomous(ret->m_type, *ret);
                return ret;
            }
            {
                // Throw away the token
                throw_parseError("Error:Message", "Undefined toktype");
            }
        }
        catch (ParseError& err)
        {
            ret->m_type = TL1Message::ParseError;
            return ret;
        }
        catch (...)
        {
            delete ret;
            throw;
        }
    }

    TL1Message* getTL1Autonomous(void)
    {

        TL1Message* ret = new TL1Message;
        try
        {
            m_lexer->setMatch(ret->m_match);

            LexerTL1::Token token;
            token = m_lexer->getToken();
            if (token.m_type == LexerTL1::EndToken)
            {
                ret->m_type = TL1Message::End;
                return ret;
            }

            if (token.m_type == LexerTL1::FormatEffector)
            {
                ret->m_type = TL1Message::Ignore;
                return ret;
            }

            if (token.m_type == LexerTL1::General)
            {
                m_lexer->putToken(token);
                parseAutonomous(ret->m_type, *ret);
                return ret;
            }
            {
                // Throw away the token
                throw_parseError("Error:Autonomous", "Undefined toktype");
            }
        }
        catch (ParseError& err)
        {
            ret->m_type = TL1Message::ParseError;
            return ret;
        }
        catch (...)
        {
            delete ret;
            throw;
        }
    }
    //////////////////////////////////////////////////////////////////////////////

    LexerTL1::Lexer* m_lexer;  // Lexer source for the parse
    CommandDB::Command* m_commandDesc; // TL1 Command description
    Map m_descriptionMap;   // Description map
    string m_specialPostfix;  // Special postfix
    TL1CommandParseBuffer *message;  // Parsed results

};

// Create a parser instance
Parser* createParser(LexerTL1::Lexer* source)
{
    return new ParserImplementation(source);
}

// throw a parser error
void throw_parseError(const string& reason, const string& code)
{
    throw ParseError(reason, code);
}

// Convert a string to an int
int stoi(const string& aid, int pos)
{
    bool isNegative = false;
    int ret = 0;
    const char* data = aid.data() + pos;
    const char* endPos = aid.data() + aid.size();

    // Search for digit

    for (; data < endPos; ++data)
    {
        if (isdigit(*data))
            break;
    }

    // If the character before the digit is a '-' then
    // this is a negative number
    if (( (aid.data() + pos) < data)
        && (*(data - 1) == '-'))
    {
        isNegative = true;
    }

    // Found digit (or end of string, read until no digits)

    for (; data < endPos; ++data)
    {
        register ch = *data;
        if (! isdigit(ch))
            break;
        ret = (ch - '0') + (ret * 10);
    }
    if (isNegative)
        ret *= -1;
    return ret;
}

// Convert an int to a string
string itos(int i, int leadingZeros)
{
    char array[20];
    if (! leadingZeros)
    {
        sprintf(array, "%d", i);
        return array;
    }
    sprintf(array, "%0*d", leadingZeros, i);
    return array;
}

// Determine if string needs encoding
bool needsStringEncode(const string& value)
{
    if (value.size() == 0)
        return true;
    const char* start = value.data();
    const char* end = start + value.size();
    for (; start < end; ++start)
    {
        if (*start == ' ' ||
            *start == '\\' ||
            *start == '"')
        {
            return true;
        }
    }
    return false;
}

// Encode a string
string encodeStringValue(const string& value)
{
    string ret;
    ret = "\""; // start of string
    const char* start = value.data();
    const char* end = start + value.size();
    for (; start < end; ++start)
    {
        if (*start == '"')
        {
            ret += (char) '\\';
        }
        ret += *start;
    }
    ret += (char) '"';
    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors for NameValue
///////////////////////////////////////////////////////////////////////////////////////////////
NameValue::NameValue()
        : m_position(0), m_block(0)
{
    // Intentionally Empty
}

NameValue::NameValue(const string& name, const string& value, const int position, const int block)
        : m_name(name), m_value(value), m_position(position), m_block(block)
{
    // Intentionally Empty
}

NameValue::NameValue(const string& name, const string& value)
        : m_name(name), m_value(value), m_position(0), m_block(0)
{
    // Intentionally Empty
}

NameValue::NameValue(const NameValue &nv)
        : m_name(nv.name()), m_value(nv.value()), m_position(nv.position()), m_block(nv.block())
{
    // Intentionally Empty
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Destructor for NameValue
///////////////////////////////////////////////////////////////////////////////////////////////
NameValue::~NameValue()
{
    // Intentionally Empty
}

///////////////////////////////////////////////////////////////////////////////////////////////
// get name
///////////////////////////////////////////////////////////////////////////////////////////////
string& NameValue::name(void) const
{
    return *(const_cast<string*>(&m_name));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// set name
///////////////////////////////////////////////////////////////////////////////////////////////
void NameValue::name(const string& name)
{
    m_name = name;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// get value
///////////////////////////////////////////////////////////////////////////////////////////////
string& NameValue::value(void) const
{
    return *(const_cast<string*>(&m_value));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// set value
///////////////////////////////////////////////////////////////////////////////////////////////
void NameValue::value(const string& value)
{
    m_value = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// get position
///////////////////////////////////////////////////////////////////////////////////////////////
int NameValue::position(void) const
{
    return m_position;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// set position
///////////////////////////////////////////////////////////////////////////////////////////////
void NameValue::position(const int position)
{
    m_position = position;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// get block
///////////////////////////////////////////////////////////////////////////////////////////////
int NameValue::block(void) const
{
    return m_block;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// set block
///////////////////////////////////////////////////////////////////////////////////////////////
void NameValue::block(const int block)
{
    m_block = block;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// get name value pair as string
///////////////////////////////////////////////////////////////////////////////////////////////
string NameValue::asString(void)
{
    string ret;
    ret = m_name + "=" + m_value;
    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////
NameValues::NameValues()
{
    // Intentionally empty
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////////////////
NameValues::~NameValues()
{
    // Intentionally empty
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Insert a name value pair
///////////////////////////////////////////////////////////////////////////////////////////////
void NameValues::insert(const string& namex, const string& valuex)
{
    push_back(NameValue(namex, valuex));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Insert a name value pair with position information
///////////////////////////////////////////////////////////////////////////////////////////////
void NameValues::insert(const string& namex, const string& valuex, const int position, const int block)
{
    push_back(NameValue(namex, valuex, position, block));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Find a value
///////////////////////////////////////////////////////////////////////////////////////////////
const string& NameValues::findValue(const string& namex) const
{
    for ( const_iterator it = begin(); it != end(); ++it )
    {
        if ( (*it).name() == namex )
            return (*it).value();
    }
    return m_blankString;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Find a value based on position
///////////////////////////////////////////////////////////////////////////////////////////////
const string& NameValues::findValue(const int position) const
{
    for ( const_iterator it = begin(); it != end(); ++it )
    {
        if ( (*it).position() == position )
            return (*it).value();
    }
    return m_blankString;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Replace a value
///////////////////////////////////////////////////////////////////////////////////////////////
bool NameValues::replaceValue(const string& name, const string& value)
{
    iterator it;
    for (it = begin(); it != end(); ++it)
    {
        if ( (*it).name() == name)
        {
            (*it).value(value);
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Remove a name value pair
///////////////////////////////////////////////////////////////////////////////////////////////
string NameValues::remove
    (const string& target)
{
    iterator i;
    for (i = begin(); i != end(); ++i)
    {
        if ((*i).name() == target)
        {
            string retValue = (*i).value();
            erase(i);
            return retValue;
        }
    }
    return string("");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Determine if a name value pair is present
///////////////////////////////////////////////////////////////////////////////////////////////
bool NameValues::present(const string& namex)
{
    for ( iterator it = begin(); it != end(); ++it )
    {
        if ( (*it).name() == namex )
            return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Return a name value pair as a string
///////////////////////////////////////////////////////////////////////////////////////////////
string NameValues::asString(void)
{
    string ret;

    for ( iterator it = begin(); it != end(); ++it )
    {
        if (it != begin())
        {
            ret += ",";
        }

        NameValue nv = (*it);
        ret += nv.asString();
    }
    return ret;
}

}

}
