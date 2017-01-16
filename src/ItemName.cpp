//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "ItemName.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemName::CItemName()
{
    memset(m_cOriginName, 0, sizeof(m_cOriginName));
    memset(m_cName, 0, sizeof(m_cName));
}

CItemName::~CItemName()
{
}
