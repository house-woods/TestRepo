#include <iostream>
#include <string>
using namespace std;
void main(int argc, char* argv[])
{
/*
    string str("hello");
    str = str.substr(5);
    cout<<str<<endl;
    if (str.empty())
       cout<<"empty"<<endl;
    else
       cout<<"not empty"<<endl;
    string str1 = 2>1?"hello":"world";
    
    cout<<str1<<endl;
*/
  string v1("");
  string v2("a");
  cout<<v1.compare(v2)<<endl;
  string::size_type pos = v1.find_first_not_of("0123456789."); 
  cout<<pos<<endl;
  cout<<atoi("")<<endl;
}
