#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  unsigned char str;
  cin>>str;
  unsigned int tmpType = str;
  switch(tmpType)
  {
  case 1:
      cout<<"match 1"<<endl;
      break;
  case 2:
      cout<<"match 2"<<endl;
      break;
  default:
      cout<<"match none."<<endl;
  }
  if (str == 1)
    cout<<"find 1"<<endl;
  else if (str == 2)
    cout<<"find 2"<<endl;
  else
    cout<<"find none"<<endl;
  return 0;
}
