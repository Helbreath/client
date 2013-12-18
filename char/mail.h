#ifndef MAIL_H
#define MAIL_H
#pragma once

#include "typedefs.h"
#include "item/Item.h"

class Mail
{
public:
	Mail(uint32 __id, string __title, string __sender, Date d);
	~Mail();
	
	uint32 id;
	string title;
	string sender;
	string * msg;
	Date date;

	Vector<CItem*> items;
};

typedef Vector<Mail*> MailVec;
typedef MailVec::iterator MailVecIt;

#endif
