#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  int i = 0 ;
  while (i==0)
  {
     cout<<"out while."<<endl;
     int j = 0;
     while(j == 0)
     {
       int l = 1;
       cin>>l;
       if (0 == l)
       {
         j = 1;
         break;
       }
     }
  }
}
