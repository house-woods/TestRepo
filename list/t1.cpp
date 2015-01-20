// sort algorithm example
#include <iostream>
#include <algorithm>
#include <list>
using namespace std;

bool myfunction (int i,int j) { return (i<j); }

int main () {
  int myints[] = {32,71,12,45,26,80,53,33};
  list<int> mylist;
  mylist.push_back(32);
  mylist.push_back(71);
  mylist.push_back(12);
  mylist.push_back(45);
  mylist.push_back(26);
  mylist.push_back(80);
  mylist.push_back(53);
  mylist.push_back(33);
  list<int>::iterator it;

  // using function as comp
  sort (mylist.begin(), mylist.end(), myfunction); // 12 32 45 71(26 33 53 80)

  // print out content:
  cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    cout << " " << *it;

  cout << endl;

  return 0;
}

