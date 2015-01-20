#include <iostream>

#include <string>

using namespace std;

void main(int argc, char* argv[])
{
  string str1("123456789");
  str1.replace(0,4,"ODU2");
  cout<<str1<<endl;
  unsigned char id = '6';
  switch (id)
  {
  case 62:
    cout<<id<<endl;
    break;
  case 6:
    cout<<id<<endl;
    break;
  default:
    cout<<"default."<<endl;
    ;
  }
  if (id == 6)
    cout<<"this is 6."<<endl;
  return;
}
