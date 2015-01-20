#include <iostream>
#include <string>
using namespace std;
int main(int argc, char* argv[])
{
  string temp("hello");
  cout<<temp.substr(0,10)<<endl;
  cout<<temp.substr(0,5)<<endl;
  cout<<temp.substr(0,3)<<endl;
  return 0;
}
