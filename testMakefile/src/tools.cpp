#include "tools.h"
#include "../test/commonerror.h"
#include <iostream>

using namespace std;
using namespace TOOLS;
using namespace COMMONS;

bool Tools::output(const string& outstr)
{
    if(outstr.empty())
    {
        cout<<ERROR_INPUT_EMPTY<<endl;
        return false;
    }
    cout<<"******"<<endl;
    cout<<outstr<<endl;
    return true;
}


