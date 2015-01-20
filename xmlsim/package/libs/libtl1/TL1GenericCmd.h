/***********************************************************************************************
* COPYRIGHT $Date:   June 25 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Generic Command
* Type: H
* Originator: $Author:   dtchalak  $
* File: $Workfile:   TL1GenericCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Generic TL1 command used to send the commands through the TL1 protocol.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/

#ifndef _TL1GenericCmd_H_
#define _TL1GenericCmd_H_

#include "TL1BaseCmd.h"


namespace EMS_FW
{


class TL1GenericCmd : public TL1BaseCmd
{


public:


    TL1GenericCmd();

    TL1GenericCmd(EMS_FW::BaseCmdInfo* info);

    ~TL1GenericCmd();

    virtual CmdResult execute();

    virtual CmdResult execute (TL1Connection *con);

protected:
    bool isNeActive();

};

};


#endif

