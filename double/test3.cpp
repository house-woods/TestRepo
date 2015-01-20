#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
double	d1	= 0.29;
double	d2	= d1 * 100.0;
long  l1	= d2;
long long l2	= d2;
cout<<"d2="<<d2<<endl;
cout<<"l1="<<l1<<endl;
cout<<"l2="<<l2<<endl;
long long l3 = 29;
if (l3 == d2)
{
  cout<<"equal"<<endl;
}
else
{
  cout<<"not equl"<<endl;
}
return 0;
}
