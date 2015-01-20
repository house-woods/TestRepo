extern "C"
{
    #include "../err/apue.h"
    #include <errno.h>
}

using namespace std;

int main(int argc, char* argv[])
{
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
    errno = ENOENT;
    perror(argv[0]);
    return 0;
}
