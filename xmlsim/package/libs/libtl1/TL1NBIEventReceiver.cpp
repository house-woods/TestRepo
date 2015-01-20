#include "TL1NBIEventReceiver.h"
#include "TL1Connection.h"
#include "CmServer.h"
#include "sysErrLog.h"

using namespace ELogger;
using namespace EMS_FW;

extern const char *sourceName;
static const char* className = "BaseFeatureManager";

TL1NBIEventReceiver::TL1NBIEventReceiver ()
{}

TL1NBIEventReceiver::~TL1NBIEventReceiver()
{}

bool TL1NBIEventReceiver::handleEvent(TL1Message* event, const string& rawMessage)
{
    TL1Connection* con = (TL1Connection*) Server_i::instance()->getComponent("NBI");
    if (con)
        con->send_data(rawMessage.c_str());

    return true;
}
