#include <string>
#include <iostream>

using namespace std;

char * itoa ( int value , char * string , int radix )
{
    sprintf(string, "%d", value);
    return string;
}
char * _ulltoa ( unsigned long long value , char * string , int radix )
{
    sprintf(string, "%llu", value);
    return string;
}
void myfor(const char* format, ...)
{
  char buffer[256];
  va_list args;
  va_start (args, format);
  vsnprintf (buffer,255,format, args);
  cout<<buffer<<endl;
  perror (buffer);
  va_end (args);
}

int main(int argc, char* argv[])
{
  int i = 9223372036854775807;
  double j = 9223372036854775807;
  long long k = 9223372036854775808;
  unsigned long long l = 18446744073709551615;
  double m = 123456789012345;
  long long n = m;
  double m2 = 123456789012345678;
  long long n2 = m2;
  double m3 = 999999999999999;
  long long n3 = m3;
  double m4 = n2;
  long long n4 = m4;
  cout<<"i="<<i<<endl;
  cout<<"j="<<j<<endl;
  cout<<"k="<<k<<endl;
  cout<<"l="<<l<<endl;
  cout<<"m="<<m<<endl;
  cout<<"n="<<n<<endl;
  cout<<"m2="<<m2<<endl;
  cout<<"n2="<<n2<<endl;
  cout<<"m3="<<m3<<endl;
  cout<<"n3="<<n3<<endl;
  cout<<"m4="<<m4<<endl;
  cout<<"n4="<<n4<<endl;
  cout<<"*******************"<<endl;
  myfor("format long long as d: %d", n3); 
  myfor("format long long as d: %llu", n3); 
  char ullta1[128];
  _ulltoa(n3,ullta1,10);
  cout<<"after _ulltoa:"<<ullta1<<endl;
  char si[20];
  char sj[20];
  char sk[20];
  char sl[20];
  char sm[20];
  char sn[20];

  itoa(i,si,10);
  itoa(j,sj,10);
  itoa(k,sk,10);
  itoa(l,sl,10);
  itoa(m,sm,10);
  itoa(n,sn,10);
  
  //cout<<"i="<<si<<endl;
  //cout<<"j="<<sj<<endl;
  //cout<<"k="<<sk<<endl;
  //cout<<"l="<<sl<<endl;
  //cout<<"m="<<sm<<endl;
  //cout<<"n="<<sn<<endl;
  return 0;
}
