#include <iostream>
#include <string>

using namespace std;
bool getRes(bool inp)
{
    if(inp)
    {
      cout<<"is true."<<endl;
    }
    else
    {
      cout<<"is false."<<endl;
    }
    return inp;
};
int main(int argc, char* argv[])
{
    bool flag = getRes(false)?true:getRes(true);
    if(flag)
      cout<<"flag is true."<<endl;
    else
      cout<<"flag is false."<<endl;
    return 0;
}
