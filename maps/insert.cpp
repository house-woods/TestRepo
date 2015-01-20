#include <iostream>
#include <map>
#include <list>
#include <string>


using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        map<string,string> idMap;
        idMap.insert(pair<string,string>("1","a"));
        idMap.insert(pair<string,string>("1","b"));
        idMap.insert(pair<string,string>("1",""));
        idMap.insert(pair<string,string>("2","hello"));
        int i = 1;
        for(map<string,string>::iterator iter = idMap.begin();iter != idMap.end();iter++)
        {
            cout<<"Item "<<i<<":"<<endl;
            cout<<iter->first<<"->"<<iter->second<<endl;
            i++;
        }
        cout<<"----------------------------"<<endl;
        list<string> idList;
        idList.push_back("1");
        idList.push_back("1");
        idList.push_back("2");
        i = 1;
        for(list<string>::iterator iter = idList.begin();iter != idList.end();iter++)
        {
            cout<<"Item "<<i<<":"<<endl;
            cout<<*iter<<endl;
            i++;
        }
    } 
    catch(...)
    {
        cout<<"catch unknown exception."<<endl;
    }
    return 0;
}


