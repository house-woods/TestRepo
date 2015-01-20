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
* Implements the login to the TL1 device command.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#ifndef _TL1LoginCmd_H_
#define _TL1LoginCmd_H_

#include "TL1BaseCmd.h"
#include <string>


namespace EMS_FW
{


class BaseCmdInfo;
class TL1Connection;

class TL1LoginCmd : public TL1BaseCmd
{

public:

    TL1LoginCmd(TL1Connection* con, const string& tid, const string& uid, const string& passwd,
                const string& neId, short encryptionType = 0);

    ~TL1LoginCmd();

    virtual CmdResult execute (TL1Connection *con);
    virtual CmdResult execute();

protected:

    virtual void prepare (string &preCmd, string &postCmd, int &timeOut);
    virtual void prepareWithAES (string &preCmd, string &postCmd, int &timeOut);


private:

    string _tid;
    string _uid;
    string _passwd;
    string _neId;
    short _encryptType;   // 0: N, 1: Y, 2: A, -1: Unknown
    TL1LoginCmd();

};
};

#endif

