//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "common.h"

class Title
{
public:
    Title() = default;
    ~Title() = default;

    std::string subject;

    uint32_t current_number;

    uint32_t next_rank_points = 0;
    uint32_t rank = 0;
    uint32_t server_title_index = 0;
};
