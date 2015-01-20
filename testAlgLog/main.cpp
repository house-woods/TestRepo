#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  bool bres = true;
  int i = 10;
  cout<<"i="<<i<<endl;
  if (bres && i < 10)
  {
    cout<<"i<10"<<endl; 
  }
  else
  {
    cout<<"i>=10"<<endl;
  }
  return 0;
}
