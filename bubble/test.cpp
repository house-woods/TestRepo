#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
   int a[5] = {1,2,3,4,5};
   cout<<sizeof(a)<<endl;
   int *ptr = a;
   cout<<sizeof(ptr)<<endl;
   cout<<sizeof(*ptr)<<endl;
   return 0;
}
