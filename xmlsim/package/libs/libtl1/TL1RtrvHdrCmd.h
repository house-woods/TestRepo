/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Login Command
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1LoginCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Implements retrieve-hdr to the TL1 device command.
* 
*  Origintator     09/27/2004  aaali
*
* $Log:  $
**********************************************************************************************/


#ifndef _TL1RtrvHdrCmd_H_
#define _TL1RtrvHdrCmd_H_

#include "TL1BaseCmd.h"
#include <string>


namespace EMS_FW
{
class BaseCmdInfo;
class TL1Connection;

class TL1RtrvHdrCmd : public TL1BaseCmd
{
public:

    TL1RtrvHdrCmd(TL1Connection* con, const string& tid, const string& commandPrefix, const string& neId);
    ~TL1RtrvHdrCmd();
    virtual CmdResult execute();
    virtual CmdResult execute (TL1Connection *con);
    void handleResponse(TL1Message* response, const string& rawMessage = "");
    bool isUniversalListener ();

private:
    string _tid, _neId, _commandPrefix, _preCmd, _postCmd;
    TL1RtrvHdrCmd();
};
};



#endif

