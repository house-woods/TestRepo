#include <string>
#include <iostream>
#include <map>
using namespace std;

int main(int argc, char* argv[])
{
    map<int, string> tmpMap;
    for(int i = 0; i < 20; i++)
    {
        tmpMap.insert(map<int,string>::value_type(i,"hello"));
    }
    for(map<int,string>::iterator it = tmpMap.begin(); it != tmpMap.end(); it++)
    {
        if (15 == (*it).first)
        {
            tmpMap.erase(it++); 
            //it = tmpMap.erase(it); 
        }
        cout<<(*it).first<<"->"<<(*it).second<<endl; 
    }
    int cat = 0;
    switch(cat) 
    {
    case 0:
        break;
    case 1:
        break;
    default:
        break; 
 
    }

    return 0;
}
