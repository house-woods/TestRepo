#include <iostream>
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
//
using namespace std;
#define SIZE_DATA 18
#define SIZE_BUF 1024
#define NO_ITERATIONS 5

int main (int argc, char *argv[])
{
  std::cout<<"Usage "<<argv[0]<<" <port_num>"<<endl;
  ACE_OS::exit(1);
}
