/*******************************************************************************
* COPYRIGHT 2002 BY TELLABS OPERATIONS, INC
* ALL RIGHTS RESERVED
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
* 1415 W Diehl Rd., Naperville, IL 60563
********************************************************************************
*
* Name: TL1ProductDecisionMaker
* Type: H
* Originator: $Author lzou$
* File: $Workfile$
* Version: $Revision$
* Description:
*  Class implementing TL1 protocol product behavior.
*  It inherits from ProtocolProductDecisionMaker.
* It implements the protocol specific operation, other product specific operation
*  should be implemented in ProductDecisionMaker.
*  The Application should inherit from ProductDecisionMaker and this class both.
*
* Revision History:
*
* Rev 1.0   init version
* $Log$
*******************************************************************************/

#ifndef _TL1ProductDecisionMaker_H_
#define _TL1ProductDecisionMaker_H_

#include "ProtocolProductDecisionMaker.h"
#include "BaseCmdInfo.h"
#include "TL1Defines.h"
#include <vector>


namespace EMS_FW
{


class TL1ProductDecisionMaker : public ProtocolProductDecisionMaker
{

public:


    TL1ProductDecisionMaker()
    {}

    virtual AgentCmdInfo* onFilterDiscovery (const string &neId, const string& filter);

    virtual string determineEventType(const string& cmdName);

    virtual string determineMoi(const string& neId, const list<string>& aidList, const string& tlMsgName = "");

    virtual string determineMoc(const string& neId, const list<string>& aidList, const string& tlMsgName = "");

    virtual bool isAidSupported(const string& aid);

    virtual bool isTL1CmdSupported(const string& cmdType);

    virtual bool isAttributeAID(const string& attrName);

    virtual bool getParentMoiMoc(const string& moi, const string& neId, const string& moc, string& parentMoi, string& parentMoc);

    virtual bool doNotifyDb(const string& neId, const string& moc = "");

    virtual void createDbObject(EMS_FW::BaseCmdInfo* info);

    virtual void deleteDbObject(EMS_FW::BaseCmdInfo* info);

    virtual void updateDbObject(EMS_FW::BaseCmdInfo* info);

    virtual void resetNEInfoBeforeDiscovery(const string& neId);

    // This method returns the common block syntax in string vector format based on its cmd.
    virtual void getCommonBlockSyntax(const string& cmd, const string& block, vector<string>& varNames);

    virtual string getReptEventSyntax (const string& neId, const string& code2, const string& code3 = "");

    const string& getClassName();

    virtual string getTL1FilterFileName (const string& neId);

    //determine table name from REF_ENTITY table based on aid/entity type
    virtual string determineTableName(const string& aid);
    virtual bool getTableandIdColName(const string& aid, const string& TL1cmd, string& tableName, string& idColName);

};

};

#endif


