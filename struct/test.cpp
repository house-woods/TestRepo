#include <iostream>
using namespace std;
typedef struct
{
    unsigned char id;
    unsigned char validity;
    long value;
    long long val64;
}
PM_PARAM_DATA;

int main()
{
  PM_PARAM_DATA data;
  data.id=0x3e;
  data.validity=0x02;
  data.value=0x07;
  PM_PARAM_DATA* ptrPm = &data;
  cout<<ptrPm->id<<endl;
  cout<<ptrPm->validity<<endl;
  cout<<ptrPm->value<<endl;
  return 0;
}
