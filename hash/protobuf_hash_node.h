#ifndef PROTOBUF_HASH_NODE_H
#define PROTOBUF_HASH_NODE_H

#include "hash_table.h"

namespace jewel
{

template<typename T, int MaxSize>
class ProtoBufNode
{
public:
	bool Serialize(char * buffer, int len)
	{
		if (_elem.ByteSize() > len - 2) {
			return false;
		}
		*(uint16_t*)buffer = _elem.ByteSize();
		return _elem.SerializeToArray(buffer + 2, len - 2);
	}
	bool Deserialize(char * buffer, int len)
	{
		if (len < 2) {
			return false;
		}
		uint16_t real_len = *(uint16_t*)buffer;
		if (real_len + 2 > len) {
			return false;
		}
		return _elem.ParseFromArray(buffer + 2, real_len);
	}
	uint32_t HashCode()
	{
		return _elem.id() * 0x9e370001u;
	}
	bool Equal(const ProtoBufNode & node) {
		return _elem.id() == node._elem.id();
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

}

#endif // PROTOBUF_HASH_NODE_H

