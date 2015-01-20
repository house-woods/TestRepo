// 
// File:   newmain.cc
// Author: bwang1
//
// Created on October 11, 2007, 3:15 PM
//

#include <stdlib.h>
#include <iostream>

using namespace std;
typedef unsigned int Bit;
class File {
public:
    File() {
        modified = 2;
        mode = 1;
        status = 5;
    };
    Bit modified:2;
    Bit mode:1;
    Bit status:3;
};
//
// 
//
int main(int argc, char** argv) {
    File aFile;
    cout<<"modified:"<<aFile.modified<<endl;
    cout<<"mode:"<<aFile.mode<<endl;
    cout<<"status:"<<aFile.status<<endl;
    return (EXIT_SUCCESS);
}

