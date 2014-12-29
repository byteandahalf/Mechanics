#pragma once
#include "Tag.h"

class StringTag : public Tag
{
public:
	std::string data;
	int len;

	StringTag(const std::string&);
	StringTag(const std::string&, const std::string&);

	virtual ~StringTag();

	virtual void write(IDataOutput*);
	virtual void load(IDataInput*);
	virtual char getId();
	virtual std::string toString();
	virtual Tag* copy();
	virtual bool equals(Tag&);
};