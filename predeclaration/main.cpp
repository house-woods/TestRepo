#include <iostream>

using namespace std;

class CB;
class CA
{
public:
    CB* getB(){return NULL;};
};

class CB
{
public:
    CA* getA(){return NULL;};
};

int main(int argc, char* argv[])
{
    return 0;
}

