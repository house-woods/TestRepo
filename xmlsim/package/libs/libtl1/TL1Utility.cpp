/*******************************************************************************
*
* COPYRIGHT 2002-2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
*
* Name: TL1Utility
* Type: cpp
* Originator: $ Author lzou$
* File: $Workfile$
* Version: $Revision$
* Description:
*       TL1Utility class is a singleton class. Various generic
* operations will be implemented here which can be used by
* objects of more than one subsystem. There will be no
* private member defined in this class. Also there will be
* no dependencies between two public member functions.
*
* Rev 1.3              03/05/2003                 lzou
*               add createTL1CmdInfoFromFilter for autodiscovery adapter
*     Rev 1.2     02/27/2003        lzou
*        add createTL1CmdInfo, rtrvEntityDetails, _constructTL1PreString
*        attachComBlkonPostStr and attachSpeBlkonPostStr
*    Rev 1.1  12/2002      lzou
*    add sendCommand to construct and send TL1 cmd
*    Rev 1.0  init version  taken from AmsUtility
*
* Revision History:
*
******************************************************************************/
#include "TL1Utility.h"
#include "Server_i.h"
#include "CmNameResolver.h"
#include "StringParser.h"
#include "TL1Defines.h"
#include "NetworkElement.h"
#include "TL1GenericCmd.h"
#include "TL1CmdFactory.h"
#include "TL1CmdInfo.h"
#include "TL1InfoList.h"
#include <ospace/helper.h>

using namespace ELogger;
using namespace EMS_FW;

extern const char* sourceName;


// default constructor
TL1Utility::TL1Utility()
{}


// default destructor
TL1Utility::~TL1Utility()
{}


//******************************************************************************
//The method determines the Parent PMolite's Moi.
//Note: Application needs to overwrite this method

void TL1Utility::determineParentMoi (const string& moi,
                                     const string& moc,
                                     string& parentMoi,
                                     const string& neId)
{
    ErrorLogger::logVarError(sourceName, "TL1Utility", "determineParentMoi", __LINE__,
                             TRACE6, "Method Begin for MOI = %s", moi.c_str());


    ErrorLogger::logVarError(sourceName, "TL1Utility", "determineParentMoi", __LINE__,
                             TRACE6, "Method End, Parent MOI = %s ", parentMoi.c_str());
    return ;

}



// This methods will construct the AID based on the naming
// conversion and MOC specified in GuiCmdInfo.
string TL1Utility::determineAid (const EMS_FW::GuiCmdInfo* info)
{
    return NULL;

}



//******************************************************************************
// The method retrieves an attribute value from a CmdInfo
// based on the attribute name.TL1EQUIPMENTHOLDER
bool TL1Utility::getCmdInfoAttribute (const BaseCmdInfo* info, MoAttributePo*& attr, const string& attrName)
{
    ErrorLogger::logError(sourceName, "TL1Utility", "getCmdInfoAttribute",
                          TRACE6, "Method Begin");

    const list<MoAttributePo*>& attrList = info->getAttributes();
    list<MoAttributePo*>::const_iterator iter;

    for ( iter = attrList.begin(); iter != attrList.end(); iter++)
    {
        string resName;
        const string& name = (*iter)->getName();
        EMS_FW::Tutil::extractAttribute(name, resName);
        if (resName == attrName)
        {
            attr = new MoAttributePo(attrName, (*iter)->getValue()->clone());
            ErrorLogger::logVarError(sourceName, "TL1Utility", "getCmdInfoAttribute", __LINE__,
                                     TRACE4, "Found attr %s", attrName.c_str());
            return true;
        }
    }
    ErrorLogger::logVarError(sourceName, "TL1Utility", "getCmdInfoAttribute", __LINE__,
                             TRACE6, "Method finished, %s not found", attrName.c_str());
    return false;
}


//******************************************************************************
//The method determines Entity Type from AID
string TL1Utility::determineEntityTypebyAid(const string& aid)
{
    ErrorLogger::logVarError(sourceName, "TL1Utility", "determineEntityTypebyAid",
                             TRACE6, "Method Begin for %s", aid.c_str());

    int pos = aid.find(TL1DEF::DELIMITER_HYPHEN);
    string entityType = aid.substr(0, pos);       // Get specific type from front of AID

    ErrorLogger::logVarError(sourceName, "TL1Utility", "determineEntityTypebyAid",
                             TRACE6, "Method End, type = %s", entityType.c_str());
    return entityType;
}


//******************************************************************************
// The method adds the MOC prefix in front of string passed in
void TL1Utility::addPrefix(string& str, const string& moc)
{
    ErrorLogger::logError(sourceName, "TL1Utility", "addPrefix", TRACE6, "Method Begin");
    string prefix = moc + TL1DEF::NAME_DELIMITER;
    str.insert(0, prefix);
    ErrorLogger::logError(sourceName, "TL1Utility", "addPrefix", TRACE6, "Method End");

}

//******************************************************************************
// The method strips the prefix off an attribute name string.
void TL1Utility::deletePrefix(string& str)
{
    ErrorLogger::logError(sourceName, "TL1Utility", "deletePrefix", TRACE6, "Method Begin");
    int pos = str.find(TL1DEF::NAME_DELIMITER);
    if (pos != string::npos)
    {
        int len = str.length();
        str = str.substr(pos + 1, (len - pos));
    }
    ErrorLogger::logError(sourceName, "TL1Utility", "deletePrefix", TRACE6, "Method End");
}


bool TL1Utility::_timeToLong(const char* timestr, long& ltime)
{
    return true;
}


bool TL1Utility::_longToTime(const long& ltime, string& timestr)
{
    return true;
}

//Not used
string TL1Utility::_getParentMoi(const string& moi)
{
    return NULL;
}

bool TL1Utility::_extractMoi(const char* moi, const char* delim, string& result)
{
    result = moi;
    return true;
}

//******************************************************************************
EMS_FW::CmdResult TL1Utility::sendCommand (BaseCmdInfo* commandInfo,
        TL1InfoList& responseList)
{
    ErrorLogger::logError(sourceName, "TL1Utility", "sendCommand", TRACE6, "Method Begin");

    auto_ptr<TL1GenericCmd> TL1cmd ((TL1GenericCmd*)BaseCmdFactory::instance()->createCommand(commandInfo, TL1CmdFactory::TL1));

    // Call execute on the new BaseCmd object capturing the CmdResult.
    const CmdResult& result = TL1cmd->execute();

    if (!result.getStatus())
    {
        ErrorLogger::logVarError(sourceName, "TL1Utility", "sendCommand", __LINE__,
                                 MAJOR, "Command Failed. Reason: %s", result.getResultInfo().c_str());
        string errdesc;
        try
        {
            CmNameResolver* nameResolver = CmNameResolver::instance();
            errdesc = nameResolver->resolveName(result.getStrErrCode());
        }
        catch (...)
        {
            errdesc = result.getResultInfo().c_str();
        }
        throw BaseException(errdesc.c_str(), "TL1Utility", "sendCommand");
    }
    else
    {
        const string& errCode = result.getStrErrCode();
        //"PRTL" means that partially completed the command. We will define it in FW later
        if (errCode == TL1DEF::Q3TL1PRTLMSG)
        {
            ErrorLogger::logVarError(sourceName, "TL1Utility", "sendCommand", __LINE__,
                                     TRACE1, "Command Partially Failed. Reason: %s",
                                     result.getResultInfo().c_str());
            throw BaseException(result.getResultInfo().c_str(), "TL1Utility", "sendCommand");
        }
    }

    EMS_FW::BaseCmdInfo* pResponse;
    while (TL1cmd->hasMoreElements())
    {
        pResponse = new TL1CmdInfo();
        if (TL1cmd->getNextElement(pResponse))
        {
            responseList.push_back(pResponse);
        }
    }

    ErrorLogger::logVarError(sourceName, "TL1Utility", "sendCommand", TRACE6, "Method End");
    return result;
}

//******************************************************************************
//  This older version re-orders the parameters in speBlk because its a map.
//  It would be nice to get rid of this version eventually.
//
BaseCmdInfo* TL1Utility::createTL1CmdInfo (const string& neId, const string& cmd,
        const string& aid, const string& tid,
        const vector <string>& commonBlk,
        const map <string, string>& speBlk,
        const string& priState, const string& secState)
{
    string cmdTid = tid;
    if (cmdTid.empty())
    {
        cmdTid = Server_i::instance()->getDefaultConfigValue (ConfigNames::TID, neId, "");
    }
    const string& preString = _constructTL1PreString (cmd, aid, cmdTid);
    string postString;
    _attachComBlkonPostStr (postString, commonBlk);
    _attachSpeBlkonPostStr (postString, speBlk);
    if (!priState.empty())
    {
        postString.append(":");
        postString.append(priState);
    }
    if (!secState.empty())
    {
        postString.append(",");
        postString.append(secState);
    }

    postString.append(";");
    ErrorLogger::logVarError(sourceName, "TL1Utility", "createTL1CmdInfo", __LINE__, TRACE10,
                             "postString is %s", postString.c_str());

    BaseCmdInfo* cmdInfo = BaseCmdFactory::instance()->createInfo(neId, postString, preString,
                           cmd, TL1CmdFactory::TL1);
    return cmdInfo;
}

//******************************************************************************
//  This newer version is meant to preserve the order of parameters in speBlk
//
BaseCmdInfo* TL1Utility::createTL1CmdInfo (const string& neId, const string& cmd,
        const string& aid, const string& tid,
        const vector <string>& commonBlk,
        const vector <pair<string, string> >& speBlk,
        const string& priState, const string& secState)
{
    string cmdTid = tid;
    if (cmdTid.empty())
    {
        cmdTid = Server_i::instance()->getDefaultConfigValue (ConfigNames::TID, neId, "");
    }
    const string& preString = _constructTL1PreString (cmd, aid, cmdTid);
    string postString;
    _attachComBlkonPostStr (postString, commonBlk);
    _attachSpeBlkonPostStr (postString, speBlk);
    if (!priState.empty())
    {
        postString.append(":");
        postString.append(priState);
    }
    if (!secState.empty())
    {
        postString.append(",");
        postString.append(secState);
    }

    postString.append(";");
    ErrorLogger::logVarError(sourceName, "TL1Utility", "createTL1CmdInfo", __LINE__, TRACE10,
                             "postString is %s", postString.c_str());

    BaseCmdInfo* cmdInfo = BaseCmdFactory::instance()->createInfo(neId, postString, preString,
                           cmd, TL1CmdFactory::TL1);
    return cmdInfo;
}

//******************************************************************************
BaseCmdInfo* TL1Utility::createTL1CmdInfo (const string& neId, const string& cmd,
        const string& aid, const string& tid,
        const vector <string>& commonBlk)
{
    string cmdTid = tid;
    if (cmdTid.empty())
    {
        cmdTid = Server_i::instance()->getDefaultConfigValue (ConfigNames::TID, neId, "");
        //        cmdTid = ne->getDefaultConfigValue(ConfigNames::TID, "");
    }
    const string& preString = _constructTL1PreString (cmd, aid, cmdTid);
    string postString;
    _attachComBlkonPostStr (postString, commonBlk);

    postString.append(";");
    ErrorLogger::logVarError(sourceName, "TL1Utility", "createTL1CmdInfo", __LINE__, TRACE10,
                             "postString is %s", postString.c_str());

    BaseCmdInfo* cmdInfo = BaseCmdFactory::instance()->createInfo(neId, postString, preString,
                           cmd, TL1CmdFactory::TL1);
    return cmdInfo;
}

//******************************************************************************
// Handle the special case that requires the name-value pair in common block
BaseCmdInfo* TL1Utility::createTL1CmdInfo (const map <string, string>& commonBlk,
        const string& neId, const string& cmd,
        const string& aid, const string& tid)
{
    string cmdTid = tid;
    if (cmdTid.empty())
    {
        cmdTid = Server_i::instance()->getDefaultConfigValue (ConfigNames::TID, neId, "");
        //        cmdTid = ne->getDefaultConfigValue(ConfigNames::TID, "");
    }
    const string& preString = _constructTL1PreString (cmd, aid, cmdTid);
    string postString("::");
    int size = commonBlk.size();
    map <string, string>::const_iterator iter;
    for (iter = commonBlk.begin();iter != commonBlk.end();)
    {
        postString = postString + (*iter).first + "=";
        postString = postString + (*iter).second;
        iter++;
        if (iter != commonBlk.end())
            postString = postString + ",";
    }

    postString = postString + ";";
    ErrorLogger::logVarError(sourceName, "TL1Utility", "createTL1CmdInfo", __LINE__, TRACE10,
                             "postString is %s", postString.c_str());

    EMS_FW::BaseCmdInfo* cmdInfo = BaseCmdFactory::instance()->createInfo(neId, postString, preString,
                                   cmd, TL1CmdFactory::TL1);
    return cmdInfo;
}

//******************************************************************************
BaseCmdInfo* TL1Utility::createTL1CmdInfo (const string& neId, const string& cmd,
        const string& aid, const string& tid,
        const map <string, string>& speBlk,
        const string& priState, const string& secState)
{
    vector <string> commonBlk;
    return createTL1CmdInfo (neId, cmd, aid, tid, commonBlk, speBlk, priState, secState);
}


//******************************************************************************
BaseCmdInfo* TL1Utility::createTL1CmdInfo (const string& neId, const string& cmd,
        const string& aid, const string& tid)
{
    string cmdTid = tid;
    if (cmdTid.empty())
    {
        cmdTid = Server_i::instance()->getDefaultConfigValue (ConfigNames::TID, neId, "");
        //        cmdTid = ne->getDefaultConfigValue(ConfigNames::TID, "");
    }
    string preString = _constructTL1PreString (cmd, aid, cmdTid);
    string postString(";");
    BaseCmdInfo* cmdInfo = BaseCmdFactory::instance()->createInfo(neId, postString, preString,
                           cmd, TL1CmdFactory::TL1);
    return cmdInfo;
}


//******************************************************************************
BaseCmdInfo* TL1Utility::createTL1CmdInfoFromFilter (const string& neId, const string& filter)
{
    ErrorLogger::logError(sourceName, getClassName().c_str(), "createTL1CmdInfoFromFilter",
                          TRACE6, "Method begins ...... ");
    string cmd;
    string tid;
    string aid;
    size_t pos = filter.find_first_of(':');
    if (pos != string::npos)
    {
        osi_tokenizer tokenizer( ":", true, "\\", "\n", "", false );
        vector < string OS_DEFAULT_ALLOCATOR( string ) > tokens;
        tokens = tokenizer.tokenize(filter);
        ErrorLogger::logVarError(sourceName, getClassName().c_str(), "createTL1CmdInfoFromFilter", __LINE__, TRACE7, "%s has %d tokens:", filter.c_str(), tokens.size());
        cmd = tokens[0];
        tid = tokens[1];
        aid = tokens[2];
    }
    else
        cmd = filter;

    BaseCmdInfo* cmdInfo = TL1Utility::createTL1CmdInfo(neId, cmd, aid, tid);

    ErrorLogger::logError(sourceName, getClassName().c_str(), "createTL1CmdInfoFromFilter",
                          TRACE6, "Method ends ...... ");
    return cmdInfo;
}

//******************************************************************************
string TL1Utility::_constructTL1PreString(const string& cmd, const string& aid, const string& tid)
{
    string result = cmd + ":" + tid + ":" + aid + ":";
    return result;
}

//******************************************************************************
void TL1Utility::_attachComBlkonPostStr (string& postString, const vector <string>& commonBlk)
{
    if (postString.empty())
    {
        postString = "::";
    }
    int size = commonBlk.size();
    for (int i = 0; i < size; i++)
    {
        postString.append( commonBlk[i] );
        if (i != size - 1)
            postString.append(",");
    }
    ErrorLogger::logVarError(sourceName, "TL1Utility", "_attachComBlkonPostStr", TRACE10,
                             "postString is %s", postString.c_str());
}

//******************************************************************************
void TL1Utility::_attachSpeBlkonPostStr (string& postString, const map <string, string>& speBlk)
{
    if (postString.empty())
    {
        postString = ":::";
    }
    else
    {
        postString.append(":");
    }
    map <string, string>::const_iterator iter;
    for (iter = speBlk.begin(); iter != speBlk.end();)
    {
        postString.append((*iter).first);
        postString.append("=");
        postString.append((*iter).second);
        iter++;
        if (iter != speBlk.end())
            postString = postString + ",";
    }
    ErrorLogger::logVarError(sourceName, "TL1Utility", "_attachComBlkonPostStr", TRACE10,
                             "postString is %s", postString.c_str());
}

//******************************************************************************
void TL1Utility::_attachSpeBlkonPostStr (string& postString, const vector < pair<string, string> >& speBlk)
{
    if (postString.empty())
    {
        postString = ":::";
    }
    else
    {
        postString.append(":");
    }
    vector< pair<string, string> >::const_iterator iter;
    for (iter = speBlk.begin(); iter != speBlk.end(); ++iter)
    {
        if (iter != speBlk.begin())
            postString.append(",");
        postString.append((*iter).first);
        postString.append("=");
        postString.append((*iter).second);
    }
    ErrorLogger::logVarError(sourceName, "TL1Utility", "_attachComBlkonPostStr", TRACE10,
                             "postString is %s", postString.c_str());
}


//******************************************************************************
void TL1Utility::deleteInfoList (list<EMS_FW::BaseCmdInfo*>& responseList)
{
    if (responseList.size() == 0)
        return ;
    list<EMS_FW::BaseCmdInfo*>::iterator p;
    for (p = responseList.begin();p != responseList.end();p++)
    {
        if (*p)
        {
            delete (*p);
        }
    }
    responseList.clear();
}

//******************************************************************************
void TL1Utility::loadVarNameVector (vector<string>& varNames, const char* name1, ...)
{
    varNames.clear();
    const char* pName = 0;

    va_list ap;
    va_start(ap, name1);
    for (pName = name1; pName != 0; pName = va_arg(ap, const char*))
    {
        varNames.push_back(pName);
    }
    va_end(ap);
}
