/***********************************************************************************************
* COPYRIGHT 2002  BY TELLABS OPERATIONS, INC 
* ALL RIGHTS RESERVED 
* NO PART OF THIS DOCUMENT MAY BE REPRODUCED WITHOUT 
* THE WRITTEN PERMISSION OF TELLABS OPERATIONS, INC., 
* 1000 REMINGTON BLVD., BOLINGBROOK, IL 60440
************************************************************************************************
* 
* Name: TL1ErrCodeResolver
* Type: cpp
* Originator: $Author: lzou$
* File: $Workfile:   TL1ErrCodeResolver.cpp  $
* Version: $Revision:  $
* Description: 
* Name resolver specific for TL1
* 
* Revision History:
*
* $Log:  $
*  
**********************************************************************************************/


#include "TL1ErrCodeResolver.h"
#include "BaseException.h"
#include "TL1ErrCodes.h"

using namespace EMS_FW;

TL1ErrCodeResolver* TL1ErrCodeResolver::_instance = 0;
const string TL1ErrCodeResolverName = "TL1NameResolver";

TL1ErrCodeResolver::TL1ErrCodeResolver()
{
    _initialize();
}

TL1ErrCodeResolver* TL1ErrCodeResolver::instance()
{
    if (_instance == 0)
        return new TL1ErrCodeResolver();
    return _instance;
}

const string& TL1ErrCodeResolver::getClassName() const
{
    return TL1ErrCodeResolverName;
}

bool TL1ErrCodeResolver::_initialize()
{
    return _initTL1ErrorCode();
}

bool TL1ErrCodeResolver::_initTL1ErrorCode()
{
    _errCodeMap[TL1_MA::TL1_EQWT] = TL1_MA::TL1_EQWT_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IBEX] = TL1_MA::TL1_IBEX_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IBMS] = TL1_MA::TL1_IBMS_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IBNC] = TL1_MA::TL1_IBNC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_ICNC] = TL1_MA::TL1_ICNC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_ICNV] = TL1_MA::TL1_ICNV_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IDNC] = TL1_MA::TL1_IDNC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IDNV] = TL1_MA::TL1_IDNV_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IDRG] = TL1_MA::TL1_IDRG_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IISP] = TL1_MA::TL1_IISP_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IITA] = TL1_MA::TL1_IITA_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IPEX] = TL1_MA::TL1_IPEX_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IPMS] = TL1_MA::TL1_IPMS_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IPNC] = TL1_MA::TL1_IPNC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_IPNV] = TL1_MA::TL1_IPNV_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_ISCH] = TL1_MA::TL1_ISCH_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_ISPC] = TL1_MA::TL1_ISPC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_PICC] = TL1_MA::TL1_PICC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_PIOC] = TL1_MA::TL1_PIOC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_PIUI] = TL1_MA::TL1_PIUI_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_RCBY] = TL1_MA::TL1_RCBY_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_RTBY] = TL1_MA::TL1_RTBY_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_RTEN] = TL1_MA::TL1_RTEN_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SAAS] = TL1_MA::TL1_SAAS_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SCNA] = TL1_MA::TL1_SCNA_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SCNF] = TL1_MA::TL1_SCNF_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SDNC] = TL1_MA::TL1_SDNC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SDNR] = TL1_MA::TL1_SDNR_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SNVS] = TL1_MA::TL1_SNVS_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SROF] = TL1_MA::TL1_SROF_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SSNC] = TL1_MA::TL1_SSNC_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SSNV] = TL1_MA::TL1_SSNV_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SSRE] = TL1_MA::TL1_SSRE_ERRORCODE;
    _errCodeMap[TL1_MA::TL1_SSTP] = TL1_MA::TL1_SSTP_ERRORCODE;
    return true;
}


int TL1ErrCodeResolver::resolveTL1ErrCode ( const string& errName)
{
    map <string, int>::iterator p = _errCodeMap.find(errName);
    if (p == _errCodeMap.end())
    {
        throw UnresolvedNameEx(errName);
    }
    return (*p).second;
}
