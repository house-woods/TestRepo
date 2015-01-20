#include <iostream>
#include <time.h>
#include <unistd.h>
#define MAXLOOP 100000000
using namespace std;
class TestMember
{
public:
    double evalReadWithMember();
    double evalReadWithTemp();
    int _elm;
};
double TestMember::evalReadWithMember()
{
    time_t start, end;
    time_t * res = NULL; 
    start = time(res);  
    cout<<(double)start<<endl;
    for(int i = 0; i < MAXLOOP; i++)
    {
        if(_elm > 0)
        {
            //do nothing
        }
    }
    end = time(res);
    cout<<(double)end<<endl;
    double retVal = difftime(end, start);
    return retVal; 
};
double TestMember::evalReadWithTemp()
{
    time_t start, end;
    time_t * res = NULL;
    int iTemp = _elm;
    start = time(res);
    cout<<(double)start<<endl;
    for(int i = 0; i < MAXLOOP; i++)
    {
        if(iTemp > 0)
        {
            //do nothing
        }
    }
    end = time(res);
    cout<<(double)end<<endl;
    double retVal = difftime(end, start);
    return retVal;
};
int main(int argc, char *argv[])
{
/*    clock_t startWithMember, endWithMember, startWithoutM, endWithM;
    startWithMember = clock();
    cout<<startWithMember<<endl;
    sleep(10);
    endWithMember = clock();
    cout<<endWithMember<<endl;
    cout<<(double)(endWithMember - startWithMember)<<endl;
*/
    TestMember aObj;
    double timeElapsed0 = aObj.evalReadWithMember();
    cout<<"Time elapsed when reading with member:"<<timeElapsed0<<endl;
    double timeElapsed1 = aObj.evalReadWithTemp();
    cout<<"Time elapsed when reading with temporary variable:"<<timeElapsed1<<endl;
    return 0;
}



