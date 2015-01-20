#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  try
  {
  int* p1 = new int[10];
  delete[] p1;
  int* p2 = NULL;
  *p2 = 10;
  delete p2;
  delete p2;
  delete p2;
  }
  catch(std::exception ex)
  {
    cout<<"Excpetion:"<<ex.what()<<endl;
  }
  catch(...)
  {
    cout<<"Unknown exception"<<endl;
  }
  return 0;
}
