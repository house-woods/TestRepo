extern "C"
{
#include "../err/apue.h"
#include <fcntl.h>
#include <unistd.h>
}


int main(int argc, char* argv[])
{
    int fdes = open("./test.txt",O_RDWR|O_APPEND);   
    if(lseek(fdes, 0, SEEK_SET) < 0)
    {
        printf("lseek error.\n");
    }
    char buff[1024];
    ssize_t size = read(fdes, buff, 512);
    printf("content:%s\n", buff);
    char buffs[20] = "hello";
    ssize_t wSize = write(fdes, buffs, 20);
    if(lseek(fdes, 0, SEEK_SET) < 0)
    {
        printf("lseek error.\n");
    }
    buff[0] = '\0';
    size = read(fdes, buff, 512);
    printf("content:%s\n", buff);
    
    return 0;
}
