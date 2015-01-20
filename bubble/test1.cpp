#include <iostream>

using namespace std;

class A
{
public:
  void output(){
    cout<<"class A"<<endl;
  }
protected:
  void outputA1() {
    cout<<""<<endl;
  }; 
private:
  void outputA2() {
    cout<<""<<endl;
  }; 
};
class B : private A
{
public:
  void outputb(){
    cout<<"class B"<<endl;
    output();
    outputA1();
    outputA2();
  };
};
int main(int argc, char* argv[])
{
  B obj;
  obj.outputb();
  return 0;
}
