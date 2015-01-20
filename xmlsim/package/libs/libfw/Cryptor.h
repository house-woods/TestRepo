//**********************************************************************************************
//* COPYRIGHT 2005 BY TELLABS OPERATIONS, INC 
//* ALL RIGHTS RESERVED 
//* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
//* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
//**********************************************************************************************
//* 
//* Name: Cryptor.h
//*
//* Description: This class consists of implementations of the cryptographic algorithms
//*              used to establish security between client and server.
//*
//**********************************************************************************************

#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <iostream>
#include <string>
#include "BaseException.h"

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif

class InvalidDecryptAttempt : public EMS_FW::BaseException 
{ };

class Cryptor {
    
public:
    enum MethodName {xorMethod, md5Method, noEncryption};
    
    static bool compareAuthenticationTokens(const string&, const string&);
    
    static string generateAuthenticationToken(const string&, const string&);
    
    static string encrypt(const string& inString, MethodName methodIndex = xorMethod);
    static string decrypt(const string& inString, MethodName methodIndex = xorMethod) throw (InvalidDecryptAttempt);
};

#endif CRYPTOR_H
