#include <iostream> 
#include <string>


using namespace std;

void main(int argc, char* argv[])
{
    string str("a\n");
    cout<<str.length()<<endl;
    cout<<"-----------"<<endl;
    cout<<str.substr(0,1)<<endl;
    cout<<"*************"<<str.substr(1,1)<<endl;
    cout<<"*************"<<endl;
    string tmp = "11111111111111111111111111111/n111111111111111111/n";
    cout<<tmp.length()<<endl;
}
