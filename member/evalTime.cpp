#include <iostream>
#include <time.h>
#include <unistd.h>
#define MAXLOOP 100000000
using namespace std;
int
timeval_subtract (timeval *result, timeval *x, timeval *y)
{
/* Perform the carry for the later subtraction by updating y. */
if (x->tv_usec < y->tv_usec) {
int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
y->tv_usec -= 1000000 * nsec;
y->tv_sec += nsec;
}
if (x->tv_usec - y->tv_usec > 1000000) {
int nsec = (x->tv_usec - y->tv_usec) / 1000000;
y->tv_usec += 1000000 * nsec;
y->tv_sec -= nsec;
}
/* Compute the time remaining to wait.
tv_usec is certainly positive. */
result->tv_sec = x->tv_sec - y->tv_sec;
result->tv_usec = x->tv_usec - y->tv_usec;
/* Return 1 if result is negative. */
return x->tv_sec < y->tv_sec;
};

class TestMember
{
public:
    timeval evalReadWithTemp();
    timeval evalReadWithMember();
    int _elm;
};
timeval TestMember::evalReadWithMember()
{
    timeval start, end;
    timeval res; 
    int ret;
    ret = gettimeofday(&start,NULL);  
    for(int i = 0; i < MAXLOOP; i++)
    {
        _elm;
    }
    ret = gettimeofday(&end, NULL);
    ret = timeval_subtract(&res, &end, &start);

    return res; 
};
timeval TestMember::evalReadWithTemp()
{
    timeval start, end;
    timeval res;
    int ret = 0;
    int iTemp = _elm;
    ret = gettimeofday(&start, NULL);
    for(int i = 0; i < MAXLOOP; i++)
    {
        _elm;
    }
    ret = gettimeofday(&end, NULL);
    ret = timeval_subtract(&res, &end, &start);
    return res;
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
    //timeval timeElapsed0 = aObj.evalReadWithMember();
    //long int time0 = timeElapsed0.tv_sec * 1000000 + timeElapsed0.tv_usec;
    //cout<<"Time elapsed when reading with member:"<<time0<<"(microseconds)"<<endl;
    timeval timeElapsed1 = aObj.evalReadWithTemp();
    long int time1 = timeElapsed1.tv_sec * 1000000 + timeElapsed1.tv_usec;
    cout<<"Time elapsed when reading with temporary variable:"<<time1<<"(microseconds)"<<endl;

    timeval timeElapsed0 = aObj.evalReadWithMember();
    long int time0 = timeElapsed0.tv_sec * 1000000 + timeElapsed0.tv_usec;
    cout<<"Time elapsed when reading with member:"<<time0<<"(microseconds)"<<endl;


    return 0;
}



