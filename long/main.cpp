#include <iostream>

using namespace std;
char * _ulltoa ( unsigned long long value , char * string , int radix )
{
    sprintf(string, "%lld", value);
    return string;
}
char * _lltoa ( long long value , char * string , int radix )
{
    sprintf(string, "%lld", value);
    return string;
}
char * _ulltox ( unsigned long long value , char * string , int radix )
{
    sprintf(string, "%x", value);
    return string;
}
void test1()
{
  long lt = 0xffffffff;
  long long llt = lt;
  cout<<"test1:signed long->"<<lt<<endl; 
  cout<<"test1:convert it to unsigned long->"<<(unsigned long)lt<<endl; 
  cout<<"test1:convert it to long long->"<<llt<<endl; 
  lt = 0xefffffff;
  llt = (unsigned long)lt;
  cout<<"test1:signed long->"<<lt<<endl; 
  cout<<"test1:convert it to unsigned long->"<<(unsigned long)lt<<endl; 
  cout<<"test1:convert it to long long from unsigned long->"<<llt<<endl; 
  lt = 0x7fffffff;
  llt = lt;
  cout<<"test1:signed long->"<<lt<<endl; 
  cout<<"test1:convert it to unsigned long->"<<(unsigned long)lt<<endl; 
  cout<<"test1:convert it to long long->"<<llt<<endl; 
  long long * ptrllt = &llt;
  *ptrllt = 0x07;
  cout<<"test1:pointer->"<<*ptrllt<<endl;
  cout<<"test1:original ll->"<<llt<<endl;
  
};
void test2()
{
  long long ult = 0xffffffffffffffff;
  cout<<"test2:long long->"<<ult<<endl; 
};
void test3()
{
  long long ult = 0xefffffffffffffff;
  cout<<"test3:signed long long->"<<ult<<endl; 
};
void test4()
{
  long long ult = 0xffffffffffffffff;
  char tmp[256];
  _lltoa(ult,tmp,10);
  cout<<"test4:unsigned long long->       "<<ult<<endl; 
  cout<<"test4:unsigned long long string->"<<tmp<<endl; 
  ult = 0xefffffffffffffff;
  _lltoa(ult,tmp,10);
  cout<<"test4:unsigned long long->       "<<ult<<endl; 
  cout<<"test4:unsigned long long string->"<<tmp<<endl; 
};
void test5()
{
  long ult_low = 0xfffffff0;
  long ult_high = 0xffffffff;
  long long ullt = ((long long)ult_high<<32)|(unsigned long)ult_low;
  char tmp[256];
  _lltoa(ullt,tmp,10);
  long long ullt1 = 0xfffffffffffffff0;
  cout<<"test5:unsigned long long->       "<<ullt1<<endl; 
  cout<<"test5:unsigned long long->       "<<ullt<<endl; 
  cout<<"test5:unsigned long long string->"<<tmp<<endl; 
  long lt_low = 0xfffffff0;
  long lt_high = 0xefffffff;
  ullt = ((long long)lt_high<<32)|(unsigned long)lt_low;
  _lltoa(ullt,tmp,10);
  ullt1 = 0xeffffffffffffff0;
  cout<<"test5:unsigned long long->       "<<ullt1<<endl; 
  cout<<"test5:unsigned long long->       "<<ullt<<endl; 
  cout<<"test5:unsigned long long string->"<<tmp<<endl; 
};
int main()
{
  test1();
  test2();
  test3();
  cout<<"***************************"<<endl;
  test4();
  test5();
  return 0;
}
