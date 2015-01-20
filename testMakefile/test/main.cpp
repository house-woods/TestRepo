#include <string>
#include <iostream>
#include "commonerror.h"
#include "../src/tools.h"

using namespace std;
using namespace TOOLS;
using namespace COMMONS;

int main(int argc, char* argv[])
{
    Tools aObj;
    string outstr("");
    if (!aObj.output(outstr))
    {
        cout<<"Test: Pass!"<<endl;
    }
    else
    {
        cout<<"Test: failed!"<<endl;
    }
    return 0; 
}
