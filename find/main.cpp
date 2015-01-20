#include <iostream>
#include <string>

using namespace std;

void main()
{
  string str("OTS");
  //int pos = str.find("NGX");
  if (str.find("NGX") != string::npos)
  {
    cout<<"gotcha."<<endl;
  }
}
