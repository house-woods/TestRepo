#include <iostream>
#include <string>
using namespace std;

void _fetchNETimeZoneForPM(bool& isEqualGreaterGMP, int& offsetGMT)
{
    // find NE time zone
    string timezone = "GREENWICH MEAN TIME: DUBLIN- EDINBURGH-LISBON-LONDON [GMT]";
    int pos = timezone.find("[GMT");
    offsetGMT = 0;
    isEqualGreaterGMP = true;
    string stroffsetGMT;
    if (pos != -1)
    {
        pos = pos + 4;
        timezone = timezone.substr(pos);

        // if the timezone is + from GMT then we need to subtract the offset
        // therefore the flag  'isEqualGreaterGMP' is set to false and vic-versa
        if (timezone.find("+") != string::npos)
            isEqualGreaterGMP = false;
        pos = timezone.find_last_of("-");
        if (pos != string::npos)
        {
            // extract offset from GMT
            stroffsetGMT = timezone.substr(pos - 2, 2);
            offsetGMT = atoi(stroffsetGMT.c_str());
        }

    }
};

int main(int argc, char* argv[])
{
  unsigned char sw_version[4];
  cout<<sizeof(sw_version)<<endl;
  cout<<sizeof(unsigned char)<<endl;
  cout<<sizeof(unsigned short)<<endl;
  cout<<sizeof(unsigned long)<<endl;
  bool isEqualGreaterGMP = false;
  int offsetGMT = 0;
  _fetchNETimeZoneForPM(isEqualGreaterGMP, offsetGMT);
  cout<<isEqualGreaterGMP<<"="<<offsetGMT<<endl;
  return 0;
}
