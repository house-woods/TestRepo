#include "mys.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv)
{
  MYST obj;
  //MYST::lane tlane;
  obj._la.resize(2);
  obj._la[0].laneno = "1"; 
  obj._la[0].optpwr = "(+10)";
  obj._la.resize(1);
  cout<<obj._la[0].laneno<<"->"<<obj._la[0].optpwr<<endl;
  cout<<"_la size="<<obj._la.size()<<endl;
  return 0;
}
