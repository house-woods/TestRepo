#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ourhdr.h"

int main(int argc, char* argv[])
{
    pid_t pid;
    if((pid = fork()) < 0)
    {
        return -1;
    }
    else if(pid != 0)
    {
        exit(0);
    }
    setsid();
    chdir("/");
    umask(0);
    while(true)
    {
        sleep(10);
    }
    return 0;
}
