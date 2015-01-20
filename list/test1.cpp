#include <iostream>
#include <algorithm>
#include <list>
using namespace std;
 
  // Assumes  TYPE t; cout << t;  is valid.
template<class TYPE>
inline ostream & operator<< ( ostream & theOstream,
                              const list<TYPE> & theList )
{
  typename list<TYPE>::const_iterator listIterator = theList.begin();
  for ( int i = 0;   listIterator != theList.end();  ++listIterator, i ++ )
    theOstream << "    [" << i << "]:   \"" << (*listIterator) << "\"" << endl;
  return theOstream;
}
 
struct Functor
{
  bool operator()( const char * a, const char * b )
    { return strcmp(a,b) < 0; }
};
 
int
main()
{
  list<char*>  l;
 
    /* Load up some example test data... */
  char  s[3];
  s[2] = '\0';
  for (   s[0]='c'; s[0]>='a'; s[0]-- )
    for ( s[1]='c'; s[1]>='a'; s[1]--  )
      l.push_back(strdup(s));
 
    /* Show us that test data... */
  cout << l << endl;
 
    /* Sort list. */
  Functor f;
  l.sort(f);
 
    /* Show us what we have now... */
  cout << l << endl;
}
