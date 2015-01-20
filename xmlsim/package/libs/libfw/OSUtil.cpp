#include <string>
#include <list>
#include <sstream>

using namespace std;

#include "OSUtil.h"
#include <stdio.h>
#include <math.h>
#include <sys/mnttab.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/swap.h>
#include <utmpx.h>
#include <dirent.h>
#include "tutil.h"
#include "sysErrLog.h"
#include "BaseException.h"
#include <procfs.h>
#include <fcntl.h>
#include <errno.h>
#include <kstat.h>

extern const char* sourceName;
static const char* className = "OSUtil";

using namespace EMS_FW;
using namespace ELogger;

bool OSUtil::getSystemInfo (SysInfo &info)
{
    char buf [300];

    info.osVersion = "";
    if (sysinfo (SI_SYSNAME, buf, 300) != -1)
        info.osVersion.append (buf);
    if (sysinfo (SI_RELEASE, buf, 300) != -1)
        info.osVersion.append (buf);
    if (sysinfo (SI_VERSION, buf, 300) != -1)
        info.osVersion.append (buf);

    info.machineArch = "";
    if (sysinfo (SI_MACHINE, buf, 300) != -1)
        info.machineArch.append (buf);
    if (sysinfo (SI_ARCHITECTURE, buf, 300) != -1)
        info.machineArch.append (buf);
    if (sysinfo (SI_PLATFORM, buf, 300) != -1)
        info.machineArch.append (buf);

    info.hostname = "Hostname: " + Tutil::getHostname() + " IP: " + Tutil::getIPAddress();

    unsigned long memoryPageSize = sysconf (_SC_PAGESIZE);
    unsigned long totalMemoryPages = sysconf (_SC_PHYS_PAGES);
    unsigned long freeMemoryPages = sysconf (_SC_AVPHYS_PAGES);
    info.mbMemTotal = ((longlong_t) totalMemoryPages * (longlong_t) memoryPageSize) / (1024 * 1024);

    struct anoninfo swpInfo;
    if (swapctl(SC_AINFO, &swpInfo) != -1)
    {
        unsigned long totalPages = swpInfo.ani_max;
        unsigned long usedPages = swpInfo.ani_resv;
        unsigned long availablePages = totalPages - usedPages;
        int memoryPageSizeDivisor = 0;
        while (memoryPageSize > 1)
        {
            memoryPageSize >>= 1;
            memoryPageSizeDivisor++;
        }
        unsigned long mbUsed = usedPages >> (20 - memoryPageSizeDivisor);
        unsigned long mbAvailable = availablePages >> (20 - memoryPageSizeDivisor);
        unsigned long mbTotal = totalPages >> (20 - memoryPageSizeDivisor);
        info.mbSwapTotal = mbTotal;
    }

    info.maxFDPerProcess = getdtablesize();
    ErrorLogger::logVarError(sourceName, className, "getSystemInfo", __LINE__, TRACE1, "Obtained System info as follows\n\nOS Version [%s]\nMachine Arch [%s]\nHost Address [%s]\nTotal Memory in MB [%lu]\nTotal SWAP in MB [%lu]\nMax number of FDs per process [%lu]\n\n",
                             info.osVersion.c_str(), info.machineArch.c_str(), info.hostname.c_str(), info.mbMemTotal, info.mbSwapTotal, info.maxFDPerProcess);

    return true;
}

bool OSUtil::getFilesystemsList (list<string> &filesystems)
{
    FILE *fp;
    struct mnttab mp;
    if ((fp = fopen ("/etc/mnttab", "r")) != NULL)
    {
        while (!getmntent (fp, &mp))
            filesystems.push_back (mp.mnt_mountp);
    }
    else
    {
        ErrorLogger::logVarError(sourceName, className, "getFilesystemsList", __LINE__, MINOR, "Could not open /etc/mnttab to get filesystem list");
        return false;
    }
    return true;
}

bool OSUtil::getFilesystemUsage (const string &name, FilesystemUsage &info)
{
    info.kbAvailable = info.kbTotal = info.kbUsed = info.percentUsed = 0;

    struct statvfs buf;
    if (statvfs (name.c_str(), &buf) != -1)
    {
        float kbFragmentSize = float (buf.f_frsize) / 1024;
        info.kbTotal = buf.f_blocks * kbFragmentSize;
        info.kbUsed = (buf.f_blocks - buf.f_bfree) * kbFragmentSize;
        info.kbAvailable = buf.f_bavail * kbFragmentSize;

        if (buf.f_blocks)
            info.percentUsed = ((float) (buf.f_blocks - buf.f_bfree) / (float) buf.f_blocks) * 100 + 1;
        //int roundedOffPercentageUsed = roundf ((float) (buf.f_blocks-buf.f_bfree) / (float) buf.f_blocks);

        ErrorLogger::logVarError(sourceName, className, "getFilesystemUsage", __LINE__, TRACE8, "Obtained info for\n\nfilesystem [%s]\nKB Fragment size [%f]\nKB Total [%lu]\nKB Used [%lu]\nKB Available [%lu]\nPercent Used [%d]\n\n",
                                 name.c_str(), kbFragmentSize, info.kbTotal, info.kbUsed, info.kbAvailable, info.percentUsed);
    }
    else
    {
        ErrorLogger::logVarError(sourceName, className, "getFilesystemUsage", __LINE__, TRACE1, "Could not obtain info for filesystem [%s]", name.c_str());
        return false;
    }
    return true;
}

bool OSUtil::getSystemUsage (SysUsage &info, const SysInfo &sysInfo)
{
    info.mbMemUsed = info.mbSwapUsed = info.percentCPUActual = info.percentCPURounded = info.percentMemUsed = info.percentSwapUsed = 0;

    unsigned long memoryPageSize = sysconf (_SC_PAGESIZE);
    unsigned long totalMemoryPages = sysconf (_SC_PHYS_PAGES);
    unsigned long freeMemoryPages = sysconf (_SC_AVPHYS_PAGES);
    unsigned long usedMemoryPages = totalMemoryPages - freeMemoryPages;

    unsigned long pagesPerMB = 1048576L / memoryPageSize; // 1048576 = 1MB
    info.mbMemUsed = usedMemoryPages / pagesPerMB; // In MB

    if (totalMemoryPages)
        info.percentMemUsed = ((double)usedMemoryPages * 100.0) / (double)totalMemoryPages;

    struct anoninfo swpInfo;
    if (swapctl(SC_AINFO, &swpInfo) != -1)
    {
        unsigned long totalPages = swpInfo.ani_max;
        unsigned long usedPages = swpInfo.ani_resv;
        unsigned long availablePages = totalPages - usedPages;
        int memoryPageSizeDivisor = 0;
        while (memoryPageSize > 1)
        {
            memoryPageSize >>= 1;
            memoryPageSizeDivisor++;
        }
        unsigned long mbUsed = usedPages >> (20 - memoryPageSizeDivisor);
        unsigned long mbAvailable = availablePages >> (20 - memoryPageSizeDivisor);
        unsigned long mbTotal = totalPages >> (20 - memoryPageSizeDivisor);

        info.mbSwapUsed = mbUsed;
        if (mbTotal)
            info.percentSwapUsed = (double) mbUsed / (double) mbTotal * 100;
        ErrorLogger::logVarError(sourceName, className, "getSystemUsage", __LINE__, TRACE8, "Obtained info for\n\nSystem\nMB Memory Used [%lu]\nPercent Memory Used [%d]\nMB Swap Used [%lu]\nPercent Swap Used [%d]\nPercent CPU Used Actual [%f]\nPercent CPU Used Rounded [%d]\n\n",
                                 info.mbMemUsed, info.percentMemUsed, info.mbSwapUsed, info.percentSwapUsed, info.percentCPUActual, info.percentCPURounded);

    }
    // info.percentCPUUsed =
    return true;
}

bool OSUtil::getProcessUsage (unsigned long pid, ProcessUsage &info, const SysInfo &sysInfo)
{
    info.mbMemUsed = info.percentMemUsed = info.mbSwapUsed = info.percentSwapUsed = info.percentCPUActual = info.percentCPURounded = info.numberFDUsed = info.percentFDUsed = 0;

    string fdDirectory = "/proc/" + Tutil::numberToString(pid) + "/fd";
    DIR *dir = opendir (fdDirectory.c_str());

    long name_max = pathconf(fdDirectory.c_str(), _PC_NAME_MAX);
    struct dirent* entry = (dirent*) malloc(offsetof(struct dirent, d_name) + name_max + 1);
    struct dirent *result;
    if (dir)
    {
        while (true)
        {
            if ( (readdir_r (dir, entry, &result) == 0) && (result != NULL))
                info.numberFDUsed++;
            else
                break;
        }
        closedir (dir);
    }
    free (entry);
    // to get rid of . and ..
    info.numberFDUsed -= 2;



    /*    string fdDirectory = "/proc/"+Tutil::numberToString(pid)+"/fd";
        DIR *dirp = opendir (fdDirectory.c_str());
        struct dirent entry, *result;
        if (dirp)
        {
             while (true)
             {
                int returCode = readdir_r(dirp,&entry,&result);
                if ((result==NULL) || (returCode!=0))
                    break;
                info.numberFDUsed++;
            }
            closedir (dirp);
        }
        
        info.numberFDUsed-=2;*/

    if (sysInfo.maxFDPerProcess)
        info.percentFDUsed = (double) info.numberFDUsed / (double) sysInfo.maxFDPerProcess * 100;

    char pname[PATH_MAX];
    int pdlen;
    int found;
    int procfd; /* filedescriptor for /proc/nnnnn/psinfo */
    char *tp;    /* ptr to ttyname,  if any */
    psinfo_t psInfo;  /* process information from /proc */
    lwpsinfo_t *lwpsinfo;   /* array of lwpsinfo structs */

    pdlen = snprintf(pname, sizeof (pname), "%s/%lu/", "/proc", pid);
    if (pdlen >= sizeof (pname) - 10)
    {
        return false;
    }

    bool procInfoReadOK = true;
    for (int retryCounter = 0;retryCounter < 3;retryCounter++)
    {
        strcpy(&pname[pdlen], "psinfo");
        if ((procfd = open(pname, O_RDONLY)) == -1)
        {
            /* Process may have exited meanwhile. */
            return false;
        }
        if (read(procfd, (char *)&psInfo, sizeof (psInfo)) < 0)
        {
            procInfoReadOK = false;
            int saverr = errno;
            (void) close(procfd);
            if (saverr == EAGAIN)
                continue;
            return false;
        }
        else
            break;
    }
    if (!procInfoReadOK)
        return false;
    close (procfd);
    double actualPercentage = (((double)(psInfo.pr_pctcpu)) / 0x8000 * 100);
    info.percentCPUActual = actualPercentage;
    info.percentCPURounded = actualPercentage;
    info.mbMemUsed = ((double) psInfo.pr_rssize / double (1024));
    if (sysInfo.mbMemTotal)
        info.percentMemUsed = (double) info.mbMemUsed / (double) sysInfo.mbMemTotal * (double) 100;
    info.mbSwapUsed = ((double) psInfo.pr_size / double (1024));
    if (sysInfo.mbSwapTotal)
        info.percentSwapUsed = (double) info.mbSwapUsed / (double) sysInfo.mbSwapTotal * (double) 100;

    ErrorLogger::logVarError(sourceName, className, "getProcessUsage", __LINE__, TRACE8, "Obtained info for\n\nProcess with PID [%lu]\nMB Memory Used [%lu]\nPercent Memory Used [%d]\nMB Swap Used [%lu]\nPercent Swap Used [%d]\nPercent CPU Used Actual [%f]\nPercent CPU Used Rounded [%d]\nNumber of FDs [%lu]\nPercent FD used [%d]\n\n",
                             pid, info.mbMemUsed, info.percentMemUsed, info.mbSwapUsed, info.percentSwapUsed, info.percentCPUActual, info.percentCPURounded, info.numberFDUsed, info.percentFDUsed);

    return true;
}

unsigned long OSUtil::getSystemUptimeInSeconds ()
{
    unsigned long systemUpTimeInSeconds = 0;

    unsigned long clockRate = sysconf(_SC_CLK_TCK);

    kstat_ctl_t *kc = kstat_open();
    kstat_t* systeMiscStruct = kstat_lookup (kc, "unix", 0, "system_misc");
    if (systeMiscStruct)
    {
        if (kstat_read (kc, systeMiscStruct, NULL) != -1)
        {
            kstat_named_t *bootTime = (kstat_named_t *) kstat_data_lookup(systeMiscStruct, "clk_intr");
            if (clockRate != 0)
                systemUpTimeInSeconds = bootTime->value.ul / clockRate;
        }
    }
    kstat_close(kc);

    return systemUpTimeInSeconds;
}

unsigned long OSUtil::getPrcoessUptimeInSeconds ()
{
    return 0;
}

string OSUtil::getmyPid ()
{
    return Tutil::numberToString (getpid());
}

string OSUtil::getParentPid ()
{
    return Tutil::numberToString (getppid());
}




