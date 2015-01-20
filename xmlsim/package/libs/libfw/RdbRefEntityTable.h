/***********************************************************************************************
* COPYRIGHT 2005 BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1415 W Diehl Rd., Naperville, IL 60563
*
* Rev 1.0  12/33/2004  lzou
* the memeory representation of REF_ENTITY table
************************************************************************************************/



#ifndef _RdbRefEntityTable_H__
#define _RdbRefEntityTable_H__


#include <map>
#include <list>

#ifdef OS_USE_ALTERNATE_STD
using namespace std;
#endif


namespace EMS_FW
{

class RdbRefEntityTable
{
public:

    static RdbRefEntityTable* instance();


    void initialize();

    void insertRecord(const string& aidType, const string& id,
                      const string& entity_i_type);

    bool getEntityIdandType (const string& aidType, string& id,
                             string& entity_i_type);

    bool getEntityIdCode (const string& aidType, string& id);

    bool getAidTypeandType (const string& id, string& aidType,
                            string& entity_i_type);

    string getAidType (const string& id);

    bool RdbRefEntityTable::getPossibleAidTypes (list<string>& aidTypes);

    string RdbRefEntityTable::getAidTypesForAlarmFilter ();

    string RdbRefEntityTable::getConditionTypesForAlarmFilter ();

protected:
    static RdbRefEntityTable* _instance;

private:


    RdbRefEntityTable();

    ~RdbRefEntityTable();

    void _insertRecord(const string& aidType, const string& id,
                       const string& entity_i_type);

    struct ref_entity
    {
        string entity_id;
        string entity_i_type;
        ref_entity(const string& id, const string& type) : entity_id(id),
                entity_i_type(type)
        {}
    }
    ;

    map <string, struct ref_entity*> _tableRecords;
    map <string, struct ref_entity*> _reverseRecords;

}
; // end of class RdbRefEntityTable

}
; // end of namespace EMS_FW

#endif
