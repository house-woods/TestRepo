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

#include "RandomNumberGenerator.h"
#include <stdlib.h>
#include <openssl/rand.h>

unsigned char RandomNumberGenerator::buffer[BUFFERSIZE];
int RandomNumberGenerator::index = BUFFERSIZE;

using namespace std;

RandomNumberGenerator::RandomNumberGenerator()
{}

void RandomNumberGenerator::nextBytes(char* buf, int bytes)
{
    unsigned int i = 0;

    buf[2*bytes] = 0;
    while (index + bytes > BUFFERSIZE)
    {
        for (; index < BUFFERSIZE; ++index, --bytes, ++i)
        {
            sprintf((char*)buf + i*2, "%02x", buffer[index]);
        }

        RAND_bytes((unsigned char*)buffer, BUFFERSIZE);
        index = 0;
    }

    for (; bytes > 0; ++index, --bytes, ++i)
    {
        sprintf((char*)buf + i*2, "%02x", buffer[index]);
    }
}
