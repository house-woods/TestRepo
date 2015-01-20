#ifndef _TL1AccountManager_H_
#define _TL1AccountManager_H_

#include <string>

using namespace std;

class TL1AccountManager
{
public:
    TL1AccountManager ();
    ~TL1AccountManager ();
    string getUsername ();
    string getPassword ();
    virtual void nextAccount (const string &tid);
    virtual void updatePassword (const string &tid, const string &username, const string &new_password);
    virtual void setUserDefinedAccount (const string &userDefinedUsername, const string &userDefinedPassword);

private:
    string _currentAccountUsername;
    string _currentAccountPassword;

protected:
    void setUsername (const string &username);
    void setPassword (const string &password);



};

#endif
