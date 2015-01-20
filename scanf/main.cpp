#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
    int itemp;
    char strtmp[10];
    strtmp[9]='\0'; 
    printf("please input:\n");
    scanf("%i",&itemp); 
    cout<<itemp<<endl;
    printf("-------------------------");
    printf("please input:\n");
    scanf("%s",&strtmp);
    printf("%s\n",strtmp); 
    return 0;
}


