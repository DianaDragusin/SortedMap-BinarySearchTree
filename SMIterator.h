#pragma once
#include "SortedMap.h"

//DO NOT CHANGE THIS PART
class SMIterator{
	friend class SortedMap;
private:
	const SortedMap& map;
	SMIterator(const SortedMap& mapionar);

	BSTNode* stack;
    int currentkey;

public:
	void first();
	void next();
	bool valid() const;
    TElem getCurrent() const;
};

