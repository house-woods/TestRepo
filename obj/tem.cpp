#include <iostream>
using namespace std;

template <typename T>
void mpr(const T& a) 
{
    cout<<"value:"<<a<<endl;
}

int main(int argc, char* argv[])
{
    int itemp = 10;
    string strTemp = "Hello";
    mpr(itemp);
    mpr(strTemp);
    return 0;
}
