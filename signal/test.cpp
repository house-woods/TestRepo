#include <stdio.h>
#include <stdlib.h>
    
static void sub2(int *p)
{
  int i;
    
  i = *p;
}
    
static void sub(int *p)
{
  sub2(p);
}
    
int main()
{
  int *p=NULL;
    
  int num;
  scanf("%d",num);
  sub(p);
  //int num;
  //scanf("%d",num);
  return 0;
}
