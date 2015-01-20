#include <iostream>

using namespace std;
void switchElm(int* a, int* b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
};
void output(int* a, int cnt)
{
  for (int i = 0; i < cnt; i++)
  {
    cout<<"Elm["<<i<<"]="<<a[i]<<endl;
  }
};
void bubble()
{
  int a[] = {88,111,2,60,983,1,13,66,5555,98,45};
  int cnt = sizeof(a)/sizeof(int);
  for (int i = 0; i < cnt; i++)
  {
    bool sw = false;
    for(int j = 0; j < cnt - i - 1; j++)
    {
      if (a[j] > a[j+1])
      {
        switchElm(&a[j],&a[j+1]);
        sw = true;
      } 
    }
    if (!sw)
    {
      break;
    }
  }
  output(a,cnt);
}
int main(int argc, char* argv[])
{
  bubble();
}
