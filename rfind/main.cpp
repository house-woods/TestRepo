#include <iostream>
#include <string>
using namespace std;
void getTid(const string& file, string& tid)
{
  string::size_type pos = file.rfind("-");
  tid = file.substr(0,pos);
}

int main(int argc, char* argv[])
{
  string tid("SH-NANO-log");
  string tid1("SH-NANO-NO1-log"); 
  string tid2("SHNANO-log");
  string tid3("SHNANO");

  string realTid("");
  getTid(tid,realTid);
  cout<<tid<<"->"<<realTid<<endl;
  cout<<"length="<<realTid.length()<<endl;
  realTid.clear();
  getTid(tid1,realTid);
  cout<<tid1<<"->"<<realTid<<endl;
  cout<<"length="<<realTid.length()<<endl;
  realTid.clear();
  getTid(tid2,realTid);
  cout<<tid2<<"->"<<realTid<<endl;
  cout<<"length="<<realTid.length()<<endl;
  realTid.clear();
  getTid(tid3,realTid);
  cout<<tid3<<"->"<<realTid<<endl;
  cout<<"length="<<realTid.length()<<endl;
  return 0;
}
