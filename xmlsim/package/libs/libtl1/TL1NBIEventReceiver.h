#ifndef _NBIEventReceiver_H_
#define _NBIEventReceiver_H_

#include <string>

using namespace std;

#include "TL1EventReceiver.h"

namespace EMS_FW
{
class TL1NBIEventReceiver : public TL1EventReceiver
{
public:

    TL1NBIEventReceiver ();
    ~TL1NBIEventReceiver ();
    bool handleEvent(TL1Message* event, const string& rawMessage = "");

};

};

#endif
