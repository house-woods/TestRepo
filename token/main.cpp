#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <ospace/helper.h>

using namespace std;

void
display_tokens( map <string, vector<string>*>& tokens )
{
    map< string, vector<string>* >::const_iterator i;
    for ( i = tokens.begin(); i != tokens.end(); i++ )
    {
        cout<<"CMD:"<<i->first<<endl;
        vector<string>::const_iterator iter;
        for(iter = i->second->begin(); iter != i->second->end(); iter++)
        {
            cout << " \"" << *iter << "\"" << endl;
        }
    }
    cout << endl;
}

void initialize(const string &resposeFilePath, map <string, vector<string>*>& _grammar)
{
    const char* methodName = "initialize";
    // read the grammar (from, usually, TL1Response.txt) and cache it
    osi_tokenizer tokenizer( ":", true, "\\\n", "\0", "", false );
    osi_tokenizer cmdExtractor( " ", false, "", "\n", "", false );
    ifstream is;
    is.open(resposeFilePath.c_str(), ios::in | ios::nocreate);
    if (is.fail())
    {
        string message = string ("Could not initialize the TL1 syntax map as the TL1 response file ") + resposeFilePath + " was not found";
        cout<<message<<endl; 
    }
    vector<string>* ptokens;
    vector< string > cmdTokens;
    char buffer[256];
    while (!is.eof())
    {
        is.getline(buffer, sizeof(buffer));
        cmdTokens = cmdExtractor.tokenize(buffer);
        if (cmdTokens.size() < 2)
            continue; //we have hit the end of file.
        ptokens = new vector<string> (tokenizer.tokenize(cmdTokens[1]));
        _grammar[cmdTokens[0]] = ptokens;
    }
}

int main()
{
    map <string, vector<string>*> _grammar;
    initialize("/home/bwang1/SunStudioProjects/token/TL1Response.txt", _grammar);
    display_tokens( _grammar );
    return 0;
}


