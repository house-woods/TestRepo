#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    string str("123");
    char *arr = new char[10];
    cout<<str<<endl;
    str = "hello";
    string str01 = arr;
    cout<<str01<<endl;
    const char *ptr = arr;
    return 0;
}
