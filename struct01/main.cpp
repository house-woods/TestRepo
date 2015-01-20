#include <iostream>
using namespace std;
typedef struct oper 
{
	int id;
	string name;
	string addr; 
} OPER;
int main(int argc, char* argv[])
{
	OPER aobj = 
	{
		id:	10,
		name:	"bobo",
		addr:	"liaoning",
	};
	cout<<"aobj::"<<endl;
	cout<<"id="<<aobj.id<<endl;
	cout<<"name="<<aobj.name<<endl;
	cout<<"addr="<<aobj.addr<<endl;
	return 0;
}
