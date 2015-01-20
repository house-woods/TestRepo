#include <iostream>
#include <string>

using namespace std;
int htoi(string &str)
{
    int len = str.length();
    int pos = 0;
    int val = 0;
    while(pos < len)
    {
        if(str[pos]>='0' && str[pos]<='9')
            val = val * 16 + str[pos] - 48;
        if(str[pos]>='a' && str[pos]<='f')
            val = val * 16 + str[pos] - 87;
        if(str[pos]>='A' && str[pos]<='F')
            val = val * 16 + str[pos] - 55;
        pos++;
    }
    return val;
}
int main(int argc, char* argv[])
{
    string str("10");
    int num = htoi(str);
    cout<<"Dec Num:"<<num<<endl;
    string str01("05");
    string str02("6");
    string str03("25"); 
    cout<<"Dec Num of 0x05:"<<htoi(str01)<<endl;
    cout<<"Dec Numb of 0x6:"<<htoi(str02)<<endl;
    cout<<"Dec Numb of 0x25:"<<htoi(str03)<<endl;
    return 0;
}
