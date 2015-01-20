#include <iostream>

using namespace std;

void func()
{
  func();
};
int main(int argc, char* argv[])
{
  func();
  return 0;
}
