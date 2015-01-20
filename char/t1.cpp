#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
  string str("abcde");
  for (int i = 0; i < str.length(); i++)
  {
    cout<<"str["<<i<<"]="<<str[i]<<endl;
  }
  return 0;
}
