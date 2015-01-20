#include <iostream>
#include <string>

using namespace std;
class A
{
public:
   int func(int arg1,int arg2,int arg3,int arg4,string arg5)
   {
       int i = 10/arg1;
       return arg1;
   };
};
void main(int argc, char* argv[])
{
    int temp = 1;
    int temp1 = 1;
    int temp2 = 2;
    int temp3 = 3;
    string temp4("hello");

    A aobj;
    aobj.func(temp,temp1,temp2,temp3,temp4); 
    return;
}
