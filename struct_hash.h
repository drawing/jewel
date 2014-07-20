#ifndef STRUCT_HASH_H
#define STRUCT_HASH_H

#include "hash_table.h"

/*
Usage:
struct TestStruct
{
	uint32_t Key;
	int Value;
};
typedef CStructNode<TestStruct> Node;

HashTable<Node> * g_table = CreateMemHash<Node>(20, 10000);

Node n;
n->Key = 12;
n->Value = 199;
g_table->Put(n);

g_table->Get(n);
*/

template<typename T>
class CStructNode
{
public:
	bool Serialize(char * buffer, int len)
	{
		if (len != sizeof(T)) {
			return false;
		}
		*(T*)buffer = _elem;
		return true;
	}
	bool Deserialize(char * buffer, int len)
	{
		if (len != sizeof(T)) {
			return false;
		}
		_elem = *(T*)buffer;
		return true;
	}
	uint32_t HashCode()
	{
		return _elem.Key * 0x9e370001u;
	}
	bool Equal(const CStructNode & node) {
		return _elem.Key == node._elem.Key;
	}
	int FixedSize() {
		return sizeof(T);
	}

	T * operator->() {
		return &_elem;
	}
public:
	T _elem;
};

#endif // STRUCT_HASH_H

