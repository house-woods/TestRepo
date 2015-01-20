#include <string>
#include <iostream>

using namespace std;
void decorate(const string& oldVer, string& newVer)
{
   string tmpVer = oldVer;
   if (0 == tmpVer.compare(0,2,"FP"))
   {
       tmpVer.erase(0,2);
   }
   string::size_type pos = tmpVer.find_first_not_of("0123456789.");
   string fVer("");
   if(pos != string::npos)
   {
       fVer = tmpVer.substr(pos, tmpVer.length() - pos);
       string::size_type lpos = fVer.find_last_not_of("0123456789.");
       if (lpos != string::npos)
       {
           fVer.replace(0,lpos + 1,".");
       }
       tmpVer = tmpVer.substr(0, pos);
   }
   int in=0;
   char buffer[20] = "\0";
   strcpy(buffer, tmpVer.c_str());
   char *p[20];
   char *buf=buffer;

   while((p[in]=strtok(buf,"."))!=NULL) 
   {
       buf = p[in++];
       buf = NULL;
   } 
   if (0 == in)
   {
     return;
   }
   int diff = 3 - in;
   for(int i = 0; i < diff; i++)
   {
       p[3-diff+i] = "0";
   };
   for(int j = 0; j < 3; j++)
   {
       if (j != 0)
       {
           newVer.append(".");
       } 
       newVer.append(p[j]);
   } 
   newVer.insert(0,"FP");
   newVer.append(fVer);
}

int main(int argc, char* argv[])
{
/*    string str("");
    if(str.compare("hello"))
    {
        cout<<"empty."<<endl;
    }
    string a("hello:");
    char b = a[a.length()-1];
    if (b == ':')
    {
        cout<<"it's :"<<endl;
    }
    cout<<b<<endl;
    const string str("hello");
    cout<<"pos="<<str.find_first_not_of("hel")<<endl;
    string str1 = str;
    str1 = str1.replace(0,5," world");
    cout<<str<<str1<<endl;
*/
    string v1("FP6.1");
    string v2("FP6.1.0");
    string v3("FP6.1.0F1");
    string v4("FP6.1.0AF1");
    string v5("FP6.1.0Af1");
    string v6("FP6.1.0A1");
    string v7("FP6.1F1");
    string v8("FP6F1");
    string v9("FP6.1.1");
    string tmp("");
    decorate(v1,tmp);
    cout<<"v1 "<<v1<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v2,tmp);
    cout<<"v2 "<<v2<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v3,tmp);
    cout<<"v3 "<<v3<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v4,tmp);
    cout<<"v4 "<<v4<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v5,tmp);
    cout<<"v5 "<<v5<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v6,tmp);
    cout<<"v6 "<<v6<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v7,tmp);
    cout<<"v7 "<<v7<<"->"<<tmp<<endl;
    tmp.clear();
    decorate(v8,tmp);
    cout<<"v8 "<<v8<<"->"<<tmp<<endl;
    return 0;
}
