#pragma once
#include <string>
#include "Tag.h"

class IntTag : public Tag
{
public:
	int data;

	IntTag(const std::string&);
	IntTag(const std::string&, int);

	~IntTag();

	virtual void write(IDataOutput*);
	virtual void load(IDataInput*);
	virtual char getId();
	virtual std::string toString();
	virtual Tag* copy();
	virtual bool equals(Tag&);
};