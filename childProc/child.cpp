#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    int x = 0;
    bool isParent = true;
    pid_t childpid;

    childpid = fork();
    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }
    if(childpid == 0)
    {
        //child proc
        x += 1;
        printf("I am child %ld, and x = %d.\n", (long)getpid(), x);
        isParent = false;
    }
    else
    {
        //parent proc
        printf("I am parent %ld, and x = %d.\n", (long)getpid(), x);
    }
    if(isParent)
    {
        printf("This is the end for parent %ld.\n", (long)getpid());
    }
    else
    {
        printf("This is the end for child %ld.\n", (long)getpid());
    }
    return 0;
}
