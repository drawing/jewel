#ifndef STRUCT_HASH_NODE_H
#define STRUCT_HASH_NODE_H

#include "hash_table.h"

/*
Usage:
struct TestStruct
{
	uint32_t ID;
	int Value;
};
typedef CStructNode<TestStruct> Node;

HashTable<Node> * g_table = CreateMemHash<Node>(20, 10000);

Node n;
n->ID = 12;
n->Value = 199;
g_table->Put(n);

g_table->Get(n);
*/

namespace jewel
{

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
		return uint32_t(_elem.id * 0x9e370001u);
	}
	bool Equal(const CStructNode & node) {
		return _elem.id == node._elem.id;
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

}

#endif // STRUCT_HASH_NODE_H

