#include <iostream>
using namespace std;

int _tmain()
{
    int ni = 50;
    unsigned int ui = 100;
    
    printf("%d\n", ni-ui);
    printf("%d\n", (ni-ui)*2);
    printf("%d\n", (ni-ui)/2);
    printf("%d\n", (ni-(int)ui)/2);
    int tmp = (unsigned)0xFFFFFFE7>>1;
    printf("%d\n", tmp);
    long long lltmp = 0xefffffff;
    long ltmp = 0xefffffff;
    cout<<"long long"<<lltmp<<endl;
    cout<<"long"<<ltmp<<endl;
    return 0;
};

void test()
{
unsigned int ui = 1;
int ni = 2;

if((ui - ni)<0){
printf("should not see me.\n\r");
}	
else 
{
  printf("unsigned.\n");
}
};
int main(int argc,char* argv[])
{
  long lhigh = 0x000000e0;
  long llow = 0x10000001;
  long long l64 = 0x000000e010000001;
  _tmain();
  test();
  return 0;
}
