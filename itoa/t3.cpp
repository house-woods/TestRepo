#include <iostream>
#include <string>

using namespace std;
class A
{
public:
  A()
  {
    cout<<"A()."<<endl;
  }; 
  ~A()
  {
    cout<<"~A()."<<endl;
  };
};
class B : public A
{
public:
  B()
  {
    cout<<"B()."<<endl;
  };
  ~B()
  {
    cout<<"~B()."<<endl;
  };
};
int main(int argc, char* argv[])
{
  {
     B obj;
  }
  return 0;
}

