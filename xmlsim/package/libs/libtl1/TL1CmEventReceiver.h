/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1CmEventReceiver
* Type: H
* Originator: $Author lzou:  
* File: $Workfile:   TL1CmEventReceiver.h  $
* Version: $Revision:   
* Description: 
* Class to represent the event handling for the CM TL1 notifications.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/
#ifndef _TL1CmEventReceiver_H_
#define _TL1CmEventReceiver_H_

#include <string>
#include "TL1EventReceiver.h"
#include "CmEventQueue.h"
#include "BaseCmdInfo.h"
#include "TL1Connection.h"

using namespace std;

namespace EMS_FW
{

class TL1CmEventReceiver : public TL1EventReceiver
{
public:

    static const string className;

    TL1CmEventReceiver(const string& neId);

    ~TL1CmEventReceiver();

    // Handle the notification.
    bool handleEvent(TL1Message* event, const string& rawMessage = "");

    virtual void handleCommand(string& request);

    virtual bool isAppInterested (const string& typeCode1, const string& typeCode2 = "");

    virtual void handleAppEvent (const string& testBlock, EMS_FW::BaseCmdInfo*& info, const string& typeCode1 = "",
                                 const string& typeCode2 = "", const string& rawMessage = "");

    virtual void blockedAction ();

    virtual void unblockedAction ();

protected:
    string _type;
    string _protocol;
    string _neType;

    CmEventQueue* equeue;

    vector< string > _reptDbChg;
    vector< string > _reptEvt;
    vector< string > _reptEvtEnv;
    vector< string > _reptEvtSess;

protected:
    TL1CmEventReceiver();

protected:
    virtual void initialize();
    virtual void _extractTL1DbChgEvent(const string& testBlock, EMS_FW::BaseCmdInfo*& info, const string& typeCode1 = "",
                                       const string& typeCode2 = "", const string& rawMessage = "");
    virtual void _extractTL1ReptEvent(const string& testBlock, EMS_FW::BaseCmdInfo*& info, const string& typeCode1 = "",
                                      const string& typeCode2 = "", const string& rawMessage = "");
    virtual void _extractSpeBlKAttrs(const string& block, EMS_FW::BaseCmdInfo*& info);
    virtual void _extractCommonBlKAttrs(const string& block, EMS_FW::BaseCmdInfo*& info,
                                        const string& cmd);
    virtual void _stripExtraCharsForStr (string& msg, const string& extraChars);

    void _extractDateTime ( TL1Message* event, long& evtTime, string& date, string& time);

    void _processDbChgHeader (const string& header, EMS_FW::BaseCmdInfo*& info);

    void tokenizeQuotedTextBlock(const string& text, vector<string>& tokens);
};

};

#endif
