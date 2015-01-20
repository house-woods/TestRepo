
/* va_arg example */
#include <stdio.h>
#include <stdarg.h>
typedef struct
{
int i;
char b;
} typeStruct;

/*int FindMax ( int amount, ...)
{
  int i,val,greater;
  va_list vl;
  va_start(vl,amount);
  greater=va_arg(vl,int);
  printf("The original one is %d.\n", greater); 
  for (i=1;i<amount-1;i++)
  {
    val=va_arg(vl,int);
    printf("The %dth value is:%d\n", i, val);
    greater=(greater>val)?greater:val;
  }
  const char* str = va_arg(vl, const char*);
  printf("The arg string is:%s\n", str);
  va_end(vl);
  return greater;
}*/
int FindMax(int amout, ...)
{
 va_list vl;
 va_start(vl, amount);
 typeStruct temp = va_arg(vl, typeStruct);   
 printf("User defined type:i=%d,b=%c.", temp.i, temp.b); 
 return 0;
}
int main ()
{
  int m;
//  m= FindMax (8,702,422,631,834,892,104,772,"hello world.");
  typeStruct utypes; 
  utypes.i = 10;
  utypes.b = 'A';
  m = FindMax(1,utypes);
  printf ("The greatest one is: %d\n",m);
  return 0;
}

