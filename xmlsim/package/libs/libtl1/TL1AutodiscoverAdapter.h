#ifndef _TL1AUTODISCOVERADAPTER_H_
#define _TL1AUTODISCOVERADAPTER_H_

#include "AgentCmdInfo.h"
#include "TL1GenericCmd.h"

namespace EMS_FW
{

class TL1AutodiscoverAdapter 
{
public:

    TL1AutodiscoverAdapter();
    
    ~TL1AutodiscoverAdapter();
    

    void getElements(const EMS_FW::BaseCmdInfo* discInfo);
    // Start operation  of getting elements from the agent.

    bool hasMoreElements();
    // Return true if there are more undiscovered elements.

    bool getNextElement(EMS_FW::BaseCmdInfo*& info);
    // Return information of the last discovered element.

    void editWarningMessage(const string& neId);
    
protected:
    EMS_FW::TL1GenericCmd* _cmd;

private:

    AgentFilter _fl; // filter to be used in sequential calls
    string _neId;
};

};
#endif
