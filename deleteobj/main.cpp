#include <iostream>
#include <string>
#include <list>

using namespace std;

class Tool
{
public:
  Tool():_desc("hello"){};
  ~Tool()
  {_desc="world!";};
  string foo()
  {
    return _desc;
  };
  string _desc;
};
int main(int argc, char* argv[])
{
  list<Tool*> toolList;
  Tool* ptrObj = new Tool();
  toolList.push_back(ptrObj);
  string tmp = ptrObj->foo();
  cout<<tmp<<endl;
  for(list<Tool*>::iterator it = toolList.begin(); it != toolList.end(); it++)
  {
    delete *it;
    *it = NULL;
    toolList.erase(it);
  }
  //delete ptrObj;
  //ptrObj = NULL;
  tmp = ptrObj->foo();
  cout<<tmp<<endl;
  return 0;
}

