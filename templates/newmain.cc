// 
// File:   newmain.cc
// Author: bwang1
//
// Created on October 10, 2007, 1:54 PM
//

//#include <stdlib.h>
#include <iostream>

#include "specilization.h"

using namespace std;
//
// 
//

int main(int argc, char** argv) {
    int iv1 = 10;
    int iv2 = 20;
    int iRet = compares(iv1, iv2);
    cout<<"iRet:"<<iRet<<endl;
    const char* cp1 = "hello";
    const char* cp2 = "world";
    iRet = compares("hello", "world");
    cout<<"iRet2:"<<iRet<<endl;
    return 0;
}

