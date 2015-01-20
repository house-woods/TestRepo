#include "TL1CmdInfo.h"

using namespace EMS_FW;


void TL1CmdInfo::setTl1msgString(const string& msgString)
{
    _tl1msgString = msgString;
}

const string& TL1CmdInfo::getTl1msgString() const
{
    return _tl1msgString;
}

void TL1CmdInfo::setTL1msgDate(const string& msgDate)
{
    _tl1msgDate = msgDate;
}

void TL1CmdInfo::setTL1msgTime(const string& msgTime)
{
    _tl1msgTime = msgTime;
}

const string& TL1CmdInfo::getTL1Aid() const
{
    return _tl1Aid;
}

const string& TL1CmdInfo::getTL1ParentId() const
{
    return _tl1ParentId;
}

const string& TL1CmdInfo::getTL1msgDate() const
{
    return _tl1msgDate;
}

const string& TL1CmdInfo::getTL1msgTime() const
{
    return _tl1msgTime;
}

void TL1CmdInfo::setTL1Aid(const string& tl1Aid)
{
    _tl1Aid = tl1Aid;

}

void TL1CmdInfo::setTL1ParentId(const string& tl1ParentId)
{
    _tl1ParentId = tl1ParentId;

}







void TL1CmdInfo::reinitialize(const BaseCmdInfo* orig)
{
    BaseCmdInfo::reinitialize(orig);
    TL1CmdInfo* info = (TL1CmdInfo*) orig;
    setTl1msgString(info->getTl1msgString());
    setTL1msgDate(info->getTL1msgDate());
    setTL1msgTime(info->getTL1msgTime());
    setTL1Aid(info->getTL1Aid());
    setTL1ParentId(info->getTL1ParentId());
    setCTAG(info->getCTAG());
    setDBCHGSEQ(info->getDBCHGSEQ());
}


BaseCmdInfo* TL1CmdInfo::clone() const
{
    BaseCmdInfo* info = new TL1CmdInfo();
    info->reinitialize(this);
    return info;
}

const string& TL1CmdInfo::getCTAG (void) const
{
    return _ctag;
}

void TL1CmdInfo::setCTAG (const string &ctag)
{
    _ctag = ctag;
}

const string& TL1CmdInfo::getDBCHGSEQ (void) const
{
    return _dbchgSeq;
}

void TL1CmdInfo::setDBCHGSEQ (const string &dbchgSeq)
{
    _dbchgSeq = dbchgSeq;
}


