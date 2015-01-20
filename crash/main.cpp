#include <string>
#include <iostream>

using namespace std;

class A{
public:
   A(){};
   ~A(){}; 
   void fun(){cout<<"hello"<<endl;};
};

int main(int argc, char* argv[])
{
  A* ptr = new A;
  delete ptr;
  ptr = NULL;
  delete ptr;
  A* ptr1 = new A;
  delete ptr;
  ptr->fun();
  return 0;
}
