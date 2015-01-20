#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    char* ptr = "080515";
    //char* ptr = "5"; 
    if(strlen(ptr) > 2)
    {
        ptr = ptr + strlen(ptr) - 2;
    }
    printf("The output is:%02.2s\n", ptr);
    return 0;
}
