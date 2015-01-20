
/***********************************************************************************************
* COPYRIGHT $Date$ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: ProductDecisionMaker
* Type: H 
* Originator: $Author$
* File: $Workfile$
* Version: $Revision$
* Description: 
* Base class implementing product behavior.
* 
* Revision History:
*
* $Log$
**********************************************************************************************/

#ifndef _AbstractProductDecisionMaker_H_
#define _AbstractProductDecisionMaker_H_

#include <string>
#include <list>
#include "MoAttributePo.h"




namespace EMS_FW
{

class AgentCmd;
class BaseCmdInfo;
class AgentCmdInfo;


class AbstractProductDecisionMaker
{
public:
    AbstractProductDecisionMaker()
    {}
    ;

    static AbstractProductDecisionMaker* getProductDecisionMaker()
    {
        return _maker;
    }
    static void registerProductDecisionMaker(AbstractProductDecisionMaker* maker)
    {
        _maker = maker;
    }
    virtual string determineMoi(const string& neId, const list<string>& aidList, const string& tlMsgName = "");
    virtual string determineMoc(const string& neId, const list<string>& aidList, const string& tlMsgName = "");
    virtual string determineAid (BaseCmdInfo *info, const string &tlMsgName);

    virtual bool isAidSupported(const string& aid)
    {
        return true;
    }
    virtual bool isTL1CmdSupported(const string& cmdType)
    {
        return true;
    }
    virtual bool isAttributeAID(const string& attrName)
    {
        return true;
    }
    virtual bool isAttributeSupported(const string& attributeId, const list<string>& aidList)
    {
        return true;
    }

private:

    static AbstractProductDecisionMaker* _maker;

};

};

#endif

