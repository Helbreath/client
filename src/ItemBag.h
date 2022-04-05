//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include "Item.h"

class ItemBag
{
public:
    ItemBag();
    ~ItemBag();

    std::vector<shared_ptr<CItem>> itemList;
    uint16_t Count() { return itemList.size(); }
};
