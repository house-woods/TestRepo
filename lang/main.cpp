#include <iostream>
#include <string>
#include <locale.h>
using namespace std;

std::string myws2s(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 4 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

int main(int argc, char* argv[])
{
    wstring tmpw = L"ÖÐÎÄ";
    wcout<<tmpw<<endl;
    cout<<"wstring len="<<tmpw.length()<<endl;
    string tmps = myws2s(tmpw);
    cout<<tmps<<endl;
    cout<<"string len="<<tmps.length()<<endl;
    return 0;
}
