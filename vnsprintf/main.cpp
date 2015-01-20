#include <iostream>
#include <stdlib.h>

using namespace std;

void main(int argc, char* argv[])
{
  char tmp[3];
  char tmp1[4];
  strcpy(tmp1,"1234567890");
  strlen(tmp1);
  strlen(tmp);
  cout<<tmp<<endl;
  cout<<tmp1<<endl;
  tmp[0]='a';
  cout<<tmp<<endl;
  memset(tmp,200,3);
  strlen(argv[0]);
  strlen(argv[1]);
  if (0==strcmp("1",argv[1]))
  {
    strlen(tmp);
    strlen(NULL);
  }
  else
  {
    strlen(tmp);
    strlen(NULL);
  }
  return;
}
