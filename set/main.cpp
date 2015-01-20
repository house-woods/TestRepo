#include <iostream>
#include <string>
#include <set>

using namespace std;

void main(int argc, char* argv[])
{
    set<string> idSet;
    idSet.insert("hello");
    idSet.insert("world");
    idSet.insert("hello");
    idSet.insert("hello");
    idSet.insert("hello1");
    int i = 0;
    for(set<string>::const_iterator it = idSet.begin(); it != idSet.end(); it++, i++)
    {
      cout<<"Item "<<i<<"="<<*it<<endl;
    }
    return;
}
