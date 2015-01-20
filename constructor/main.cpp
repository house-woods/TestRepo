#include <iostream>

using namespace std;


class CBase
{
public:
    CBase()
    {
        cout<<"CBase()."<<endl;
    }
    ~CBase()
    {
        cout<<"~CBase()"<<endl;
    }
};

class CDerived : public CBase
{
public:
    CDerived()
    {
        cout<<"CDerived()."<<endl;
    }
    ~CDerived()
    {
        cout<<"~CDerived()."<<endl;
    }
};
int main(int argc, char* argv[])
{
    {
        CDerived aobj;   
    }
    string a = "hello";
    const char * ptr = "hello";
    int i;
    cin>>i;
    return 0; 
}
