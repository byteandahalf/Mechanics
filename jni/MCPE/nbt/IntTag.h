#pragma once

#include "Tag.h"

class IntTag : public Tag
{
public:
	int data;

	IntTag(std::string const&);
	IntTag(std::string const&, int);

	virtual ~IntTag();
	virtual void write(IDataOutput&) const;
	virtual void load(IDataInput&);
	virtual std::string toString(void) const;
	virtual char getId(void) const;
	virtual bool equals(Tag const&) const;
	virtual Tag* copy(void) const;
};