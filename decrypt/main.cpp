#include <string>
#include <iostream>

#include "tutil.h"
#include "Rijndael.h"
#include "ComManager.h"
#include "sysErrLog.h"

using namespace std;
using namespace EMS_FW;
using namespace ELogger;

const char* ServerIdName = "DECRYPT";
const char* sourceName = "decrypt";

void getReadablePW (string& userPassword)
{
     char pChar=userPassword[0];
     int indexP=0;

     for (;(pChar!='\0' && indexP<=31);indexP++)
     {
        pChar = userPassword[indexP];
        if (pChar<33 || pChar >126)
        {
           userPassword.resize(indexP);
           break;
        }
     }

}
char charToUpperCase (char element)
{
    if ((element >= 'a') && (element <= 'z'))
        element -= 32;
    return element;
}

void toUpperCase (string &myString)
{
    transform (myString.begin(), myString.end(), myString.begin(), charToUpperCase);
}
string decryptRijndaelEx (const string& hexEncryptedText, const string& key="zifheers8592#(.@")
{
    const int blockSize = 16;
    const string initialChain(blockSize, '\0');

    string plainText;

    try
    {
        CRijndael oRijndael;
        oRijndael.MakeKey(key.data(), initialChain.data(), key.size(), initialChain.size());

        string encryptedText;
        encryptedText.reserve(blockSize);
        for ( int i = 0; i < 2 * blockSize; i += 2)
        {
            char hi = hexEncryptedText[i];
            char lo = hexEncryptedText[i + 1];
            encryptedText.push_back( (char) ( ( ((hi >= '0' && hi <= '9') ? (hi - '0') : (hi - 'A' + 10)) << 4) |
                                              ((lo >= '0' && lo <= '9') ? (lo - '0') : (lo - 'A' + 10)) ) );
        }

        plainText.resize(blockSize);
        oRijndael.DecryptBlock(encryptedText.data(), const_cast<char *>(plainText.data()));

        if (plainText.find_last_not_of('\0') == string::npos)
        {
            plainText.resize(0);
        }
        else
        {
            plainText.resize(plainText.find_last_not_of('\0') + 1);
        }
    }
    catch (...)
    {
        plainText = "bad\003string";
    }

    return plainText;
}

int main(int argc, char* argv[])
{
   LocalErrorLogger::startLogging("./out.log", 3, 100000);
   string rawPassword;
   string hashedPwd = argv[1];
   cout<<"Encrypted password:"<<hashedPwd<<endl;
   toUpperCase(hashedPwd);
   rawPassword = decryptRijndaelEx(hashedPwd);
   getReadablePW(rawPassword);  
   cout<<"Decrypted password:"<<rawPassword<<endl;
   return 0;
}
