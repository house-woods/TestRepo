#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class A
{
public:
  A(const char* name, string id)
  {
    ptr = new char[10];
    strcpy(_name,name);
    _id=id;
  };
  void refill()
  {
    string tmp(22,'#');
    strcpy(_name,tmp.c_str());
  };
  string _id;
  char _name[10];
  char * ptr;
};

int main(int argc, char* argv[])
{
    string id("8789");
    A* obj = new A("test",id);
    A* obj2 = new A("china","99932");
    A tmpObj("stack","22321");
//    obj->refill();
    char str1[] = "1234567890";
    string message(27,'*');
    cout<<"message="<<message.size()<<endl;
    //char* msg = new char[message.size()];
    char str2[] = "abcdefghijklmnopqrstuvwxyz";
    cout<<sizeof(str2)<<endl;
    strcpy(str2, message.c_str());
//    msg[message.size()] = '\0';
    cout<<str2<<"||||"<<endl;
    cout<<obj->_id<<endl;
    return 0;
}
