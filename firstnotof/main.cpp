#include <iostream>
#include <string>

using namespace std;

void main(int argc, char* argv[])
{
    string lower_case = "abcdefghijklmnopqrstuvwxyz ,-";
    string str = "this is the lower-case part, AND THIS IS THE UPPER-CASE PART";
    int index = str.find_first_not_of(lower_case);
    cout << "first non-lower-case letter in str at: " << index << endl;
    cout<<str[index]<<endl;
}
