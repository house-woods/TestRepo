
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

#ifndef _PROTOCOLPRODUCTDECISIONMAKER_H_
#define _PROTOCOLPRODUCTDECISIONMAKER_H_

#include "AbstractProductDecisionMaker.h"


namespace EMS_FW
{


class ProtocolProductDecisionMaker:public AbstractProductDecisionMaker
{
public:
    ProtocolProductDecisionMaker(){
    };
	
	virtual string determineMoi(const string& neId,const list<string>& aidList,const string& tlMsgName="");
	virtual string determineMoc(const string& neId,const list<string>& aidList,const string& tlMsgName="");
	virtual bool isAidSupported(const string& aid);
	virtual bool isTL1CmdSupported(const string& cmdType);
	virtual bool isAttributeAID(const string& attrName) {return true;}
	virtual bool isAttributeSupported(const string& attributeId, const list<string>& aidList) {return true;}


};

};

#endif

