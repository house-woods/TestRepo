#include <list>
#include <iostream>
#include <string>
using namespace std;
class Elm
{
  public:
    bool operator<(const Elm& inP) const {
       return (_elm.length()<inP._elm.length());
    };
    string _elm; 
};
class T
{
public:
  void test(list<Elm>& inParam)
  {
      cout<<"Before sort."<<endl;
      display(inParam); 
      inParam.sort();
      cout<<"After sort."<<endl;
      display(inParam);
  };
  void display(list<Elm>& inParam)
  {
    int i = 0;
    for(list<Elm>::iterator it = inParam.begin(); it != inParam.end(); it++,i++)
    {
      cout<<"Item "<<i<<"="<<(*it)._elm<<endl;
    }
  };
};
int main(int argc, char* argv[])
{
  list<Elm> tList;
  Elm tmp;
  tmp._elm="12345"; 
  tList.push_back(tmp);
  tmp._elm="12";
  tList.push_back(tmp);
  tmp._elm="123";
  tList.push_back(tmp);
  T aobj;
  aobj.test(tList); 
  return 0;
}
