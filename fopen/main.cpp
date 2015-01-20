extern "C"
{
#include "../err/apue.h"
#include <fcntl.h>
}


int main(int argc, char* argv[])
{
    int filedes1 = 0;
    int filedes2 = 0;
    filedes1 = open("./test.txt", O_RDONLY);
    printf("file des:%d\n", filedes1);
    filedes2 = open("./test.txt", O_RDONLY);
    printf("file des:%d\n", filedes2);
    char tmp = getchar();
    return 0;
}
