#include "AbstractProductDecisionMaker.h"


EMS_FW::AbstractProductDecisionMaker* EMS_FW::AbstractProductDecisionMaker::_maker = NULL;
using namespace EMS_FW;


//******************************************************************************
string AbstractProductDecisionMaker::determineMoi(const string& neId,
        const list<string>& aidList,
        const string& tlMsgName)
{
    return "";
}

//******************************************************************************
string AbstractProductDecisionMaker::determineMoc(const string& neId,
        const list<string>& aidList,
        const string& tlMsgName)
{
    return "";
}

string AbstractProductDecisionMaker::determineAid (BaseCmdInfo* info, const string &tlMsgName)
{
    return "";
}




