#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
  wchar_t* buf = L"\x9f";
  wcout<<buf<<endl;
  return 0;
}
