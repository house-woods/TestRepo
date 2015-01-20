#include <iostream>
#include <string>

using namespace std;
class Opt
{
public:
	Opt()
	{ 
		_elm = 10;
	};
	int _elm;
};
class O
{
public:
	virtual void pr() = 0;
};
void O::pr()
{
	{
		cout<<"This is pr() in class O."<<endl;
	}
};
class A : public O
{
public:
	A()
	{
		cout<<"A()"<<endl;
	};
	virtual ~A()
	{
		cout<<"~A()"<<endl;
	};
	virtual void f(int i)
	{
		cout<<"A::f(int)"<<i<<endl;
	};
	void f(double i)
	{
		cout<<"A::f(double)"<<i<<endl;
	};/*
	void pr()
	{};*/
};
class B : public A
{	
public:
	B()
	{
		cout<<"B()"<<endl;
	};
	~B()
	{
		cout<<"~B()"<<endl;
	};
	using A::f;
	/*void f(string i)
	{
		cout<<"B::f(string)"<<i<<endl;
	};*/	
	/*void f(int i)
	{
		cout<<"B::f(int)"<<i<<endl;
	};*/
	void f(Opt i)
	{
		cout<<"B::f(Opt)"<<i._elm<<endl;
	};
};
int main()
{
	{
		Opt obj;
		B objB;
		/*objB.f(obj);*/
		objB.f(20);
		objB.f(15.1);
	}
	cout<<"*************************************"<<endl;
	{
		A * ptr = new B();
		delete ptr;
	}
	//system("pause");
	return 0;
}
