#ifndef _MYS_H
#define _MYS_H
#include <string>
#include <list>

class MYST
{
public:
  MYST(){};
  ~MYST(){};
  struct lane
  {
  std::string laneno;
  std::string optpwr;
  };
  std::list<lane> _la;
}

#endif
