#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <ospace/helper.h>

using namespace std;

class OtsUtility
{
public:
    static int compareNeVersionWithF(const string& neId, const string& versionToMatch);
    static void OtsUtility::seperateNeVersion(string& neVersion, string& fVer);
    static int OtsUtility::compareFVersion(string& neFver, string& fVerToMatch);
};
int OtsUtility::compareNeVersionWithF(const string& neId, const string& versionToMatch)
{
    static osi_tokenizer t(".", false, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "", "", false);

    string neVersion = neId;
    if (neVersion.empty())
    {
        return -1;
    }
    // The NE version prefix will be "TELLABS_7100." or "FP". 13 and 2 chars respectively.
    if (0 == neVersion.compare(0, 2, "FP"))
        neVersion.erase(0, 2);
    else if (0 == neVersion.compare(0, 13,"TELLABS_7100."))
        neVersion.erase(0, 13);
    string verToMatch = versionToMatch;
    string neFVersion("");
    string fVersionToMatch("");
    OtsUtility::seperateNeVersion(neVersion, neFVersion);
    OtsUtility::seperateNeVersion(verToMatch, fVersionToMatch);
    vector<string>& versionTokens = t.tokenize(neVersion);
    vector<string>& matchTokens = t.tokenize(verToMatch);

    int nSizeDiff = versionTokens.size() - matchTokens.size();
    vector<string>& smaller = (nSizeDiff > 0) ? matchTokens : versionTokens;
    for (int counter = 0; counter < abs(nSizeDiff); counter++)
    {
        smaller.push_back("0");
    }

    vector<string>::iterator itVersion = versionTokens.begin();
    vector<string>::iterator itMatch = matchTokens.begin();

    int diff = 0;
    while (diff == 0 && itVersion != versionTokens.end())
    {
        const string& first = *itVersion++;
        const string& second = *itMatch++;

        diff = atoi(first.c_str()) - atoi(second.c_str());
    }
    if (0 == diff)
    {
        //in case 6.1.0A > 6.1.0
        diff = OtsUtility::compareFVersion(neFVersion, fVersionToMatch);
    }

    return diff;
}
void OtsUtility::seperateNeVersion(string& neVersion, string& fVer)
{
    string::size_type pos = neVersion.find_first_not_of("0123456789.");
    if (pos != string::npos)
    {
        fVer = neVersion.substr(pos);
        neVersion = neVersion.substr(0, pos);
    }
}
//F version format: F<p> p 2 digits in max
int OtsUtility::compareFVersion(string& neFver, string& fVerToMatch)
{
    int diff = 0;
    string neChar(""), neNum("0"), matChar(""), matNum("0");
    if (!neFver.empty())
    {
        string::size_type lpos = neFver.find_last_not_of("0123456789.");
        neChar = neFver.substr(0,lpos + 1);
        if (lpos != string::npos)
        {
            neNum = neFver.substr(lpos + 1);
        }
    }
    if (!fVerToMatch.empty())
    {
        string::size_type lpos = fVerToMatch.find_last_not_of("0123456789.");
        matChar = fVerToMatch.substr(0,lpos + 1);
        if (lpos != string::npos)
        {
            matNum = fVerToMatch.substr(lpos + 1);
        }
    }
    diff = neChar.compare(matChar);
    if (0 == diff)
    {
        diff = atoi(neNum.c_str()) - atoi(matNum.c_str());
    }
    return diff;
}

int main(int argc, char *argv[])
{
  cout<<argv[1]<<"----"<<argv[2]<<endl;
  int res = OtsUtility::compareNeVersionWithF(argv[1],argv[2]);
  if (0==res) 
  {
    cout<<argv[1]<<"=="<<argv[2]<<endl;
  }
  else if(res < 0)
  {
    cout<<argv[1]<<"<"<<argv[2]<<endl;
  }
  else 
  {
    cout<<argv[1]<<">"<<argv[2]<<endl;
  }
  return 0;
}
