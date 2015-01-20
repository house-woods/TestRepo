#include "ConfigFile.h"
#include "sysErrLog.h"
using namespace ELogger;
extern char* sourceName;

/*****************************************************************
 * Default constructor 
 * 
 *****************************************************************/
ConfigFile::ConfigFile()
{
    // Initialize the pointer to the stream pointer.
    f = 0;
}

/*****************************************************************
 * Copy constructor
 * 
 *****************************************************************/
ConfigFile::ConfigFile(const ConfigFile &right)
{}

/*****************************************************************
 * This method is responsible for returning true if config file 
 * exists and is properly formatted.
 * 
 *****************************************************************/
bool ConfigFile::isGood()
{
    return _isGood;

}

/*****************************************************************
 * This constructor is responsible for parsing config values
 * from the specified files and store them in the map.
 * 
 *****************************************************************/
ConfigFile::ConfigFile (string fileName)
{
    f = 0;
    _isGood = false;
    f = new ifstream();
    f->open(fileName.c_str(), ios::nocreate);
    f->setf(ios::skipws);
    if ((*f) == false)
    {
        ErrorLogger::instance()->logVarError("ConfigFile", "ConfigFile", MAJOR,
                                             "Config File %s does not exist..", fileName.c_str());
        //return false;

    }
    else
        _isGood = true;
    string line;
    int length = 0;
    while (getLine(line))
    {

        if (line.length() == 0)  // get next config info if it is empty line
            continue;
        if (line.find("#") == 0) // get next config info if it comments with "#"
            continue;

        string key = "";
        string value = "";
        if (line.size() > 0)
        {
            int pos = line.find("=");
            if (pos >= 1)
            {
                key = line.substr(0, pos);
                if (((unsigned)(pos + 1)) < line.size())
                    value = line.substr(pos + 1);
            }
            else
            {
                //ErrorLogger::instance()->logVarError("Tutil",Critical,
                //"extractKeyValue() Invalid Line no delimeter %s",line.c_str());
            }


            setConfigValue(key, value);

        }

    }
}

/*****************************************************************
 * Destructor
 * 
 *****************************************************************/
ConfigFile::~ConfigFile()
{
    // Cleanup the stream allocation
    delete f;
}


/*****************************************************************
 * This method returns a line from the config file.
 * 
 *****************************************************************/
bool ConfigFile::getLine (string& line)
{
    if (!(*f))
        return false;
    char cline[2048];
    f->getline(cline, 2048);
    string tmp = cline;
    int len = tmp.length();
    int last = tmp.find_last_not_of(" ", len);
    if ((last < 0) || (last > len))
    {
        line = tmp; // no white spaces at the end
        return true;
    }
    line = tmp.substr(0, last + 1);
    return true;
}
