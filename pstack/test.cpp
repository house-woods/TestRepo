#include <iostream>
#include <string>

using namespace std;

class A
{
public:
  A()
  {
    _val = 0;
    cout<<"hello world!"<<endl;
    cout<<_val<<endl;
  };
  A operator ++()
  {
    cout<<"pre ++"<<endl;
    ++_val;
    return *this;
  };
  A operator ++(int i)
  {
    A temp(*this);
    ++(*this);
    return temp;
  };
  int _val;
};
void main(int argc, char* argv[])
{
/*  const char * chr = "hello world";
  cout<<"length of string="<<strlen(chr)<<endl;
  A aobj;
  char a = 'a';
  cout<<int(a)<<endl;
  char b = 'A';
  cout<<int(b)<<endl;
  cout<<7/2<<endl;
  cout<<(aobj++)._val<<endl;
  cout<<aobj._val<<endl;
  cout<<(++aobj)._val<<endl;
*/
  int cnt = 0;
  string fe("abc#2");
  string infe("abc");
  if (fe.length() > infe.length() + 1)
  {
    cnt = atoi(fe.substr(infe.length() + 1,fe.length()-infe.length()-1).c_str());
  }
  cout<<cnt<<endl;
  return;
}
