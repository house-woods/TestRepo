/***********************************************************************************************
* COPYRIGHT $Date:   Dec 14, 2004  $ BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
************************************************************************************************
* 
* Name:             TL1CommandConfigurator.h
* Type:             C++ header
* Originator:       Altaf Aali
* File: $Workfile:  TL1CommandConfigurator.h
* Description:      Class responsible for initializing and providing access to configuration
*                   paramaters for TL1 commands such as timesouts, syntax etc
* 
*                   !!Perfect software is not a myth!!
**********************************************************************************************/
#ifndef _TL1COMMANDCONFIGURATOR_H_
#define _TL1COMMANDCONFIGURATOR_H_

#include <string>
#include <map>

using namespace std;

class TL1CommandConfigurator
{
public:

    static TL1CommandConfigurator *instance();
    unsigned long _defaultMSECTimeout;
    void initializeMSECTimeouts (unsigned long defaultMSECTimeout);
    unsigned long getMSECTimeout (string command, const string &version = "");

private:

    TL1CommandConfigurator ();
    ~TL1CommandConfigurator ();
    static TL1CommandConfigurator * _instance;
    map<string, string> _commandToTimeoutMap;
};

#endif
