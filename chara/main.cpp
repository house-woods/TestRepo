#include <iostream>
#include <bitset>
#include <string>
using namespace std;

void main() 
{ 
 //   int i=15;
 //   char s[20];	
 //   itoa(i, s, 16);
	//string str = s;
	//cout<<str<<endl;
    //printf("0x%s\n",s);

	//string str("");
	//str.resize(10);
	//str.assign(10, '0');
	//cout<<str<<endl;

	string str("0987654321");
	for(int i = 0; i < str.length(); i++)
	cout<<i<<str[i]<<endl;

        int a = 5%2;
	cout<<a<<endl;

	int b = 5/2;
	cout<<b<<endl;
	
	cout<<"****************"<<endl;
	unsigned int c = 1<<7;
	cout<<c<<endl;
	unsigned int d = 1<<0;
	cout<<d<<endl;
	unsigned int off = 2;
	unsigned int e = 1<<off;
	cout<<e<<endl;
	unsigned int f = 1<<3;
	cout<<f<<endl;


}
