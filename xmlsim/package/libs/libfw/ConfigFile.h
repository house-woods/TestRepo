#ifndef ConfigFile_h
  #define ConfigFile_h 1
  #include <ospace/file.h>
  #include <iostream>
  #include <fstream>
  #include "ConfigObject.h"
  #ifdef OS_USE_ALTERNATE_STD
using namespace std;
  #endif

//	This class represents represents a config file which
//  contains a list of key, value pair with the format key=value
//	It is responsible for 
//  - Parsing key, value pairs from the config file and storing those in 
//    the internal map.
//  - Getting the value of the specified config parameter.
//  - Determining if the file exists and properly formatted.
class ConfigFile : public ConfigObject  
{

public:
    // Default constructor
  ConfigFile();

    // Copy constructor
  ConfigFile(const ConfigFile &right);

    // Constructor for the specified file.
  ConfigFile (string fileName);

    // Destructor.
  ~ConfigFile();

    // Returns next line from the config file
  bool getLine (string& line);

    // Returns true if the file exists and is properly formatted
  bool isGood();

private:
  bool _isGood; // true, if file exists and is properly formatted
  ifstream *f;  // File pointer.
};

#endif
