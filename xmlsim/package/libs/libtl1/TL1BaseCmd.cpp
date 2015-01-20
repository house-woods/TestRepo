/***********************************************************************************************
* COPYRIGHT 2003-2004 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 West Diehl Road, Naperville, IL 60563 U.S.A.
************************************************************************************************
*
* Name: TL1 Base Command
* Type: C++
* Originator: lzou
* Description:
* Base TL1 command implementing async interface.
*
* Revision History:
*   Rev 1.3     09/27/2004  aaali
*       Modified to work with the TL1MsgConverter change to a singleton and remove neId dependency
*
* Rev 1.2  07/10/2003 lzou
*  post the lock in finalize
* Rev 1.1  01/07/2003 lzou
*  Add destructor and finalize functions
*  Add timeout machanism
*  Assign CmdResult as one member varible of TL1BaseCmd and check its status after sending command
*
**********************************************************************************************/

#include "TL1BaseCmd.h"
#include "TL1Connection.h"
#include "BaseCmdInfo.h"
#include "sysErrLog.h"
#include "TL1ConnectionResponse.h"
#include "SocketHandler.h"
#include "TL1MsgConverter.h"
#include "MoAttributePo.h"
#include "EmsTypes.h"
#include "BaseException.h"
#include "tutil.h"
#include "Server_i.h"

extern const char* CtagServerPrefix;
extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;

osi_mutex TL1BaseCmd::_ctagsInUseMutex;
list<string> TL1BaseCmd::_ctagsInUse;

TL1BaseCmd::TL1BaseCmd(BaseCmdInfo* info, TL1Connection *con) : BaseCmd(info) , TL1EventReceiver()
{
    _inLock = false;
    initialize();
}

TL1BaseCmd::~TL1BaseCmd()
{
    finalize();
}

string TL1BaseCmd::getNeId()
{
    return (info_->getNeId());
}

bool TL1BaseCmd::isUniversalListener()
{
    return false;
}

void TL1BaseCmd::prepare (string &preCmd, string &postCmd, int &timeOut)
{}

bool TL1BaseCmd::initialize()
{
    _discardFlag = false;
    _waitOnIP = Server_i::instance()->getDefaultConfigValue("TL1WaitOnIP", "false");
    _result = CmdResult(false, "Command failed");
    return true;
}

bool TL1BaseCmd::finalize()
{
    _result.setStatus(false);
    _result.setResultInfo("Command aborted");
    _discardFlag = true;
    cleanUpResList();
    return true;
}

bool TL1BaseCmd::isCTAGInUse (const string &ctag)
{
    bool debugCtagGen = ( (Server_i::instance()->getDefaultConfigValue ("DebugCTAGGeneration", "false")) == "true");

    MessageLock lock (_ctagsInUseMutex)
    ;

    if (debugCtagGen)
    {
        string ctagsInUseList;
        for (list<string>::iterator iter = _ctagsInUse.begin();iter != _ctagsInUse.end();iter++)
            ctagsInUseList += (*iter) + " ";
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "isCTAGInUse", __LINE__, TRACE1,
                                 "Attempting to find ctag [%s] in following list of in-use ctags \n\n[%s]\n", ctag.c_str(), ctagsInUseList.c_str());
    }
    list<string>::iterator iter = find (_ctagsInUse.begin(), _ctagsInUse.end(), ctag);
    if (iter == _ctagsInUse.end())
    {
        if (debugCtagGen)
        {
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "isCTAGInUse", __LINE__, TRACE1,
                                     "ctag [%s] not found in the list; adding it", ctag.c_str());
        }

        _ctagsInUse.push_back (ctag);

        if (debugCtagGen)
        {
            string ctagsInUseList;
            for (list<string>::iterator iter = _ctagsInUse.begin();iter != _ctagsInUse.end();iter++)
                ctagsInUseList += (*iter) + " ";
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "isCTAGInUse", __LINE__, TRACE1,
                                     "The following is the new list of in-use ctags \n\n[%s]\n", ctagsInUseList.c_str());
        }
        return false;
    }

    if (debugCtagGen)
    {
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "isCTAGInUse", __LINE__, TRACE1,
                                 "ctag [%s] found in the list of in-use ctags ", ctag.c_str());
    }
    return true;
}


string TL1BaseCmd::generateCTAG (void)
{
    bool debugCtagGen = ( (Server_i::instance()->getDefaultConfigValue ("DebugCTAGGeneration", "false")) == "true");

    while (true) // the maximum try should equal to the highest 4-digit integer
    {
        if (debugCtagGen)
        {
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "generateCTAG", __LINE__, TRACE1,
                                     "Generating a unique CTAG");
        }
        const string& id = Tutil::getUniqueId();
        unsigned pos = id.size() - 4; // ATTENTION limited to 6 characters
        string ctag = id.substr(pos);

        if (debugCtagGen)
        {
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "generateCTAG", __LINE__, TRACE1,
                                     "Generated CTAG is [%s]; checking if it is in use", ctag.c_str());
        }

        if (!TL1BaseCmd::isCTAGInUse (ctag))
            return ctag;

        if (debugCtagGen)
        {
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "generateCTAG", __LINE__, TRACE1,
                                     "Generated CTAG [%s] is in use; need to re-generate", ctag.c_str());
        }
    }
}

void TL1BaseCmd::releaseCTAG (const string &ctag)
{

    bool debugCtagGen = ( (Server_i::instance()->getDefaultConfigValue ("DebugCTAGGeneration", "false")) == "true");

    MessageLock lock (_ctagsInUseMutex)
    ;

    if (debugCtagGen)
    {
        string ctagsInUseList;
        for (list<string>::iterator iter = _ctagsInUse.begin();iter != _ctagsInUse.end();iter++)
            ctagsInUseList += (*iter) + " ";
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "releaseCTAG", __LINE__, TRACE1,
                                 "Attempting to find ctag [%s] in following list of in-use ctags \n\n[%s]\n", ctag.c_str(), ctagsInUseList.c_str());
    }
    list<string>::iterator iter = find (_ctagsInUse.begin(), _ctagsInUse.end(), ctag);
    if (iter != _ctagsInUse.end())
    {
        if (debugCtagGen)
        {
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "releaseCTAG", __LINE__, TRACE1,
                                     "ctag [%s] found in the list; removing it", ctag.c_str());
        }
        _ctagsInUse.erase (iter);

        if (debugCtagGen)
        {
            string ctagsInUseList;
            for (list<string>::iterator iter = _ctagsInUse.begin();iter != _ctagsInUse.end();iter++)
                ctagsInUseList += (*iter) + " ";
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "releaseCTAG", __LINE__, TRACE1,
                                     "The following is the new list of in-use ctags \n\n[%s]\n", ctagsInUseList.c_str());
        }
    }
    if (debugCtagGen)
    {
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "releaseCTAG", __LINE__, TRACE1,
                                 "ctag [%s] not found in the list; removing it", ctag.c_str());
    }

}


bool TL1BaseCmd::hasMoreElements()
{
    if (_responseList.size() > 0)
        return true;
    return false; // default implementation
}

bool TL1BaseCmd::getNextElement(EMS_FW::BaseCmdInfo* info)
{
    BaseCmdInfo* resp = _responseList.front();
    info->reinitialize(resp);
    _responseList.pop_front();
    delete resp;
    //we found the next element
    return true;
}


bool TL1BaseCmd::getNextResponse()
{
    return true; // default implementation
}


void TL1BaseCmd::handleResponse(TL1Message* response, const string& rawMessage)
{
    if (!_discardFlag)
    {
        try
        {
            TL1MsgConverter::instance()->convertMessage (_responseList, response, _result, info_->getType(), info_->getNeId());
        }
        catch (BaseException &ex)
        {
            string message = ex.getDescription();
            _result.setStatus(false);
            _result.setResultInfo(message);
        }
        if (response->m_finished)
        {
            _keepWaiting = false;
            _lock.post(); // this is the last response unblock
        }
        else
        {
            _result.setMultiPartFlag(true);
            if (_waitOnIP == "true")
            {
                if (response->m_code == "IP")
                    _keepWaiting = true;
            }
        }
    }
    else
    {
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "handleEvent", __LINE__, MAJOR,
                                 "Response came in too late; Command [%s] has already timed out", _commandText.c_str());
    }
}

string TL1BaseCmd::getCommandText ()
{
    return _commandText;
}

string TL1BaseCmd::getCTAG()
{
    return _ctag;
}

void TL1BaseCmd::cleanUpResList()
{
    for (list<BaseCmdInfo*>::iterator p = _responseList.begin();p != _responseList.end();p++)
        delete *p;
    _responseList.clear();
}

CmdResult TL1BaseCmd::execute(TL1Connection *con)
{
    string preCmd, postCmd;
    int timeout;
    prepare (preCmd, postCmd, timeout);
    return sendCommand (con, preCmd, postCmd, timeout, 1);
}

CmdResult TL1BaseCmd::execute ()
{
    string preCmd, postCmd;
    int timeout;
    prepare (preCmd, postCmd, timeout);
    return sendCommand (preCmd, postCmd, timeout, 1);
}


CmdResult TL1BaseCmd::sendCommand(TL1Connection *con, const string& preCmd, const string& postCmd, int timeout, int trys)
{
    _lock.reset();
    _ctag = TL1BaseCmd::generateCTAG();
    try
    {
        _commandText = preCmd + _ctag + postCmd;
        _result.setResultInfo("The following command was aborted: " + _commandText);
        con->sendCommand (preCmd, postCmd, this, timeout, _ctag);
    }
    catch (BaseException &ex)
    {
        string message = ex.getDescription();
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, MAJOR, message.c_str());
        _result.setStatus(false);
        _result.setResultInfo (message.c_str());
        con->releaseReceiver (_ctag);
        TL1BaseCmd::releaseCTAG (_ctag);
        return _result;
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, MAJOR,
                                 "Failed to send command [%s]", _commandText.c_str());
        _result.setStatus(false);
        _result.setResultInfo("Failed to send the following command: " + _commandText);
        con->releaseReceiver (_ctag);
        TL1BaseCmd::releaseCTAG (_ctag);
        return _result;
    }
    _inLock = true;

    // this change allows a command to keep waiting in a loop
    // while IPs are being received
    do
    {
        _keepWaiting = false;
        if (_lock.wait(timeout))
        {
            _result.setStatus(false);
            _result.setResultInfo("The command [" + _commandText + "] timed out");
            _discardFlag = true;
        }
        else
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, TRACE1, "Received response for command [%s] within a timeout of [%d minutes %d seconds]", _commandText.c_str(), timeout / 60000, (timeout / 1000) % 60);
        if (_keepWaiting)
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, TRACE1,
                                     "The command will continue to wait because an IP was received:\n\n    %s\n", _commandText.c_str());
    }
    while (_keepWaiting == true);

    _inLock = false;
    con->releaseReceiver (_ctag);
    TL1BaseCmd::releaseCTAG (_ctag);
    return _result;
}

CmdResult TL1BaseCmd::sendCommand(const string& preCmd, const string& postCmd, int timeout, int trys)
{
    do
    {
        _lock.reset();
        _ctag = TL1BaseCmd::generateCTAG();
        string duplicateMessage ;
        try
        {
            string workaround;
            if (preCmd.compare(0, 12, "RTRV-NEIDMAP") == 0)
            {
                workaround = ":::SHELFNO=ALL";
            }
            if (preCmd.compare(0, 17, "RTRV-PMSCHED-1DAY") == 0)
            {
                workaround = "::,1-DAY";
                _commandText = preCmd.substr(0,12) + preCmd.substr(17) + _ctag + workaround + postCmd;
            }
            else 
                _commandText = preCmd + _ctag + workaround + postCmd;
            ErrorLogger::maskPassword(_commandText, duplicateMessage);
            _result.setResultInfo("The following command was aborted: " + duplicateMessage);
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, TRACE1, "Sending command [%s] with a timeout of [%d minutes %d seconds]", duplicateMessage.c_str(), timeout / 60000, (timeout / 1000) % 60);
            if (!TL1Session::forwardCommandToSession (this))
            {
                _result.setStatus (false) ;
                _result.setResultInfo ("Failed to send command " + _commandText + "; Session closed");
                TL1Session::unregisterSessionByCTAG (_ctag);
                TL1BaseCmd::releaseCTAG (_ctag);
                return _result;
            }
        }
        catch (BaseException &ex)
        {
            string message = ex.getDescription();
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, MAJOR, message.c_str());
            _result.setStatus(false);
            _result.setResultInfo (message.c_str());
            TL1Session::unregisterSessionByCTAG (_ctag);
            TL1BaseCmd::releaseCTAG (_ctag);
            return _result;
        }
        catch (...)
        {
            ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, MAJOR,
                                     "Failed to send command [%s]", _commandText.c_str());
            _result.setStatus(false);
            _result.setResultInfo("Failed to send the following command: " + _commandText);
            TL1Session::unregisterSessionByCTAG (_ctag);
            TL1BaseCmd::releaseCTAG (_ctag);
            return _result;
        }
        _inLock = true;

        // this change allows a command to keep waiting in a loop
        // while IPs are being received
        do
        {
            _keepWaiting = false;
            if (_lock.wait(timeout))
            {
                _result.setStatus(false);
                _result.setResultInfo("The command [" + duplicateMessage + "] timed out");
                _discardFlag = true;
            }
            else
                ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, TRACE1, "Received response for command [%s] within a timeout of [%d minutes %d seconds]", duplicateMessage.c_str(), timeout / 60000, (timeout / 1000) % 60);

            if (_keepWaiting)
                ErrorLogger::logVarError(sourceName, "TL1BaseCmd", "sendCommand", __LINE__, TRACE1,
                                         "The command will continue to wait because an IP was received:\n\n    %s\n", duplicateMessage.c_str());
        }
        while (_keepWaiting == true);

        _inLock = false;
        TL1Session::unregisterSessionByCTAG (_ctag);
        TL1BaseCmd::releaseCTAG (_ctag);

        // If the command failed due to a time out, it's worth retrying the command
        string resultInfo = _result.getResultInfo();
        if (_result.getStatus() == false &&
            (resultInfo.find("Timed Out:") != osi_npos || resultInfo.find("SRTO") != osi_npos ))
        {
            trys--;
        }
        else
        {
            return _result;
        }
    }
    while (trys > 0);
    return _result;
}

void TL1BaseCmd::postCmdLock()
{
    MessageLock lock (_lockMutex)
    ;
    _keepWaiting = false;
    if (_inLock)
    {
        _lock.post();
        _inLock = false;
    }
}
