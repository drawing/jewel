#ifndef PROTOBUF_HASH_NODE_H
#define PROTOBUF_HASH_NODE_H

#include "hash_table.h"

template<typename T, int MaxSize>
class ProtoBufNode
{
public:
	bool Serialize(char * buffer, int len)
	{
		if (len != sizeof(T)) {
			return false;
		}
		return _elem.ParseFromArray(buffer, len);
	}
	bool Deserialize(char * buffer, int len)
	{
		if (len != sizeof(T)) {
			return false;
		}
		return _elem.SerializeToArray(buffer, len);
	}
	uint32_t HashCode()
	{
		return _elem.ID() * 0x9e370001u;
	}
	bool Equal(const ProtoBufNode & node) {
		return _elem.ID() == node._elem.ID();
	}
	int FixedSize() {
		return MaxSize;
	}

	T * operator->() {
		return &_elem;
	}
public:
	T _elem;
};

#endif // PROTOBUF_HASH_NODE_H

