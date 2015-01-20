#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

void log(const char *desc, ...)
{
        const int MAX_LEN = 100;
        char res[MAX_LEN];
        va_list ap;
        va_start(ap, desc);
        vsnprintf(res, MAX_LEN - 1, desc, ap);
        va_end(ap);
        res[MAX_LEN - 1] = '\0';
        cout<<res<<endl;
}

void main(int argc, char* argv[])
{
  char tmp[3];
  char tmp1[4];
  int ch = atoi(argv[1]);
  int cnt = atoi(argv[2]); 
  memset(tmp1,ch,cnt);
  log("This is %s, and %s.",tmp1,NULL);
  return;
}
