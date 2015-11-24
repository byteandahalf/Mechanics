#pragma once

#include <string>

class PrintStream;
class IDataOutput;
class TagMemoryChunk;
class IDataInput;
class Tag
{
public:
	Tag(const std::string&);

	virtual ~Tag();
	virtual void deleteChildren();
	virtual void write(IDataOutput&) const = 0;
	virtual void load(IDataInput&) = 0;
	virtual std::string toString() const = 0;
	virtual char getId() const = 0;
	virtual bool equals(const Tag&) const;
	virtual void print(PrintStream&) const;
	virtual void print(std::string const&, PrintStream &) const;
	virtual void setName(const std::string&);
	virtual std::string getName() const;
	virtual Tag* copy() const = 0;

	static std::string getTagName(char);
	static Tag* newTag(char, std::string const&);

	void readNamedTag(IDataInput&, std::string&);
	void writeNamedTag(std::string const&, Tag const&, IDataOutput&);
};