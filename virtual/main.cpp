#include <iostream>
#include <string>

using namespace std;
class A
{
public:
    A()
    {
        cout<<"A()."<<endl;
    };
    virtual ~A()
    {
        cout<<"~A()."<<endl;
    };
    virtual void pr()
    {
        cout<<"A::pr()."<<endl;
    };
};
class B : public A
{
public:
    B()
    {
        cout<<"B()."<<endl;
    };
    ~B()
    {
        cout<<"~B()."<<endl;
    };
    void pr()
    {
        cout<<"B::pr()."<<endl;
    };
};
int main(int argc, char* argv[])
{
    A* ptr = new B();
    ptr->pr();
    delete ptr;
    return 0;
}

