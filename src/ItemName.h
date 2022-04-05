//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

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
