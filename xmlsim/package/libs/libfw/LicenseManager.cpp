#include <string>
#include <map>
#include <vector>

using namespace std;

#include "LicenseManager.h"
#include "BaseException.h"
#include "sysErrLog.h"
#include "Server_i.h"
#include <ospace/helper.h>
#include <iostream>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <fstream>
#include <ospace/file.h>
#include "tutil.h"

using namespace std;
using namespace EMS_FW;
using namespace ELogger;

extern const char *sourceName;
static const char* className = "LicenseManager";

LicenseManager* LicenseManager::_instance = NULL;

LicenseManager::LicenseManager()
{}

LicenseManager::~LicenseManager()
{}

LicenseManager* LicenseManager::instance()
{
    if (_instance == NULL)
    {
        _instance = new LicenseManager();
        _instance->initialize();
    }
    return _instance;
}

void LicenseManager::initialize()
{
    const char *methodName = "initialize";
    try
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE4, "Method started");
        _licenseMap.clear();
        readLicenseFiles();
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE4, "Method ended");
    }
    catch (osi_file_toolkit_error& ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR, "Failed to initialize license manager; %s", ex.description(ex.code()));
    }
    catch (BaseException &ex)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR, "Failed to initialize license manager; %s", ex.getDescription().c_str());
    }
    catch (...)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR, "Failed to initialize license manager; Unknown error");
    }
}

void LicenseManager::getLicense(LicenseMap& licenses)
{
    const char *methodName = "getLicense";

    licenses.clear();

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Returning following configured licenses:");

    for (LicenseMap::iterator i = _licenseMap.begin();i != _licenseMap.end();i++)
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Feature name [%s]  Enabled [%d]", ((*i).first).c_str(), (*i).second);

    licenses.insert(_licenseMap.begin(), _licenseMap.end());
}

bool LicenseManager::isLicensed(const std::string& feature)
{
    const char *methodName = "isLicensed";

    bool isLicensed = false;

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Verifying whether feature [%s] is licensed", feature.c_str());
    LicenseMap::iterator i = _licenseMap.find(feature);
    if (i != _licenseMap.end())
    {
        isLicensed = i->second;
    }
    if (isLicensed)
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Feature [%s] is licensed", feature.c_str());
    else
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Feature [%s] is NOT licensed", feature.c_str());

    return isLicensed;
}

void LicenseManager::readLicenseFiles()
{
    const char *methodName = "readLicenseFiles";

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE4, "Method started");

    _licenseMap.clear();

    string cfgDirectory = Server_i::instance()->getConfigValue("BaseDirectory") + EMSDEF::SLASH + "EMS/cfg";

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Attempting to read in license files from [%s]", cfgDirectory.c_str());

    osi_directory dir(cfgDirectory);
    vector<string> filenameList = dir.filenames(); // get a list of all the files in the folder

    int numberOfLicenseFiles = 0;

    for (vector<string>::const_iterator it = filenameList.begin(); it != filenameList.end(); it++)
    {
        string filename = cfgDirectory + EMSDEF::SLASH + *it;
        osi_path path = filename;
        string extension = path.extension();
        Tutil::toLowerCase (extension);
        if (extension != "lic")
            continue;

        numberOfLicenseFiles++;

        int idx = 0;
        char* result = NULL;
        char* strtmp = NULL;

        readFile( filename.c_str(), &result);

        if (result == NULL)
        {
            /* License file is invalid or corrupt */
            ErrorLogger::logError(sourceName, className, methodName, __LINE__, MAJOR,
                                  "License file is invalid or corrupt. All features default to NOT LICENSED");

            return ;
        }

        int ver_n = result[idx + 3] & 0xff;
        ver_n <<= 8;
        ver_n += result[idx + 2] & 0xff;
        ver_n <<= 8;
        ver_n += result[idx + 1] & 0xff;
        ver_n <<= 8;
        ver_n += result[idx + 0] & 0xff;
        idx += 4;

        strtmp = new char[ver_n + 1];
        strtmp[ver_n] = '\0';
        for (int j = 0; j < ver_n; ++j)
        {
            strtmp[j] = result[idx++];
        }
        string version = strtmp;
        delete strtmp;

        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                                 "License file version %s.", version.c_str());

        unsigned long d = 0;
        for (int j = 7; j >= 0; --j)
        {
            d += result[idx + j] & 0xff;
            if (j != 0)
                d <<= 8;
        }
        idx += 8;

        time_t t = (time_t)d;

        vector<string> features;
        int feature_array_n = result[idx + 3] & 0xff;
        feature_array_n <<= 8;
        feature_array_n += result[idx + 2] & 0xff;
        feature_array_n <<= 8;
        feature_array_n += result[idx + 1] & 0xff;
        feature_array_n <<= 8;
        feature_array_n += result[idx + 0] & 0xff;
        idx += 4;

        int rounded_bit_slots = (feature_array_n % 8 == 0) ? feature_array_n / 8 : feature_array_n / 8 + 1;

        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1,
                                 "Number of Features %d.", feature_array_n);

        int tmp = 0;
        for (int j = 0; j < feature_array_n; ++j)
        {
            tmp = result[idx + 3] & 0xff;
            tmp <<= 8;
            tmp += result[idx + 2] & 0xff;
            tmp <<= 8;
            tmp += result[idx + 1] & 0xff;
            tmp <<= 8;
            tmp += result[idx + 0] & 0xff;
            idx += 4;

            strtmp = new char[tmp + 1];
            strtmp[tmp] = '\0';
            for (int j = 0; j < tmp; ++j)
            {
                strtmp[j] = result[idx++];
            }
            string name = strtmp;
            delete strtmp;

            // if feature was not added already, add it disabled by default
            LicenseMap::iterator iter = _licenseMap.find (name);
            if (iter == _licenseMap.end())
                _licenseMap.insert(LicenseMap::value_type(name, false));
            features.push_back(name);
        }

        for (int i = 0; i < rounded_bit_slots; ++i)
        {
            unsigned char next = result[idx++];
            for (int j = 0; j < 8; ++j)
            {
                int index = 8 * i + j;

                if (index < feature_array_n)
                {
                    LicenseMap::iterator i = _licenseMap.find(features[index]);
                    if (i != _licenseMap.end())
                    {
                        // if feature is enabled, don't disable it
                        if (i->second == false)
                            i->second = (next & (0x01 << j)) > 0;
                    }
                }
            }
        }
        delete result;
        result = NULL;
    }
    if (!numberOfLicenseFiles)
        throw BaseException ("No license files were found under " + cfgDirectory);

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE4, "Method ended");
}

void LicenseManager::readFile(const char* filename, char** data)
{
    const char *methodName = "readFile";

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE4, "Method started");

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Attempting to read licenses from [%s]", filename);

    ifstream in(filename, ios::binary);

    *data = NULL;
    long begin, end, size, token_size;
    char* token = NULL;

    if (!in.good())
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "License file input stream is not good.");
        return ;
    }

    begin = in.tellg();
    in.seekg(0, ios::end);
    end = in.tellg();
    in.seekg(0, ios::beg);

    size = end - begin - 2;
    if (size < 0)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "License file calculated size is incorrect.  License file may have been modified or corrupted.");
        in.close();
        return ;
    }

    token_size = in.get();
    token_size += (in.get() * 256);

    if (token_size >= size)
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "License file calculated token size is incorrect.  License file may have been modified or corrupted.");
        in.close();
        return ;
    }

    int data_size = size - token_size;

    token = new char[token_size];
    *data = new char[data_size + 1];
    (*data)[data_size] = 0;

    in.read(*data, data_size);
    in.read(token, token_size);

    in.close();

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Attempting to verify licensw file [%s]", filename);
    if (!verify((unsigned char*)*data, data_size, (unsigned char*)token, token_size))
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "License file verification failed.  License file may have been modified or corrupted.");
        delete *data;
        *data = NULL;
    }

    delete token;

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE4, "Method ended");
}

bool LicenseManager::verify(const unsigned char* data, int data_n, const unsigned char* token, int token_n)
{
    const char *methodName = "verify";

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE4, "Method started");

    bool val = true;
    int token_size = 0;
    unsigned char received_token[SHA_DIGEST_LENGTH];
    unsigned char generated_token[SHA_DIGEST_LENGTH];
    BN_CTX * ctx = BN_CTX_new();
    BIGNUM* bn_token = 0;
    BIGNUM* k = 0;
    BIGNUM* n = 0;
    BIGNUM* p = 0;

    bn_token = BN_new();
    BN_dec2bn(&n, "24180164132843301963857388585910664570217613156037144177972655318122803970975323816705769986525299236671434207105571291925202783841348149940647037053559444370783712309449885569006426371444963855619854293887495927959998525461961665017748194950740367818504688577091520685525552937666707068751829677231838380093655103629173639179564042647860010703953232948112123063485150275756542188022961318427045492050546525287273898648850372371283816043507927267345819730492231299235122062655607343487408147769179422765860879607203739398731861167346278333411603081381346623577411742560068448762413210235231425005886724543952297997141");
    BN_dec2bn(&p, "65537");
    k = BN_bin2bn(token, token_n, NULL);

    if (ctx != 0 && k != 0 && p != 0 && n != 0 && bn_token != 0
        && BN_mod_exp(bn_token, k, p, n, ctx))
    {
        if (BN_num_bytes(bn_token) == SHA_DIGEST_LENGTH)
        {
            BN_bn2bin(bn_token, received_token);
            SHA1(data, data_n, generated_token);

            for (int j = 0; j < SHA_DIGEST_LENGTH; ++j)
            {
                if (received_token[j] != generated_token[j])
                {
                    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                             "License file token verification failed.  License file may have been modified or corrupted.");
                    val = false;
                    break;
                }
            }
        }
        else
        {
            ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                     "License file token size is incorrect.  License file may have been modified or corrupted.");
            val = false;
        }
    }
    else
    {
        ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, MAJOR,
                                 "Large integer arithmetic library failure during modular exponentiation or memory allocation.");
        val = false;
    }

    if (ctx != 0)
        BN_CTX_free(ctx);
    if (bn_token != 0)
        BN_free(bn_token);
    if (k != 0)
        BN_free(k);
    if (n != 0)
        BN_free(n);
    if (p != 0)
        BN_free(p);

    ErrorLogger::logError(sourceName, className, methodName, __LINE__, TRACE4, "Method ended");

    return val;
}
int LicenseManager::getLicenseNumber(const string& feature)
{
    const char *methodName = "getLicenseNumber";
    int licenseNumber = 0;

    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE4, "Get license number of feature [%s].", feature.c_str());
    for (LicenseMap::iterator i = _licenseMap.begin();i != _licenseMap.end();i++)
    {
        //FEATURE#NUMBER
        if (0 == i->first.compare(0,feature.length(),feature) && i->second && (i->first.length() > (feature.length() + 1)))
        {
            int lCnt = 0;
            lCnt = atoi(i->first.substr(feature.length() + 1,i->first.length()-feature.length()-1).c_str());
            if (lCnt > licenseNumber)
            {
                licenseNumber = lCnt;
            }
        }
    }
    ErrorLogger::logVarError(sourceName, className, methodName, __LINE__, TRACE1, "Feature [%d] is licensed", licenseNumber);

    return licenseNumber;
}
