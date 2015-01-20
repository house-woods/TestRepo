#include <string>
#include <iostream>

using namespace std;

class B
{
public:
  B(){cout<<"B()"<<endl;};
  virtual ~B(){cout<<"~B()"<<endl;}; 
  void out1()
       {
         cout<<"B::out1"<<endl;
         p1();
       };
  void out2()
       {
         cout<<"B::out2"<<endl;
         p2(); 
       };
  void out3()
       {
         cout<<"B::out3"<<endl;
         p3(); 
       };
  void p3(){cout<<"B::p3"<<endl;};
private:
  virtual void p1(){cout<<"B:p1"<<endl;};
  void p2(){cout<<"B::p2"<<endl;};
};
class D:public B
{
public:
  D(){cout<<"D()"<<endl;};
  ~D(){cout<<"~D()"<<endl;};
  void p3(){cout<<"D::p3"<<endl;};
private:
  virtual void p1(){cout<<"D:p1"<<endl;};
  void p2(){cout<<"D::p2"<<endl;};
};

int main(int argc, char* argv[])
{
  cout<<"1)"<<endl;
  B * ptr = new D();
  ptr->out1();
  cout<<"2)"<<endl;
  ptr->out2();
  cout<<"3)"<<endl;
  ptr->out3();
  delete ptr;
  return 0;
}
