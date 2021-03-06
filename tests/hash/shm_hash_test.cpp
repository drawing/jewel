#include "hash_table.h"
#include "struct_hash_node.h"
#include "hash_builder.h"

#include <gtest/gtest.h>

using namespace jewel;

struct TestStruct
{
	int id;
	int value;
};
typedef CStructNode<TestStruct> Node;

TEST(ShmHash, Init)
{
	HashTable<Node> * table = CreateShmHash<Node>(0x1D000010, 20, 10000);
	ASSERT_FALSE(table == NULL);
}

TEST(ShmHash, Put)
{
	HashTable<Node> * table = CreateShmHash<Node>(0x1D000010, 20, 10000);
	Node n;
	n->id = 12;
	n->value = 199;
	ASSERT_TRUE(table->Put(n));

	n->id = 13;
	n->value = 900;
	ASSERT_TRUE(table->Put(n));
}

TEST(ShmHash, Get)
{
	HashTable<Node> * table = CreateShmHash<Node>(0x1D000010, 20, 10000);
	ASSERT_FALSE(table == NULL);

	Node n;
	n->id = 12;
	ASSERT_TRUE(table->Get(n));
	ASSERT_TRUE(n->value == 199);

	n->id = 13;
	ASSERT_TRUE(table->Get(n));
	ASSERT_TRUE(n->value == 900);

	n->id = 14;
	ASSERT_FALSE(table->Get(n));
}

