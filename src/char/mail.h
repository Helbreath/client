#ifndef MAIL_H
#define MAIL_H
#pragma once

#include "../shared/typedefs.h"

class CItem;

class Mail
{
public:
	Mail(uint32_t __id, string __title, string __sender, Date d);
	~Mail();
	
	uint32_t id;
	string title;
	string sender;
	string * msg;
	Date date;

	std::vector<CItem*> items;
};

typedef std::vector<Mail*> MailVec;
typedef MailVec::iterator MailVecIt;

#endif
