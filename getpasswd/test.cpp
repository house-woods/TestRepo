#include <iostream>
#include <string>
#include <ospace/time.h>
using namespace std;
string generateNewPasswd()
{
    string newPassword;
    char specialChars[] = "!@$";
        //osi_time_and_date currTimeDate = osi_time_and_date::now();
        osi_date currDate(11,17,2013);
        osi_time currTime(21,12,24);
        cout << "date = " << currDate << endl;
        cout << "time = " << currTime << endl;
        cout<<"day_of_year="<<currDate.day_of_year()<<endl;
        cout<<"week_of_year="<<currDate.week_of_year()<<endl;
        cout<<"week_day="<<currDate.weekday()<<endl;
        cout<<"hour="<<currTime.hours()<<endl;
        cout<<"minutes="<<currTime.minutes()<<endl;
        cout<<"seconds="<<currTime.seconds()<<endl;
        newPassword += (char) ('A' + (currDate.day_of_year() % 26));
        newPassword += (char) ('x' - (currTime.minutes() % 20));
        newPassword += (char) ('9' - currDate.weekday());
        newPassword += (char) ('b' + (currTime.seconds() % 23));
        short weekNum = currDate.week_of_year();
        if (weekNum < 26)
            newPassword += (char) ('Z' - weekNum);
        else if (weekNum < 52)
            newPassword += (char) ('a' + (weekNum - 26));
        else
            newPassword += (char) ('9' - (weekNum - 52));
        newPassword += (char) ('Z' - currTime.hours());
        newPassword += (char) ('9' - (currDate.year() % 10));
        newPassword.insert((size_t) (currDate.year() % 7), 1, specialChars[currTime.seconds() % 3]);
    return newPassword;
}
int
main(int argc, char* argv[])
{
  osi_time_toolkit initialize;
  string newpasswd = generateNewPasswd();
  std::cout <<"new password="<<newpasswd<<endl; 
  return 0;
}
