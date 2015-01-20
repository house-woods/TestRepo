#include <iostream>
#include <string>

using namespace std;
void mypr(string inStr)
{
    cout<<inStr<<endl;
}
void prDe(void)
{
    cout<<"------------------------------"<<endl;
}
int main(int argc, char* argv[])
{
    mypr("hello world!");
    prDe();
    int i = 0;
    cin>>i; 
    return 0;
}


