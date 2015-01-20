#include <iostream>
#include <string>
#include <set>

using namespace std;


void main(int argc, char* argv[])
{
  set<string> shelfNos;
/*  shelfNos.insert("");
  shelfNos.insert("1");
  shelfNos.insert("2");
  shelfNos.insert("1");
  shelfNos.insert("10");
*/
        string shelfList("");
        if (shelfNos.size() > 0)
        {
            int i = 0;
            for (set<string>::const_iterator itr = shelfNos.begin();itr != shelfNos.end();++itr,i++)
            {
                cout<<"Item "<<i<<"="<<*itr<<endl;
                if ((*itr).length() > 0)
                {
                    if (shelfList.empty())
                    {
                        shelfList = "('" + *itr++ + "'";
                    }
                    else
                    {
                        shelfList += ",'" + *itr + "'";
                    }
                }
            }
            if (!shelfList.empty())
            {
                shelfList.append(")");
            }
        }
   cout<<"Out string = "<<shelfList<<endl;
   cout<<"Out string length = "<<shelfList.length()<<endl;
}
