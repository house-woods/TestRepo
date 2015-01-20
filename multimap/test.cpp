#include <iostream>
#include <map>
#include <utility>
//#include <unordered_map>
using namespace std;
 
int main()
{  
    std::multimap<int,char> map_ex;
    //map_ex.insert({{1,'a'},{2,'b'},{1,'c'}});
    map_ex.insert(multimap<int,char>::value_type(1,'a'));
    map_ex.insert(multimap<int,char>::value_type(1,'b'));
    map_ex.insert(multimap<int,char>::value_type(1,'c'));
    map_ex.insert(multimap<int,char>::value_type(2,'d'));
    std::multimap<int,char>::const_iterator search = map_ex.find(1);
    do
    {
    if(search != map_ex.end()) {
        std::cout << "Found " << search->first << " " << search->second << "\n";
    }
    else {
        std::cout << "Not found" << std::endl;
    }
    ++search;
    } while (search != map_ex.upper_bound(1));
    return 0;
}
