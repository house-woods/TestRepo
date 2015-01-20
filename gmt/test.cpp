#include <iostream>
#include <string>
#include <ospace/time.h>
int
main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout<<"Usage: "<<argv[0]<<" micro_seconds"<<std::endl;
  }
  osi_time_toolkit initialize;
  unsigned long ulTime = atoi(argv[1]);
  osi_time_and_date dt(ulTime);
  std::cout<<"time zone="<<dt.time_zone()<<std::endl;
  std::string startDateValue = dt.to_string("%m-%d");
  std::string startTimeValue = dt.to_string("%H-%M");
  std::cout << "date= " << startDateValue<< std::endl;
  std::cout << "time= " << startTimeValue<< std::endl;
  std::cout << "utc date= " << dt.utc_date().to_string("%m-%d")<< std::endl;
  std::cout << "utc time= " << dt.utc_time().to_string("%H-%M")<< std::endl;
  return 0;
}
