#include "TL1AccountManager.h"

TL1AccountManager::TL1AccountManager()
{
    _currentAccountUsername = "";
    _currentAccountPassword = "";
}

TL1AccountManager::~TL1AccountManager()
{}

string TL1AccountManager::getUsername()
{
    return _currentAccountUsername;
}

string TL1AccountManager::getPassword()
{
    return _currentAccountPassword;
}

void TL1AccountManager::setUsername(const string &username)
{
    _currentAccountUsername = username;
}

void TL1AccountManager::setPassword (const string &password)
{
    _currentAccountPassword = password;
}

void TL1AccountManager::nextAccount (const string &tid)
{}

void TL1AccountManager::updatePassword (const string &tid, const string &username, const string &new_password)
{}

void TL1AccountManager::setUserDefinedAccount (const string &userDefinedUsername, const string &userDefinedPassword)
{}
