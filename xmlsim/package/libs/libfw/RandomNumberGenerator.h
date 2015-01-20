/*
=========================================================
FileName:   RandomNumberGenerator.h
Author(s):  Kevin L. Stern
Notes:      This is a cryptographically secure random
            number generator for Unix.

History:
Date        Name         Modification
---------------------------------------------------------
02/11/2005  Kevin Stern  Initial version
 
=========================================================
*/

#ifndef _RandomNumberGenerator_H_
#define _RandomNumberGenerator_H_

#include <string>

using namespace std;

class RandomNumberGenerator
{
public:
    static void RandomNumberGenerator::nextBytes(char*, int);
private:
    static const int BUFFERSIZE = 512;
    static unsigned char buffer[BUFFERSIZE];
    static int index;
    RandomNumberGenerator();
};

#endif // ifndef _RandomNumberGenerator_H_
