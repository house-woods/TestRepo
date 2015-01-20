//*******************************************************************************
//* COPYRIGHT 2005 BY TELLABS OPERATIONS, INC
//* ALL RIGHTS RESERVED
//* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT
//* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC.,
//*******************************************************************************
//*
//* Name: Cryptor.cpp
//*
//* Description: This class consists of implementations of the cryptographic algorithms
//*              used to establish security between client and server.
//*
//*******************************************************************************

#include <stdio.h>
#include <string>
#ifdef WIN32
#include <mbstring.h>
#endif
#include "Cryptor.h"
#include "MD5.h"


//******************************************************************************
//*
//* Function: Cryptor::encrypt
//*
//* Description:
//*
//*    Encrypts a string using one of two specified methods.
//*
//* Parameters:
//*
//*    inString - String to be encrypted.
//*
//*    name - Encryption method to employ, use one of the constants defined in
//*           the ENUM MethodName.
//*
//* Return:
//*
//*    string - Encrypted string
//*
//******************************************************************************
string Cryptor::encrypt(const string& inString, MethodName name)
{
    switch (name)
    {
        // MD5 hashing requested
    case md5Method:
        {
            MD5 md5;
            unsigned len = inString.length();

            md5.update((unsigned char *)inString.c_str(), len);
            // Create the hash
            md5.finalize();
            // Return it as a 33-byte ASCII-Hex string.
            char* hexDig = md5.hex_digest();
            string retToken = string(hexDig);
            delete [] hexDig;
            // 
            return retToken;
        }
    }
}



//******************************************************************************
//*
//* Function: Cryptor::decrypt
//*
//* Description:
//*
//*    Decrypts a string that was encrypted using the XOR method. NOTE: MD5  is
//*    a one-way encryption technique and consequentially cannot be decrypted.
//*    An exception is raised if an attempt is made to decrypt an MD5 string
//*    (based on the value of MethodName)
//*
//* Parameters:
//*
//*    inString - encrypted string
//*
//*    name - encryption method (based on the MethodName ENUM)
//*
//* Return:
//*
//*    string - decrypted string
//*
//*******************************************************************************
string Cryptor::decrypt(const string& inString, MethodName name) throw (InvalidDecryptAttempt)
{
    switch (name)
    {
    case md5Method:
        // There is no decrypting an MD5 hash, MD5 is one-way.
        throw InvalidDecryptAttempt();

    }
}



//******************************************************************************
//*
//* Function: Cryptor::compareAuthenticationTokens
//*
//* Parameters:
//*
//*    token_1 - first encrypted password
//*
//*    token_2 - second encrypted password
//*
//* Return:
//*
//*    bool - True - if the encrypted passwords match, False otherwise
//*
//******************************************************************************

bool Cryptor::compareAuthenticationTokens(const string& token_1, const string& token_2)
{
    // If the strings are identical return true (success).
    return (strcmp(token_1.c_str(), token_2.c_str()) == 0);
}


//******************************************************************************
//*
//* Function: Cryptor::generateAuthenticationToken
//*
//* Parameters:
//*
//*    hashedPassword - previously encrypted password
//*
//*    nonce - any random number
//*
//*
//* Return:
//*
//*    string - MD5 digest
//*
//******************************************************************************
string Cryptor::generateAuthenticationToken (const string& hashedPassword, const string& nonce)
{
    // Construct an MD5 object.
    MD5 md5;

    // Create concatenated string of all involved parameters.
    string authToken = hashedPassword + nonce;

    // Update the MD5 object with the authentication information.
#ifdef WIN32

    md5.update((unsigned char *)authToken.c_str(), _mbslen((unsigned char *)authToken.c_str()));
#elif EMSUNIX

    md5.update((unsigned char *)authToken.c_str(), strlen( authToken.c_str() ) );
#endif

    md5.finalize();

    char* hexDig = md5.hex_digest();
    string retToken = string(hexDig);
    delete [] hexDig;
    return retToken;
}
