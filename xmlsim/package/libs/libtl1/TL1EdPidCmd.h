/*******************************************************************************
* COPYRIGHT May 21 2004 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
* 
* Name: TL1 Edit Password Command
* Type: H
* Originator: ssteele1
* File: TL1EdPidCmd.h
* Description: 
* Implements the ED-PID to the TL1 device command.
* 
*******************************************************************************/

#ifndef _TL1EdPidCmd_H_
#define _TL1EdPidCmd_H_

#include "TL1BaseCmd.h"
#include <string>

namespace EMS_FW
{

class TL1Connection;

class TL1EdPidCmd : public TL1BaseCmd
{

public:

    TL1EdPidCmd(TL1Connection* pConn, const string& tid, const string& uid,
                const string& oldPasswd, const string& newPasswd,
                const string& neId, short encryptType=0);

    ~TL1EdPidCmd();

    virtual CmdResult execute();
    virtual CmdResult execute(TL1Connection *con);


private:

    string _tid;
    string _uid;
    string _oldPasswd;
    string _newPasswd;
    string _neId;
    short _encryptType;   // 0: N, 1: Y, 2: A, -1: Unknown
    TL1EdPidCmd();

};


};

#endif

