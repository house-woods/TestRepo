#include<stdio.h>
#include<dirent.h>
struct dirent *dptr;
int main(int argc, char *argv[])
{
    char buff[256];
    DIR *dirp;
    printf("\n\n Enter directory Name");
    scanf("%s",buff);
    if((dirp=opendir(buff))==NULL)
    {
        printf("Error");
        //exit(1);
        return -1;
    }
    int i = 1;
    while(dptr=readdir(dirp))
    {
	printf("------------%i-------------\n", i);
        printf("d_name->%s\n",dptr->d_name);
	printf("d_ino->%i\n",dptr->d_ino);
	printf("d_doff->%i\n",dptr->d_off);
	printf("d_reclen->%i\n",dptr->d_reclen);
	printf("----------end of %i------------\n", i++);
    }
    closedir(dirp);
}

