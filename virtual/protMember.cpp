#include <iostream>
#include <string> 

using namespace std;
class A
{
public:
protected:
    int _mem;
};
class B : public A
{
public:
    void setMem(int ival)
    {
        _mem=ival;
    };
    void pr()
    {
        cout<<"_mem="<<_mem<<endl;
    };
};
void main(int argc, char* argv[])
{
/*    B aobj;
    aobj.setMem(10);
    aobj.pr();
*/
    string astr("-hi");
    string bstr("-end.");
    string side = "hello" + astr + bstr;
    cout<<side<<endl;
    return;
}
