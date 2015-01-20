extern "C"
{
    #include "../err/apue.h"
}
using namespace std;


int main(int argc, char* argv[])
{
    printf("hello world from process ID %d\n", getpid());
    exit(0);
}
