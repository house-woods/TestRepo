#ifndef TL1NESecurityObject_h
#define TL1NESecurityObject_h

#include "EmsObject.h"

#include <map>

#include <ospace/thread/mutex.h>
#include <ospace/thread/lock.h>

//class PMoLite;
class EMS_FW::BaseCmdInfo;
class EMS_FW::GuiCmdInfo;

class TL1NESecurityObject : public EMS_FW::EmsObject
{
public:
    TL1NESecurityObject();
    virtual ~TL1NESecurityObject();

    virtual void handleCreateEvent (const EMS_FW::BaseCmdInfo* info);
    virtual void handleCreateEvent (const EMS_FW::BaseCmdInfoList* infoList)
    {}

    virtual void handleDeleteEvent (const EMS_FW::BaseCmdInfo* info);
    virtual void handleDeleteEvent (const EMS_FW::BaseCmdInfoList* infoList)
    {}

    virtual void getAttributes (const EMS_FW::GuiCmdInfo* info, list<MoAttributePo*>& attrList);

    virtual void action(const EMS_FW::GuiCmdInfo* info, list<MoAttributePo*>& resList);

    virtual bool interestedEvent (const EMS_FW::BaseCmdInfo* info);
    virtual bool interestedEvent (const EMS_FW::BaseCmdInfoList* infoList)
    {
        return false;
    }

    virtual const string& getClassName() const;

protected:

    // Overridables
    //virtual void initializeMoLite (PMoLite* mol, const EMS_FW::BaseCmdInfo* info);
    virtual string getTL1UserMoc();
    void _sendIndicationToClients(Node::Indication& nodeIndication);
    //virtual bool _fillUserSummary(PMoLite* puserMol, Node::NodeSummary& summary);

private:
    static const string _className;
    static const string AS_KEY_TL1USERS;

    static osi_mutex _timeMutex;
    typedef osi_sem_lock<osi_mutex_semaphore> TimeLock;


    void _insertInterestedClient(const string& NeId, const EMS_FW::SESSIONIDTYPE& sessionId);
    void _removeNotInterestedClient(const EMS_FW::SESSIONIDTYPE& sessionId);


    typedef std::multimap<string, EMS_FW::SESSIONIDTYPE, less<string> > InterestedClientsMap;
    typedef std::multimap<string, EMS_FW::SESSIONIDTYPE, less<string> >::iterator InterestedClientsIter;

    static InterestedClientsMap _clientsMap;
};

#endif
