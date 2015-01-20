/*******************************************************************************
*
* COPYRIGHT 2002 - 2008 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
*
* Name: TL1Utility
* Type: H
* Originator: lzou
* File:
* Version:
* Description:
*       TL1Utility class is a singleton class. Various generic
* operations will be implemented here which can be used by
* objects of more than one subsystem. There will be no
* private member defined in this class. Also there will be
* no dependencies between two public member functions.
*
* Revision History:
*    Rev 1.3  03/05/2003     lzou
*    add createTL1CmdInfoFromFilter for autodiscovery adapter
*    Rev 1.2     02/27/2003        lzou
*         add createTL1CmdInfo, rtrvEntityDetails, _constructTL1PreString
*         _attachComBlkonPostStr and _attachSpeBlkonPostStr
*    Rev 1.1  12/2002      lzou
*    add sendCommand to construct and send TL1 cmd
*    Rev 1.0  init version  taken from AmsUtility
*
******************************************************************************/

#ifndef TL1Utility_h
#define TL1Utility_h 1


#include "BaseCmd.h"
#include "GuiCmdInfo.h"
#include "BaseCmdFactory.h"
#include "sysErrLog.h"
#include "TL1Defines.h"
#include "MoAttributePo.h"
#include "Server_i.h"
#include "ConfigNames.h"
#include "tutil.h"
#include "TL1InfoList.h"
#include <map>
#include <vector>

using namespace std;
namespace EMS_FW
{

class TL1Utility : public Tutil
{
public:

    TL1Utility();

    ~TL1Utility();


    static void determineParentMoi (const string& moi, const string& moc, string& parentMoi, const string& neId);

    static string determineAid (const EMS_FW::GuiCmdInfo* info);


    // The method retrieves an attribute value from a CmdInfo
    // based on the attribute name.
    static bool getCmdInfoAttribute (const EMS_FW::BaseCmdInfo* info, MoAttributePo*& attr, const string& attrName);

    // This method determines Entity Type from AID
    static string determineEntityTypebyAid(const string& aid);

    // This method adds the MOC prefix in front of string passed in
    static void addPrefix(string& str, const string& moc);

    // This method strips the prefix off from the string.
    static void deletePrefix(string& str);

    static EMS_FW::CmdResult sendCommand (BaseCmdInfo* commandInfo, TL1InfoList& responseList);

    static BaseCmdInfo* createTL1CmdInfo (const string& neId, const string& cmd,
                                          const string& aid, const string& tid,
                                          const vector <string>& commonBlk, const map <string, string>& speBlk,
                                          const string& priState = "", const string& secState = "");

    static BaseCmdInfo* createTL1CmdInfo (const string& neId, const string& cmd,
                                          const string& aid, const string& tid,
                                          const vector <string>& commonBlk, const vector<pair <string, string> >& speBlk,
                                          const string& priState = "", const string& secState = "");

    static BaseCmdInfo* createTL1CmdInfo (const string& neId, const string& cmd,
                                          const string& aid, const string& tid,
                                          const vector <string>& commonBlk);

    // handle special case in OTS that requires name-value pair in common block
    static BaseCmdInfo* createTL1CmdInfo (const map <string, string>& commonBlk, const string& neId,
                                          const string& cmd, const string& aid, const string& tid = "");

    static BaseCmdInfo* createTL1CmdInfo (const string& neId, const string& cmd,
                                          const string& aid, const string& tid,
                                          const map <string, string>& speBlk, const string& priState = "", const string& secState = "");

    static BaseCmdInfo* createTL1CmdInfo (const string& neId, const string& cmd,
                                          const string& aid, const string& tid = "");


    // This method creates the cmdInfo for autodiscovery based one the filter string
    // Different NE type may need to manipulate the filter string to create cmdInfo
    static BaseCmdInfo* createTL1CmdInfoFromFilter (const string& neId, const string& filter);


    static void deleteInfoList (list<EMS_FW::BaseCmdInfo*>& responseList);

    static void loadVarNameVector (vector<string>& varNames, const char* name1, ...);


protected:

    bool _timeToLong(const char* timestr, long& ltime);

    bool _longToTime(const long& ltime, string& timestr);

    string _getParentMoi(const string& moi);

    virtual bool _extractMoi(const char* moi, const char* delim, string& result);


private:

    static string _constructTL1PreString(const string& cmd, const string& aid,
                                         const string& tid = "");

    static void _attachComBlkonPostStr (string& postString, const vector <string>& commonBlk);
    static void _attachSpeBlkonPostStr (string& postString, const map <string, string>& speBlk);
    static void _attachSpeBlkonPostStr (string& postString, const vector<pair <string, string> >& speBlk);

};

};

#endif

