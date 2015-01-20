#include <iostream>
#include <string>
using namespace std;

char * itoa ( int value , char * string , int radix )
{
    sprintf(string, "%d", value);
    return string;
}

int main(int argc, char* argv[])
{
/*  char a = 'a';
  string b = "string=";
  string c(1,a);
  b.append(c);
  cout<<b<<endl;
*/
  for(int port = 1; port <= 105; port ++)
  {
    int colNum = (port-1)/5 + 1;
    char colAlp = 'V' + (port-1)%5;
    char buffer[5];
    itoa(colNum,buffer,10);
    string rbPort = string(buffer) + string(1,colAlp);
    cout<<"rbPort["<<port+1<<"]="<<rbPort<<endl;
  }
  return 0;
}
