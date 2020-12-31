// ItemName.h: interface for the CItemName class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

class CItemName
{
public:
    CItemName();
    virtual ~CItemName();

    char m_cOriginName[21];
    char m_cName[34];
};
