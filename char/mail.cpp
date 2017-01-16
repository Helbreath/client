#include "mail.h"
#include "item/Item.h"

Mail::Mail(uint32_t __id, string __title, string __sender, Date d) :
	id(__id), title(__title), sender(__sender), date(d), msg(0)
{
}

Mail::~Mail()
{
	if(msg)
		delete msg;

	for(std::vector<CItem*>::iterator it = items.begin();
		it != items.end();
		++it)
	{
		delete *it;
	}
}