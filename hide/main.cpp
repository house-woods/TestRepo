#include <iostream>
#include <string>

using namespace std;

class A
{
public:
void fun()
{
  cout<<"A::fun()."<<endl;
};
};
class B : public A
{
public:
void fun()
{
  cout<<"B::fun()."<<endl;
};  
};
void main(int argc, char* argv[])
{
  B obj;
  obj.fun();
  return;
}
