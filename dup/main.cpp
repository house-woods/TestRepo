extern "C"
{
#include "../err/apue.h" 
#include <unistd.h>
}

int main(int argc, char* argv[])
{
    int fds = 0;
    int fd = 1;
    fds = dup2(fd, 0);
    printf("ret fds:%d, fd:%d\n", fds, fd);
    fds = dup2(fd, 1);
    printf("ret fds:%d, fd:%d\n", fds, fd);
    fds = dup2(fd, 2);
    printf("ret fds:%d, fd:%d\n", fds, fd);

    if(fd > 2)
    {
        close(fd);
    }
    return 0;
}
