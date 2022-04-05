//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "common.h"
#include "skilltypes.h"

class skill
{
public:
    skill() = default;
    ~skill() = default;

    std::string name;

    uint32_t level = 0;
    bool usable = false;
    uint8_t use_method = 0;
};
