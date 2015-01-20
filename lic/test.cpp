#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    ofstream out(argv[1], ios::binary);
    char buff[1];
    buff[0] = 0x30;
    out.write(buff, 1);
    out.close();
    return 0;
}

