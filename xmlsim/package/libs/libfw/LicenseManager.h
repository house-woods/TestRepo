#ifndef _LICENSEMANAGER_H_
#define _LICENSEMANAGER_H_

#include <string>
#include <map>

using namespace std;

class LicenseManager
{
public:
    typedef std::map<std::string, bool> LicenseMap;
    static LicenseManager *instance();
    bool isLicensed (const string &feature);
    void initialize ();
    void getLicense(LicenseMap& licenses);
    int getLicenseNumber(const string& feature);

protected:
    static LicenseManager* _instance;
    void readLicenseFiles ();
    void readFile(const char*, char**);
    bool verify(const unsigned char*, int, const unsigned char*, int);
    LicenseMap _licenseMap;
    LicenseManager ();
    ~LicenseManager ();

};

#endif

