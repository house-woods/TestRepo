#include <iostream>

using namespace std;

void main(int argc, char* argv[])
{
  for(int i = 0;i < 3;i++)
  {
    for(int j = 0;j < 3; j++)
    {
       cout<<i<<","<<j<<endl;
       if (1 == j)
       {
         break;
       }
    }
    break;
  }
}
