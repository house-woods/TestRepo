#include <stdio.h>
#define M 1900
#define N 1900
int main( void )
{
int a1[M][N];
int i,j;
for(i=0;i<M;i++)
for(j=0;j<N;j++)
{
a1[i][j]=N*i+j+1;
printf("%d ",a1[i][j]);
if(j==N-1)
printf("\n");
}
return 0;
}
