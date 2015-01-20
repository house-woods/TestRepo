#ifndef _OSUTIL_H_
#define _OSUTIL_H_

namespace EMS_FW
{
typedef struct SysInfo
{
    string osVersion;
    string machineArch;
    string hostname;
    unsigned long mbMemTotal;
    unsigned long mbSwapTotal;
    unsigned long maxFDPerProcess;
}
SysInfo;

typedef struct FilesystemUsage
{
    unsigned long kbTotal;
    unsigned long kbUsed;
    unsigned long kbAvailable;
    int percentUsed;
}
FilesystemUsage;

typedef struct SysUsage
{
    unsigned long mbMemUsed;
    int percentMemUsed;

    unsigned long mbSwapUsed;
    int percentSwapUsed;

    double percentCPUActual;
    int percentCPURounded;
}
SysUsage;


typedef struct ProcessUsage
{
    unsigned long mbMemUsed;
    int percentMemUsed;

    unsigned long mbSwapUsed;
    int percentSwapUsed;

    double percentCPUActual;
    int percentCPURounded;

    unsigned long numberFDUsed;
    int percentFDUsed;
}
ProcessUsage;

class OSUtil
{
public:
    static bool getSystemInfo (SysInfo &info);
    static bool getSystemUsage (SysUsage &info, const SysInfo &sysInfo);
    static bool getProcessUsage (unsigned long pid, ProcessUsage &info, const SysInfo &sysInfo);
    static bool getFilesystemsList (std::list<std::string>& fileSystems);
    static bool getFilesystemUsage (const std::string &filesystem, FilesystemUsage &info);
    static std::string getmyPid ();
    static std::string getParentPid ();
    static unsigned long getSystemUptimeInSeconds ();
    static unsigned long getPrcoessUptimeInSeconds ();
};
};


#endif
