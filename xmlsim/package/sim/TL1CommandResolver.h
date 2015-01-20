#include <string>
#include <list>
using namespace std;
class TL1CommandResolver
{
public:

    static TL1CommandResolver* getInstance();
    bool resolveCommand(string &configTid, string &command, list<string> &response);
    bool resolveUserCommand(string &command, string &response, string &origCommand);

protected:
    TL1CommandResolver()
    { }
    ;
    ~TL1CommandResolver()
    { }
    ;

private:
    static TL1CommandResolver* m_commandResolver;
};
