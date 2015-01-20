#include <string>
#include <iostream>

#include "specilization.h"
template <typename T>
int compares(const T &v1, const T &v2)
{
    if(v1 < v2)
    {
        return -1;
    }
    else if(v1 > v2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
};
template <>
int compares<const char*>(const char* const &v1, const char* const &v2)
{
    std::cout<<"The template specilization instance."<<std::endl;
    return std::strcmp(v1, v2);
};

