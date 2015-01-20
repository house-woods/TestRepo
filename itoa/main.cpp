#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

using namespace std;
char * itoa ( int value , char * string , int radix )
{
    sprintf(string, "%d", value);
    return string;
}

void main(int argc, char* argv[])
{
    int num = 1511111;
    char severity[5];
    itoa(num,severity,10);
    string str("Output:"); 
    str.append(severity);
    cout<<str<<endl;
    return;
}
