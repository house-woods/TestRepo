#ifndef ConfigObject_h
  #define ConfigObject_h 1

using namespace std;

  #include <ospace/file.h>
  #include <string>
  #include <list>
  #include <map>
  #include "sysmon.hh"

// This class represents a configuration map of key and its associated value.
// It is responsible for 
// - getting a value for the specified key
// - setting the value for the specified key
// - converting the map to corba config structure.
// - updating the config map using the information from config structure.
class ConfigObject 
{

public:
  // Default constructor
  ConfigObject();

  // Copy constructor
  ConfigObject(const ConfigObject &right);

  // Destructor
  ~ConfigObject();

  // Returns the value for the specified config parameter
  string getConfigValue (string name);

  // Sets the value for the specified config parameter
  void setConfigValue (string name, string value);

  // Converts the map to corba config structure.
  void getConfiguration (SysMon::Configuration& config);

  // Updates the config map using the information from config structure.
  void setConfiguration (const SysMon::Configuration& config);
private:
  map<string,string> _configuration; // configuration map for storing key and values.

};

#endif
