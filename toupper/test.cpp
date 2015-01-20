#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class Tutil
{
public:
static char charToLowerCase (char element)
{
    if ((element >= 'A') && (element <= 'Z'))
        element += 32;
    return element;
};
char charToUpperCase (char element)
{
    if ((element >= 'a') && (element <= 'z'))
        element -= 32;
    return element;
};

static void toUpperCase (string &myString)
{
    transform (myString.begin(), myString.end(), myString.begin(), charToUpperCase);
};
};


int main(int argc, char* argv[])
{
  string str("AC 1D 14 3D 33 7e");
  string str1("AC 1D 14 3D 33 7E");
  Tutil::toUpperCase(str);
  cout<<str<<endl;
  cout<<str1<<endl;
  if(0 == str1.compare(str))
  {
    cout<<"equal"<<endl;
  }
  return 0;
}
