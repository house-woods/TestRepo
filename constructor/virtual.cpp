#include <iostream>

using namespace std;

class CBase
{
public:
    CBase()
    {
        m_data=10;
        cout<<"CBase()."<<endl;
    };
    virtual ~CBase()
    {
        cout<<"~CBase()."<<endl;
    };
    virtual int getVal()
    {
        return m_data;
    };
private:
    int m_data;
};

class CDerived : public CBase
{
public:
    CDerived()
    {
        _da0 = 20;
        cout<<"CDerived()."<<endl;
    }
    ~CDerived()
    {
        cout<<"~CDerived()."<<endl;
    }
    virtual int getVal()
    {
        return _da0;
    }
private:
    int _da0;
};

class CDe : public CBase
{
public:
    CDe()
    {
        _da1 = 30;
        cout<<"CDe()."<<endl;
    }
    ~CDe()
    {
        cout<<"~CDe()."<<endl;
    }
    virtual int getVal()
    {
        return _da1;
    }
private:
    int _da1;
};

int main(int argc, char* argv[])
{
    CBase * ptr = new CDerived;
    cout<<"val:"<<ptr->getVal()<<endl;
    cout<<"==============================="<<endl;
    delete ptr;
    int i;
    cin>>i;
    return 0;
}
