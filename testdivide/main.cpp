#include <iostream>
#include <string>

using namespace std;

void main(int argc, char* argv[])
{
    int i = 8200;
    int j = 4000;
    cout<<i/j<<endl;
    cout<<i%j<<endl;
    cout<<8000%4000<<endl;
    int x = 0;
    x = (i%j?(i/j + 1):(i/j));
    cout<<x<<endl;
    i = 8000;
    j = 4000;
    x = (i%j?(i/j + 1):(i/j));
    cout<<x<<endl;
    string str("hello world!");
    string tmp = str.substr(0, 5);
    string tmp2 = str.substr(5,str.length());
    cout<<tmp<<endl;
    cout<<tmp2<<endl;
    cout<<str<<endl;
    int cnt = 10;
    int len = ( (9 == cnt - 1) ? 10 : 4000);
    cout<<len<<endl;
    cout<<"-------------------------"<<endl;
    string num("1010"); 
    int inum = atoi(num.substr(0,num.length()).c_str());
    cout<<inum<<endl;
}
