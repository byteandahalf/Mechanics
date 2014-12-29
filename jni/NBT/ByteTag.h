#pragma once
#include "Tag.h"

class ByteTag : public Tag
{
public:
	char data;

	ByteTag(const std::string&);
	ByteTag(const std::string&, char);

	~ByteTag();
	virtual void write(IDataOutput*);
	virtual void load(IDataInput*);
	virtual char getId();
	virtual std::string toString();
	virtual Tag* copy();
	virtual bool equals(Tag&);
};