//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "typedefs.h"

class CItem;

class mail
{
public:
    mail(uint32_t __id, std::string __title, std::string __sender, Date d);
    ~mail();

    uint32_t id;
    std::string title;
    std::string sender;
    std::string *msg;
    Date date;

    std::vector<CItem *> items;
};
