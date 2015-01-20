#include <iostream>
#include <string>
using namespace std;

void main(int argc, char* argv[])
{
    string date = "20101208092722";
    cout<<"10*date[6]->"<<10*date[6]<<endl;
    cout<<"date[6]->"<<date[6]<<endl;
    cout<<"date[7]- (11 * '0')->"<<date[7] - (11 * '0')<<endl;
    cout<<"date[7]->"<<date[7]<<endl;
    cout<<"11* '0'->"<<(11 * '0')<<endl;
    int iDate = 10*date[6] + date[7]-(11 * '0');
    cout<< iDate<<endl;
    return;
}
