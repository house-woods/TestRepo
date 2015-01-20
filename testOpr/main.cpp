#include <iostream>
#include <string>

using namespace std;
class Clock
{
public:
 Clock & operator ++() 
{
  ++m_i;
  return *this;
};

Clock operator ++(int)
{
  Clock temp;
  temp.m_i = m_i;
  m_i++;
  return temp;
};

int m_i;
};

int main(int argc, char* argv[])
{
    Clock obj;
    obj.m_i = 10;
    cout<<"**"<<obj.m_i<<endl;
    Clock tmp = ++obj;
    cout<<"after pre ++"<<tmp.m_i<<endl;
    tmp = obj++;
    cout<<"after post ++"<<tmp.m_i<<endl;
    cout<<"Last :"<<obj.m_i<<endl;
    return 0;
}
