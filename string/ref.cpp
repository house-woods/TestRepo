#include <string>
#include <iostream>

using namespace std;

class B
{
public:
  ~B(){cout<<"~B()"<<endl;};
};
class A 
{
public:
  A(){};
  A(const string& instr):str(instr){};
  string str;
};
int main(int argc, char* argv[])
{
  for (int i = 0; i < 10000000000; i++)
  {
    A obj;
    obj = A("This is the default result structure.");
  }
  cout<<"done!"<<endl;
  return 0;
}
