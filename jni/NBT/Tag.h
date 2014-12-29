#pragma once
#include <string>

class IDataOutput;
class IDataInput;
class PrintStream;

class Tag
{
public:
	int errorState;
	std::string name;

	virtual ~Tag();
	virtual void deleteChildren();
	virtual void write(IDataOutput*);
	virtual void load(IDataOutput*);
	virtual std::string toString();
	virtual char getId();
	virtual bool equals(Tag&);
	virtual void print(PrintStream&);
	virtual void print(std::string const&, PrintStream&);
	virtual void setName(std::string const&);
	virtual std::string getName();
	virtual Tag* readNamedTag(IDataInput&);
	virtual void writeNamedTag(IDataInput&);

	static Tag* newTag(char, const std::string&);
	static std::string getTagName(char);
	
	virtual Tag* copy();
	virtual Tag& operator=(const Tag&);
};