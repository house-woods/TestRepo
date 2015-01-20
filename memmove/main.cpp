#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
  char tmp[10] = "123456789";
  char* tmp1 = tmp + 3;
  cout<<tmp<<endl;
  cout<<tmp1<<endl;
  cout<<strlen(tmp1)<<endl;
  //memmove(tmp,tmp1,strlen(tmp1));
  strcpy(tmp,tmp1); 
  cout<<tmp<<endl;
  cout<<tmp1<<endl;
  return 0;
}
