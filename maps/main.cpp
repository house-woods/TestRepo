#include <iostream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{
	string a,b;		
	map<string, string> strMap;
	try
	{    
		a = "a";
		b = "b";
		strMap.insert(map<string, string>::value_type(a, a));
		strMap.insert(map<string, string>::value_type(a, b));
	}
	catch(...)
	{
		cout<<"exception."<<endl;
	}
	for(map<string, string>::iterator iter = strMap.begin(); iter != strMap.end(); iter++)
	{
		cout<<iter->first<<"->"<<iter->second<<endl;
	}
	return 0;

}
