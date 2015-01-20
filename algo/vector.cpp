#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    vector<string> vStr(88,"Y");
    for(vector<string>::iterator it = vStr.begin();it != vStr.end();it++)
    {
        cout<<*it<<endl;
    }
    return 0;
}
