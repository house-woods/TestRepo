// set_difference example
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
class A
{
private:
  static bool myfunction (string i,string j) 
  {
   return ( atoi(i.c_str())<atoi(j.c_str()) ); 
  };

public:
  int doit()
  {
  // {5,10,15,20,25};
  vector<string> aVec;
  aVec.push_back("5");
  aVec.push_back("10");
  aVec.push_back("15");
  aVec.push_back("20");
  aVec.push_back("25");


  //int second[] = {50,40,30,20,10};
  vector<string> bVec;
  bVec.push_back("50");
  bVec.push_back("40");
  bVec.push_back("30");
  bVec.push_back("20");
  bVec.push_back("10");

  vector<string> v(10);
  vector<string>::iterator it;

  sort (aVec.begin(),aVec.end(),myfunction);     //  5 10 15 20 25
  sort (bVec.begin(),bVec.end(),myfunction);   // 10 20 30 40 50

  it=set_difference (aVec.begin(), aVec.end(), bVec.begin(), bVec.end(), v.begin(), myfunction);
  // 5 15 25  0  0  0  0  0  0  0
  cout << "difference has " << int(it - v.begin()) << " elements.\n"<<"----"<<*it<<endl;
  v.erase(it,v.end());
  int cnt = 1;
  for(vector<string>::iterator iter = v.begin();iter != v.end();iter++)
  {
    cout<<"Item "<<cnt<<"="<<*iter<<endl;
    cnt++;
  }
  return 0;
  };
};
int main(int argc, char* argv[])
{
  A aobj;
  aobj.doit();
  return 0;
}
