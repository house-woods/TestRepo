#include <iostream>
#include <stdio.h>


using namespace std;

void main(int argc, char* argv[])
{
    char buff[20];
    cout<<"123"<<endl; 
    sprintf(buff, "%x", 0);
    cout<<buff<<endl;

    char buff01[20];
    cout<<"0"<<endl;
    sprintf(buff01,"%.1X", 5);
    cout<<buff01<<endl;

    char buf02[20];
    cout<<"123"<<endl;
    sprintf(buf02, "%.1d" , 123);
    cout<<buf02<<endl;

    return;
}
