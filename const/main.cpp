#include <iostream>
#include <map>
#include <string>

using namespace std;
class A
{
public:
  void pr(){cout<<"A::pr()"<<endl;};
};

int main(int argc, char* argv[])
{
    map<string,string> idMap;
    //string tmp = idMap.begin()->first;
    if (idMap.begin() != idMap.end())
    {
        string tmp = idMap.begin()->first;
    }
    const A* ptr = new A();
    A* ptr1;
    ptr1 = ptr;
    return 0;
}
