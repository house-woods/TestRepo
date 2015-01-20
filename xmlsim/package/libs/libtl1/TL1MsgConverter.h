/***********************************************************************************************
* COPYRIGHT 2002 - 2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
*
* Name: TL1MsgConverter
* Type: h
* Originator: rugale
* File: TL1MsgConverter.h
* Version:
* Description:
* Convert the TL1 Message into name-value pairs.
*
* Revision History:
* Rev 1.1  09.27/04  aaali
*  Convert TL1MsgConverter to a singleton and remove dependency on the NE ID
*
*
**********************************************************************************************/
#ifndef _TL1MsgConverter_H_
#define _TL1MsgConverter_H_

#include <string>
#include <vector>
#include <map>
#include <list>
#include "BaseCmdInfo.h"
#include <ospace/thread.h>

class TL1Message;

namespace EMS_FW
{
class BaseCmdInfo;

class TL1MsgConverter
{

    typedef osi_sem_lock<osi_mutex_semaphore> AutoUnlock;

public:
    TL1MsgConverter();
    static TL1MsgConverter *instance();
    virtual void initialize (const string &responseFilePath);
    static void finalize ();
    virtual void convertMessage(list<EMS_FW::BaseCmdInfo*>& infoList, TL1Message* event, CmdResult& result, const string &cmdName, const string &neId = "");
    static void _stripExtraCharsForStr (string& msg, const string& extraChars);

protected:
    virtual void _processPDAttrs(const string& block, EMS_FW::BaseCmdInfo*& info, const string& commonblock, string& aid);
    virtual void _processSpecBlk(const string& block, EMS_FW::BaseCmdInfo*& info);
    virtual void _processStateBlk(const string& block, EMS_FW::BaseCmdInfo*& info);

    void tokenizeQuotedTextBlock(const string& text, vector<string>& tokens);

    map <string, vector<string>*> _grammar;
    static TL1MsgConverter * _instance;
    osi_mutex _initializationMutex;
    bool _initialized;
};
};

#endif

