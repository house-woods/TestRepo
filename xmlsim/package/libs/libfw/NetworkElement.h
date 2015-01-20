#ifndef NetworkElement_h
#define NetworkElement_h 1

#include "sysmon.hh"

#include "FWComponent.h"

#include <string>
#include <list>
#include <map>

#include <ospace/thread.h>
#include <ospace/thread/rwsem.h>


// This class represents a network element managed by the
// EMS server in the server group. It provides the base implementation.
//  Each subclass is supposed to provide implementation specific
//  to the server type and network element type.
//  An EMS server can manage one or more network elements.
//  This class and its subclasses are responsible for
//  - Starting the network element with in a EMS server.
//  - Stopping the network element with in a EMS server.
//  - Setting and getting the state of the network element.
//  - Getting the value of the specified config parameter.
//  - Setting the value of the specified config parameter.
// - Printing current config values.
//  - Registering and unregistering framework components.
//  Each subclass is supposed to provide implementation for
//  following operations.
//  - initialize()
//    This operation is called when sysmon wants to start this network element
//    in the ems server. It is responsible for performing all the activities necessary to
//    start communicating with the network element like
//    - Registering the event handlers for handling events coming from the network element
//    - Initiating auto-discovery/synchronization
//    - Registering corba interfaces with naming service
//    - Connecting to the notifications channels as suppliers or consumers
//  - synchronizeWithPeer()
//    This method will be called when all the peers with in a fcp group are initialized.
//    e.g FML, FMW and FMR are peers supporting fault handling.
//    This operation is responsible for performing synchronizations with in the fcp group.
//    e.g. FMLoder can use this method to syncronize with FmReader and Fm Writer.
//  - synchronizeWithFcp()
//    This method will be called when all the servers with in a server group are intialized.
//    This operation is responsible for performing synchronizations across the fcp groups.
//    e.g. CM can use this method to synchronize with Fm for bubbleup.
//  - finalize()
//    This operation is called when sysmon wants to stop this network element
//    in the ems server. It is responsible for performing all the activities necessary to
//    stop communicating with the network element like
//    - Unregistering the event handlers for handling events coming from the network element
//    - Unregistering corba interfaces from the naming service
//    - Disonnecting from the notifications channels as suppliers or consumers
//    - Deleting all the resources allocated when the network element was active.
//  - configurationChanged()
//   This opeartin is called when sysmon wants to change the configuration  of
//    this network element.
namespace EMS_FW
{
class NetworkElement
{
public:
    //    This operation is called when sysmon wants to start this network element
    //    in the ems server. It is responsible for performing all the activities necessary to
    //    start communicating with the network element like
    //    - Registering the event handlers for handling events coming from the network element
    //    - Initiating auto-discovery/synchronization
    //    - Registering corba interfaces with naming service
    //    - Connecting to the notifications channels as suppliers or consumers
    virtual void initialize ();

    //    This operation will be called when all the peers with in a fcp group are initialized.
    //    e.g FML, FMW and FMR are peers supporting fault handling.
    //    This operation is responsible for performing synchronizations with in the fcp group.
    //    e.g. FMLoder can use this method to syncronize with FmReader and Fm Writer.
    virtual void synchronizeWithPeer();


    //    This operation will be called when all the servers with in a server group are intialized.
    //    This operation is responsible for performing synchronizations across the fcp groups.
    //    e.g. CM can use this method to synchronize with Fm for bubbleup.
    virtual void synchronizeWithFcp();

    //    This operation is called when sysmon wants to stop this network element
    //    in the ems server. It is responsible for performing all the activities necessary to
    //    stop communicating with the network element like
    //    - Unregistering the event handlers for handling events coming from the network element
    //    - Unregistering corba interfaces from the naming service
    //    - Disonnecting from the notifications channels as suppliers or consumers
    //    - Deleting all the resources allocated when the network element was active.
    virtual void finalize ();

    // This opeartin is called when sysmon wants to change the configuration  of
    //    this network element.
    virtual void configurationChanged(const SysMon::NetworkElementConfiguration& config);

    // Returns the value of the specified config parameter.
    std::string getConfigValue (const std::string& name) const;
    // Returns the value of the specified config parameter or the defaultValue.
    std::string getDefaultConfigValue (const std::string& name, const std::string& defaultValue);

    // Sets the value of the specified config parameter.
    void setConfigValue(const std::string& name, const std::string& value);

    // Default constructor.
    NetworkElement();

    // Copy constructor.
    NetworkElement(const NetworkElement &right);

    // Destructor.
    virtual ~NetworkElement();

    // This operation is responsible for staring this network element.
    void start (const SysMon::Configuration& config);

    // This operation is responsible for stopping this network element.
    void stop();

    // This method sets thes state of the network element.
    void setState(SysMon::State state);

    // This operation is responsible for changing the configuration
    // associated with this network element. It calls the
    // configurationChanged() method to indicate to the
    // subclasses that the sysmon wants to change the
    // configuration associated with that NE.
    void setConfiguration (const SysMon::NetworkElementConfiguration& config);

    // Displays the configuration of this network element.
    void displayConfiguration();

    // Returns the framework component for the specified name.
    EMS_FW::FWComponent* getComponent(const std::string& name);

    // Registers the framework component for the specified name.
    void registerComponent(EMS_FW::FWComponent* component);

    // Unregisters the framework component for the specified name.
    void unregisterComponent(EMS_FW::FWComponent* component);

    // Returns true if all the config parameters specified in mapList are present.
    bool checkMap(const std::list<std::string>& mapList, std::string& missingStrings);

    // Prints a list of registered framework components.
    void printRegisteredResources();

    // This operation is responsible for running this network element.(Obsolete)
    virtual void run();

    //added by lzou
    // This method registers an framework component in a specific group
    void registerGroupComponent(EMS_FW::FWComponent* component, const std::string& groupName, const std::string& componentName);
    // This method unregisters an framework component in a specific group
    void unregisterGroupComponent(EMS_FW::FWComponent* component, const std::string& groupName, const std::string& componentName);

    //    This method gets a list of components in a specified group.
    std::list<EMS_FW::FWComponent* > getComponents(const std::string& groupName, const std::string& neId);

    virtual void accessNe();
    // increment the counter that counts the use of the Ne resource

    virtual void releaseNe();
    // decrement the counter that counts the use of the Ne resource.

    void setReadOnly(bool readOnly)
    {
        _readOnly = readOnly;
    }
    bool isReadOnly()
    {
        return _readOnly;
    }

    virtual void setAutodiscoverMode(const std::string& mode);
    // Put Network Element in on/off autodiscover mode.

    const std::string& getAutodiscoverMode() const
    {
        return _autodiscoverMode;
    }

    virtual bool isAutodiscoverMode()
    {
        return false;
    }
    // Return true if Network Element is in autodiscover mode. Default is false.

    virtual void disableNe();
    // put the Ne in a disabled state and wait
    // the currently active users of the Ne to release the use of the Ne.
    // If time out expire and still there are locks on the Ne kill the process.

    virtual void enableNe();
    // put the Ne in a enabled state, after the disbled state, to help serialize access to DBs


protected:

    std::string _autodiscoverMode;


private:
    typedef std::map<std::string, std::string, std::less<std::string> > cfgMap;
    typedef std::map<std::string, EMS_FW::FWComponent*, std::less<std::string> > cmpMap;
    //added by lzou
    typedef std::map<std::string, std::list<EMS_FW::FWComponent*>*, std::less<std::string> > grpMap;

    typedef osi_sem_lock<osi_mutex_semaphore> GuardLock;

    osi_mutex _guardMutex;


    int _usersCounter;
    // count the number of users currently using the Ne resources.

    SysMon::State _state;   // The state of the network element

    osi_read_write_semaphore _configurationSemaphore;

    cfgMap _configuration; // Configuration map

    cmpMap _components; // Component map

    grpMap _groups;  // Local cache to store group information.

    void _disableNe(); // Put the Ne in disabled state;

    bool _readOnly;
};

};
#endif
