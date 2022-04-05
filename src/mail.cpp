//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "mail.h"
#include "Item.h"

mail::mail(uint32_t __id, string __title, string __sender, Date d) : id(__id), title(__title), sender(__sender), date(d), msg(nullptr)
{
}

mail::~mail()
{
	if (msg)
		delete msg;

	for (auto & item : items)
	{
		delete item;
	}
}
