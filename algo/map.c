//#include <iostream>
//#include <map>
//#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

//using namespace std;

int main(int argc, char* argv[])
{
  /*
  map<string,string> tmpM;
  tmpM.insert(pair<string,string>("1","hello"));
  cout<<tmpM["1"]<<endl;
  string col1 = "name";
  string col2 = "desc";
  string sql = "SELECT " + col1 + ", " + col2 + " FROM t1 WHERE ID=:1";
  cout<<sql<<endl;
  sql = "UPDATE SHELF_CM_VW SET " + col1 + " =:1, " + col2 + " =:2 WHERE ID = :3";
  cout<<sql<<endl;
  */
  char chan[800];
  for(int i=1; i <= 88; i++)
  {
    if (i != 1)
    {
      chan = strcat( chan, "&" );
    }
    char tmp[3];
    chan = strcat(chan,itoa(i,tmp,10)); 
  }
  printf ("out: %s\n",chan);

  return 0;
}
