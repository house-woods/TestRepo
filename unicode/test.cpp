#include <iostream>
#include <string>
using namespace std;

void GetMemory(char **p, int num){
  *p = (char *)malloc(num);
};

void Test(void){
  char *str = NULL;
  GetMemory(&str, 100);
  strcpy(str, "hello");
  printf(str);
  cout<<endl; 
};

int main(int argc, char* argv[])
{
    Test();
}
