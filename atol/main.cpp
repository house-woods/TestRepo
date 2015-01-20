#include <iostream>

using namespace std;
long htol(const string &str)
{
   int len = str.length();
   int pos = 0;
   long val = 0;
   while(pos < len)
   {
       if(str[pos]>='0' && str[pos]<='9')
           val = val * 16 + str[pos] - 48;
       if(str[pos]>='a' && str[pos]<='f')
           val = val * 16 + str[pos] - 87;
       if(str[pos]>='A' && str[pos]<='F')
           val = val * 16 + str[pos] - 55;
       pos++;
   }
   return val;
}
void main(int argc, char* argv[])
{
    string str("00086001");
    long num = htol(str.c_str());
    cout<<num<<endl;
    cout<<hex<<num<<endl;
    if(num & 0x86001)
    {
       cout<<"& equal."<<endl;
    }
    if (num == 0x86001)
    {
       cout<<"== equal."<<endl; 
    }
    return;
}
