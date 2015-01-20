#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    const string& interfaceSide = "Line H"; 
    string dwdmSide, portOrLine;
    int pos = interfaceSide.find_last_of("ABCDEFGH");
    if (pos != string::npos)
    {
        dwdmSide = interfaceSide.substr(pos, 1);
    }
    cout<<"*******>"<<pos<<endl;
    cout<<"------->"<<dwdmSide<<endl;
    return 0;
}
