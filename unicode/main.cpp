#include <string>
#include <iostream>
#include <wchar.h>
#include <locale.h>

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"");
    wchar_t tmp[] = L"Ҳ";
    char tmp1[] = "Ҳ";
    char tmp2[] = "0123";
    //tmp[0] = 159;
    wprintf(L"unicode:");
    cout<<"no l:"<<endl;
    wprintf(L"%s.\n",tmp);
    wprintf(L"%ls\n\n",tmp);
    cout<<"****************"<<endl;
    printf("y%s.\n",tmp1);
    printf("x%ls.\n",tmp1);
    return 0; 
}
