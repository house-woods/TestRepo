/***********************************************************************************************
* COPYRIGHT $Date:   June 16 2002 14:47:34  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1 Logout Command
* Type: H
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1LogoutCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Implements the logout to the TL1 device command.
* 
* Revision History:
*
* $Log:  $
**********************************************************************************************/


#ifndef _TL1LogoutCmd_H_
#define _TL1LogoutCmd_H_

#include "TL1BaseCmd.h"
#include <string>


namespace EMS_FW
{


class TL1Connection;

class TL1LogoutCmd : public TL1BaseCmd
{

public:

    TL1LogoutCmd(TL1Connection* con, const string& tid, const string& uid, const string& neId);

    ~TL1LogoutCmd();

protected:
    virtual void prepare (string &preCmd, string &postCmd, int &timeOut);

private:


    string _tid;
    string _uid;
    string _neId;


    TL1LogoutCmd();

};



};



#endif

