#pragma once
#include "Tag.h"
#include <vector>

class TagMemoryChunk;
class ListTag;

class CompoundTag : public Tag
{
public:
	std::map<std::string, Tag*> tags;

	CompoundTag();
	CompoundTag(const std::string&);
	~CompoundTag();

	virtual void write(IDataOutput*);
	virtual void load(IDataInput*);

	virtual void getAllTags(std::vector<Tag*>&);
	virtual char getId();

	void put(const std::string&, Tag*);
	void putByte(const std::string&, char);
	void putShort(const std::string&, short);
	void putInt(const std::string&, int);
	void putLong(const std::string&, long long int);
	void putFloat(const std::string&, float);
	void putDouble(const std::string&, float); //Blame Mojang
	void putString(const std::string&, const std::string&);
	void putByteArray(const std::string&, TagMemoryChunk);
	void putCompound(const std::string&, CompoundTag*);
	void putBoolean(const std::string&, bool);

	Tag* get(const std::string&);

	bool contains(const std::string&);
	bool contains(const std::string&, int);

	char getByte(const std::string&);
	short getShort(const std::string&);
	int getInt(const std::string&);
	long long int getLong(const std::string&);
	float getFloat(const std::string&);
	double getDouble(const std::string&);
	std::string getString(const std::string&);
	TagMemoryChunk getByteArray(const std::string&);
	TagMemoryChunk getIntArray(const std::string&);
	CompoundTag* getCompound(const std::string&) const;
	ListTag* getList(const std::string&);
	bool getBoolean(const std::string&);

	virtual std::string toString();
	virtual void print(const std::string&, PrintStream&);
	virtual bool isEmpty();
	virtual Tag* copy();
	virtual bool equals(Tag&);
	//virtual operator=(const CompoundTag&);
};
