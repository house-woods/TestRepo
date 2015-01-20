/***********************************************************************************************
* COPYRIGHT 2002 - 2004 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name: TL1 Login Command
* Type: C++
* Originator: $Author:   lzou  $
* File: $Workfile:   TL1LoginCmd.h  $
* Version: $Revision:   1.5  $
* Description: 
* Implements the login to the TL1 device command.
* 
* Revision History:
*   Rev 1.3     09/27/2004  aaali
*       Modified to work with the TL1MsgConverter change to a singleton
* Rev 1.2  02/07/2003  lzou
*  Add initialize function to catch the Exception 
* Rev 1.1  01/10/2003  lzou
*  Add Time Out and response check for login 
*
**********************************************************************************************/

#include "TL1LoginCmd.h"
#include "sysErrLog.h"
#include "TL1Connection.h"
#include "CmNameResolver.h"
#include "ConfigNames.h"
#include "Server_i.h"
#include "TL1CommandConfigurator.h"
#include "tutil.h"
#include "Cryptor.h"

extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;


TL1LoginCmd::TL1LoginCmd(TL1Connection* con, const string& tid, const string& uid, const string& passwd, const string& neId, short encryptionType)
{
    _tid = tid;
    _uid = uid;
    _passwd = passwd;
    _neId = neId;
    _encryptType = encryptionType;
    _discardFlag = false;
    _result = CmdResult(false, "This is the default result structure.");

    // initialize info
    string preCmd = "ACT-USER:" + tid + ":" + uid + ":";
    string postCmd = "::*********,*;";
    BaseCmdInfo* cmdInfo = new BaseCmdInfo(postCmd, preCmd, "ACT-USER");
    cmdInfo->setNeId(neId);
    info_ = (auto_ptr<EMS_FW::BaseCmdInfo>&) cmdInfo;
}

TL1LoginCmd::~TL1LoginCmd()
{}

void TL1LoginCmd::prepare (string &preCmd, string &postCmd, int &timeOut)
{
    ErrorLogger::logError(sourceName, "TL1LoginCmd", "execute", __LINE__, TRACE11, "Method start.");
    preCmd = "ACT-USER:";
    preCmd += _tid;
    preCmd += ":";
    preCmd += _uid;
    preCmd += ":";
    postCmd = "::";
    
    if (_encryptType >0 || _passwd.length() > 30)
    {
       if (_encryptType > 1)
       {
          postCmd += _passwd;
          postCmd += ",A";	 
        }
       else if (_encryptType == 0)
       {//decrypt passwd
       		string rawPassword;
     		string hashedPwd = _passwd;
     		Tutil::toUpperCase(hashedPwd);
     		rawPassword = Tutil::decryptRijndaelEx(hashedPwd);
		Tutil::getReadablePW(rawPassword);     		
                _passwd = rawPassword.c_str();                
		 postCmd += _passwd;  //put the decrypt pwd  
       }       
       else    // could be 1, could be length>30, but _encryptType <0
       {
          postCmd += _passwd;
          postCmd += ",Y";
	}
    }
    postCmd += ";";

    timeOut = TL1CommandConfigurator::instance()->getMSECTimeout ("ACT-USER");

    ErrorLogger::logError(sourceName, "TL1LoginCmd", "prepare", __LINE__, TRACE11, "Method finished.");
}


CmdResult TL1LoginCmd::execute(TL1Connection *con)
{
    string preCmd, postCmd;
    int timeout;

    // if password is AES and NE is 6.0 
    // OR password is MD5 and NE is below 6.0
    prepare (preCmd, postCmd, timeout);

    CmdResult& result = sendCommand (con, preCmd, postCmd, timeout, 1);

    if (!result.getStatus())    // If command failed,
    {
       preCmd.clear();
       postCmd.clear();
       cleanUpResList();
       prepareWithAES(preCmd, postCmd, timeout);
       return sendCommand (con, preCmd, postCmd, timeout, 1);
    }
    return result;
}

CmdResult TL1LoginCmd::execute ()
{
    string preCmd, postCmd;
    int timeout;
    prepare (preCmd, postCmd, timeout);
    //return sendCommand (con, preCmd, postCmd, timeout, 1);

    CmdResult& result = sendCommand (preCmd, postCmd, timeout, 1);
    if (!result.getStatus())    // If command failed,
    {
       preCmd.clear();
       postCmd.clear();
       cleanUpResList();
       prepareWithAES(preCmd, postCmd, timeout);
       return sendCommand (preCmd, postCmd, timeout, 1);
    }
    return result;
}

// prepare login with MD5 password, but original password is AES. 
void TL1LoginCmd::prepareWithAES (string &preCmd, string &postCmd, int &timeOut)    
{
    ErrorLogger::logError(sourceName, "TL1LoginCmd", "prepareWithAES", __LINE__, TRACE11, "Method start.");
    preCmd = "ACT-USER:";
    preCmd += _tid;
    preCmd += ":";
    preCmd += _uid;
    preCmd += ":";
    postCmd = "::";

    if (_encryptType>0 || _passwd.length() > 30)
    {
       if ((_encryptType >1) || (_encryptType <0)) // either we know this is AES password, or it is unknown
       {
          string hashCurrentPwd;
          Tutil::getHashedPW(_passwd, hashCurrentPwd);
          postCmd += hashCurrentPwd;
          postCmd += ",Y";
       }
       else // This is not right. Password is MD5, should not come here.
       {
            ErrorLogger::logVarError(sourceName, "TL1LoginCmd", "prepareWithAES", __LINE__, MAJOR, "Login Command %s has incorrect password encryption type %d.", preCmd.c_str(), _encryptType);
            postCmd += _passwd;
            postCmd+=",Y";
       }
    }
    else
       postCmd += _passwd;
    postCmd += ";";

    timeOut = TL1CommandConfigurator::instance()->getMSECTimeout ("ACT-USER");

    ErrorLogger::logError(sourceName, "TL1LoginCmd", "prepare", __LINE__, TRACE11, "Method finished.");

}


