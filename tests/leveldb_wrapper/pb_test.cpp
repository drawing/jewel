

#include "test_node.pb.h"

#include <gtest/gtest.h>

namespace jewel
{

int CompareObject(const test_node::KeyType & key1, const test_node::KeyType & key2)
{
	return key1.key() - key2.key();
}

}

#include "protobuf_store.h"
#include "storage.h"

TEST(LevelDB, PBKV)
{
	jewel::Storage store;
	store.Init("./db");

	store.Register(2, test_node::KeyType());

	test_node::KeyType key1;
	test_node::ProtoNode value1, value2;

	value1.set_id(56);
	value1.set_value(67);

	key1.set_key(123);
	store.Put(2, key1, value1);

	store.Get(2, key1, &value2);

	EXPECT_EQ(value1.id(), value2.id());
	EXPECT_EQ(value1.value(), value2.value());
}

