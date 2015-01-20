#ifndef __TL1MESSAGE_H__
#define __TL1MESSAGE_H__

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif
#include <vector>
#include <string>
#include <ostream>

struct DateTime
{
   int m_year;
   int m_mon;
   int m_day;

   int m_hour;
   int m_min;
   int m_sec;
};

struct TextBlock
{
    enum Type {None, Comment, CommentNoEnd, Quoted, Unquoted};

    Type   m_type;
    string m_value;

    TextBlock(void)
            : m_type(None), m_value(" ")
    {}
    TextBlock(Type type, const string& value)
		: m_type(type), m_value(value)
    {}
};

typedef vector<TextBlock> TextBlocks;

struct TL1Message
{
    TL1Message(void)
            : m_type(None), m_finished(true)
    {}
    enum Type {None,
               Log,
               Acknowledgement,
               Response,
               Autonomous,
               ParseError,
               Ignore,
               System,
               TestCommand,
               Err,
               Timeout,    // Internal Messages
               Rejected,   // Internal Messages
               End};
    Type   m_type;
    bool   m_finished;  // false if response ended in '>'
    string m_match;     // Text this message matched

    DateTime   m_dateTime;  // The date and time of the message
    
    string m_sid;   // System ID
    string m_code;  // Ack code for acknowledgement
                    // 'M' for Response
                    // "*C", "**", "* ", "A " for Autonomous
    string m_tag;   // Ctag for Ack and response
                    // Atag for autonomous

    // For Response
    string m_completeCode;

    // For Autonomous
    string m_outputCode1;
    string m_outputCode2;
    string m_outputCode3;

    // For Response and Autonomous
    TextBlocks  m_textBlocks;

    // For Rejected
    string m_reason;
};

ostream& operator<< (ostream& os, TL1Message::Type& type);
ostream& operator<< (ostream& os, TL1Message& msg);


#endif
