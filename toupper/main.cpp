#include <string>
#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
  string str("AC 1D 14 3D 33 7e"); 
  string str1("AC 1D 14 3D 33 7E");
  if(0 == str1.compare(toupper(str.c_str()))) 
  {
    cout<<toupper(str.c_str())<<endl;
  }
  return 0;
}

