#include <iostream>

using namespace std;



class singleton
{
private:
	singleton()
	{
		cout<<"constructor."<<endl;
	};
	~singleton()
	{
		cout<<"destructor."<<endl;
	};
	singleton(singleton & obj) 
	{
	};
	singleton & operator=(singleton &obj){return obj;};
public:
	static singleton * instance()
	{
		if(_ins != NULL)
		{
			_ins = new singleton();
		}
		return _ins;
	}
	static singleton * _ins;
};

singleton * singleton::_ins = NULL;

void main(int argc, char* argv[])
{
	{
		cout<<"1"<<endl;
		singleton::instance();
		cout<<"2"<<endl;
	}
	int i;
	cin>>i;
	return;
}
