#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;
struct Functor
{
 bool operator()(string f, string s)
 {
     if(f.length() < s.length())
     {
       return true;
     }
     else
     {
       return false;
     }
 };
};

class T
{
public:
/*struct Functor
{
  bool operator()(string f, string s)
  {
      if(f.length() < s.length())
      {
        return true;
      }
      else
      {
        return false;
      }
  };
};*/
  void test(list<string>& inParam)
  {
      cout<<"Before sort."<<endl;
      display(inParam); 
      Functor f;
      inParam.sort(f);
      cout<<"After sort."<<endl;
      display(inParam);
  };
  void display(list<string>& inParam)
  {
    int i = 0;
    for(list<string>::iterator it = inParam.begin(); it != inParam.end(); it++,i++)
    {
      cout<<"Item "<<i<<"="<<*it<<endl;
    }
  };
};
int main(int argc, char* argv[])
{
  list<string> tList;
  tList.push_back("12345");
  tList.push_back("12");
  tList.push_back("123");
  T aobj;
  aobj.test(tList); 
  return 0;
}
