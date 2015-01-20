#include <string>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
    vector<int> tmpVec;
    for (int i = 0; i < 20; i++)
    {
        tmpVec.push_back(i);
    }

    for (vector<int>::iterator it = tmpVec.begin(); it != tmpVec.end(); it++)
    {
        if ( 15 == *it )
        {
            it = tmpVec.erase(it);
        } 
        cout<<*it<<endl;
    }
    return 0;
}
