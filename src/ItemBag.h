
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
