#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  string str1;
  const string TYPE = "HELLO";
  if (str1 == TYPE)
  {
    cout<<"Equal."<<endl;
  }
  else
  {
    cout<<"Not Equal"<<endl;
  }
  return 0;
}
