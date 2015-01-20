#include <iostream>
#include <string>

using namespace std;
void Func(char str[100])
{
    cout<<"sizeof(str)="<<sizeof(str)<<endl;
}
int main(int argc, char* argv[])
{
   char str[] = "Hello";
   char *p = str;
   int n = 10;

  cout<<"sizeof(str)="<<sizeof(str)<<endl;
  cout<<"sizeof(p)="<<sizeof(p)<<endl;
  cout<<"sizeof(n)="<<sizeof(n)<<endl; 
  cout<<"***********************"<<endl;
  char test[100];
  Func(test);
  void *pp = malloc(100);
  cout<<"sizeof(pp)="<<sizeof(pp)<<endl; 
  return 0;
}
