

#ifndef _TL1CmdInfo_H_
#define _TL1CmdInfo_H_

#include <string>
#include <list>
#include "BaseCmdInfo.h"

namespace EMS_FW
{

class TL1CmdInfo : public NotificationInfo
{
public:

    TL1CmdInfo()
    {}

    TL1CmdInfo(const string& moc, const string& moi,
               const string& type = "", const long& timestamp = 0) :
            NotificationInfo(moc, moi, type, timestamp)
    {}

    void setTl1msgString(const string& msgString);

    const string& getTl1msgString() const;

    void setTL1msgDate(const string& msgDate);

    void setTL1msgTime(const string& msgTime);

    void setTL1Aid(const string& tl1Aid);

    void setTL1ParentId(const string& tl1ParentId);

    const string& getTL1Aid() const;

    const string& getTL1ParentId() const;

    const string& getTL1msgDate() const;

    const string& getTL1msgTime() const;

    virtual void reinitialize(const BaseCmdInfo* orig);
    // Reinitialize the BaseCmdInfo with the new values. Old attributes will be deleted.

    virtual BaseCmdInfo* clone() const;
    // Create copy of the original BaseCmdInfo.

    const string& getCTAG (void) const;

    void setCTAG (const string &ctag);

    const string& getDBCHGSEQ (void) const;

    void setDBCHGSEQ (const string &dbchgSeq);


protected:

    string _tl1msgString;

    string _tl1msgDate;

    string _tl1msgTime;

    string _tl1Aid;

    string _tl1ParentId;

    string _ctag;

    string _dbchgSeq;

};

};
#endif
