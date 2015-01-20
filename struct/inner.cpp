#include <iostream>
using namespace std;

struct A
{
public:
        struct B1
        {
                public: int a;
                private: int b;
                public: void foo(A &p) { 
                                cout << p.i1 << endl;      // OK, because i1 is public in class A
                                //cout << p.i2 << endl;      // Fail, because i2 is private in class A
                                }
        };

private:
        class B2
        {
                public: int a;
                private: int b;
                public: void foo(A &p) {
                                cout << p.i1 << endl;      // OK, because i1 is public in class A
                                //cout << p.i2 << endl;      // Fail, because i2 is private in class A
                                }
        };

public:
        B1 b11;
        B2 b12;
        int i1;
private:
        B1 b21;
        B2 b22;
        int i2;
public: 
                void f(B1& p) { 
                cout << p.a << endl;      // OK, because a is public in class B1
                //cout << p.b << endl;      // Fail, because b is private in class B1
                }
                void f(B2& p) { 
                cout << p.a << endl;      // OK, because a is public in class B2
                //cout << p.b << endl;      // Fail, because b is private in class B2
                }
};

int main(int argc, char *argv[])
{
    A a ;
        A::B1 ab1;                              // OK, because B1 is declared as public inner class.
        //A::B2 ab2;                              // Fail, because B2 is declared as private inner class
    ab1.foo(a);
    return 0;
}
