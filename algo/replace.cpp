#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
   vector<string> strV;
   strV.push_back("1");
   strV.push_back("1");
   strV.push_back("1");
   strV.push_back("1");
   for(vector<string>::iterator iter = strV.begin();iter != strV.end();iter++)
   {
      (*iter).replace(0,(*iter).length(),"abc");
   } 
   int cnt = 1;
   for(vector<string>::iterator iter = strV.begin();iter != strV.end();iter++)
   {
       cout<<"Item "<<cnt++<<"="<<*iter<<endl;
   }
   return 0;
}
