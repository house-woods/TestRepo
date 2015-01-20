#include <iostream>

using namespace std;

class A
{
public:
  A() {_val=10;};
  void out(){cout<<_val<<endl;};
private:
  int _val;
};
int main(int argc, char* argv[])
{
  A* ptr = new A(); 
  delete ptr;
  delete ptr;
  delete ptr;
  delete ptr;
  ptr->out();
  A* ptr1 = new A();
  ptr1->out();
  int a[2];
  a[3]=10;
  a[4]=11;
  for (int i=0;i<30;i++)
  {
    a[i]=i+1;
  }
  for (int i=0;i<30;i++)
  {
    cout<<a[i];
  }
  int b[5];
  for (int i=0;i<30;i++)
  {
    b[i] = i;
  }
  for (int i=0;i<30;i++)
  {
    cout<<b[i]<<endl;
  }
  

  return 0;
}
