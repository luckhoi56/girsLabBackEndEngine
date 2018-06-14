// $Id: BRASSError.cpp 619 2005-05-24 19:08:43Z fgroen $

#include "BRASSError.h"

BRASSError::BRASSError(void)
{
}

BRASSError::~BRASSError(void)
{
}

///////////////////////////////////////////////////////////////////

/**
 * Adds a new error to the error list.
 * Returns the number of entreis in the list.
 */
int MsgList::add(const string & msg)
{
	MsgListEntry entry;
	entry.msg = msg;
	msg_lst.push_back(entry);
	return msg_lst.size();
}

/**
 * Returns the next messge in the list.
 * The  message is removed from the list.
 * Returns 0 if the list is empty.
 */
int MsgList::next(string & msg)
{
	list<MsgListEntry>::iterator it = msg_lst.begin();

	if (it != msg_lst.end()) 
	{
		msg = (*it).msg;
		msg_lst.erase(it);
		return msg_lst.size() + 1;
	} 
	else 
	{
		msg = "";
		return 0;
	}
}

/**
 * Returns the size of the error list.
 */
int MsgList::size(void) const 
{
	return (int)msg_lst.size();
}

/**
 * Clears the error list.
 */
void MsgList::clear() 
{
	msg_lst.clear();
}
