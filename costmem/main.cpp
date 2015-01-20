#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
char* a = new char[4000000000];
char ret = '0';
while (ret != 'q')
{
  cin>>ret;
  cout<<ret<<endl;
}
return 0;
}
