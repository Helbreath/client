#include "mail.h"

Mail::Mail(uint32 __id, string __title, string __sender, Date d) :
	id(__id), title(__title), sender(__sender), date(d), msg(NULL)
{
}

Mail::~Mail()
{
	if(msg)
		delete msg;

	for(Vector<CItem*>::iterator it = items.begin();
		it != items.end();
		++it)
	{
		delete *it;
	}
}