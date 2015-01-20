#include <iostream>

using namespace std;

class A
{
public:
  virtual void fun()=0;
};
class B : public A
{
public:
  void fun(){cout<<"B::fun()"<<endl;};
};
class C : public B
{
  //void fun(){return;};
};
int main(int argc, char* argv[])
{
  C obj;
  obj.fun();
  int i;
  cin>>i;
  return 0;
}
