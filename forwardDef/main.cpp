#include <iostream>
#include <string>
using namespace std;

namespace n1 {
class B;
class A
{
public:
  void fun()
  {
    cout<<"A::fun()"<<i<<endl;
    i = 10;
  };
  B* obj;
  static int i;
  int j;
};
int A::i = 0;
class B
{
public:
  void fun(){cout<<"B::fun()"<<endl;};
};
}
namespace n2 {
class B;
class A
{
public:
  void fun(){cout<<"A::fun()"<<endl;};
  B* obj;
};
class B
{
public:
  void fun(){cout<<"B::fun()"<<endl;};
};
}
using namespace n1;
using namespace n2;
int main(int argc, char* argv[])
{
  n1::A obj;
  obj.fun();
  obj.fun();
  return 0;
}
