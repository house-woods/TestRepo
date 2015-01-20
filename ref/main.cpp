#include <iostream>
#include <map>
#include <string>

using namespace std;
void func(int* cnt)
{
  cout<<"input cnt="<<*cnt<<endl;
  (*cnt)++;
}; 
int main(int argc, char* argv[])
{
  int cnt = 0;
  func(&cnt);
  cout<<"After func, cnt="<<cnt<<endl;
  map<string, bool> testmap;
  map<string, bool>* tmptr = &testmap;
  tmptr->insert(std::pair<string,bool>(string("hello"),false));
  for(map<string, bool>::iterator it = testmap.begin(); it != testmap.end(); it++)
  {
    cout<<"first"<<it->first<<endl;
    //count<<"second"<<it->second<<endl;
  } 
  return 0;
}
