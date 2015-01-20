#include <iostream>
using namespace std;
class CTest
{
public:
	CTest():m_data0(50){};
	void spr()
        {
            cout<<"const data:"<<m_data0<<endl;
            cout<<m_data<<endl;
        };
        
private:
        const int m_data0;
	static int m_data;
};
CTest::m_data=10;
void main(int argc, char* argv[])
{
    CTest aobj;
    aobj.spr();
    return;
}
