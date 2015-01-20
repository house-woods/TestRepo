#include <iostream>
#include "ace/ACE.h"
#include "ace/Thread_Manager.h"
using namespace std;

void* run1(void* args)
{
  cout<<"Thread1"<<endl;
  return 0;
};

void* run2(void* args)
{
  cout<<"Thread2"<<endl;
  return 0;
};

void* run3(void* args)
{
  cout<<"Thread3"<<endl;
  return 0;
};

int main(int argc, char* argv[])
{
  cout<<"start"<<endl;
/*  ACE_Thread_Manager::instance()->spawn(run1);
  ACE_Thread_Manager::instance()->spawn(run2);*/
  ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)run3); 
  cout<<"End"<<endl;
  return 0;
}

