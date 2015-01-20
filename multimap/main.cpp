#include <iostream>
#include <string>
#include <map>

using namespace std;

void func()
{
  static bool flag = false;
  static int cnt = 1;
  if(!flag)
  {
    flag = true;
    cout<<"enter"<<cnt++<<endl;
  }
}
int main(int argc, char* argv[])
{
  func();
  static multimap<string, string> cpTypeMap;
  cpTypeMap.insert(pair<const string, string>("OADM", "OPTICAL ADD DROP MULTIPLEXER"));
  cpTypeMap.insert(pair<const string, string>("OADM", "OPTICAL ADD DROP MULTIPLEXER"));
  cpTypeMap.insert(pair<const string, string>("OSM40", "OTU SWITCHING MODULE"));
  for (multimap<string,string>::iterator it = cpTypeMap.begin();it != cpTypeMap.end();it++)
  {
     cout<<it->first<<":="<<it->second<<endl;
  }
  func();
  
  return 0;    
}

