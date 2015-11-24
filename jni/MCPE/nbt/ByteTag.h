#pragma once

#include "Tag.h"

class ByteTag : public Tag
{
public:
	char data;

	ByteTag(std::string const&);
	ByteTag(std::string const&, char);

	virtual ~ByteTag();
	virtual void write(IDataOutput&) const;
	virtual void load(IDataInput&);
	virtual std::string toString() const;
	virtual char getId() const;
	virtual bool equals(Tag const&);
	virtual Tag* copy() const;
};