#include "../hash_table.h"
#include "../struct_hash_node.h"
#include "../hash_builder.h"

#include <gtest/gtest.h>

/*
ASSERT_TRUE
ASSERT_EQ
ASSERT_NE
ASSERT_LT
ASSERT_LE
ASSERT_GT
ASSERT_GE

EXPECT_EQ
*/

struct TestStruct
{
	int ID;
	int Value;
};
typedef CStructNode<TestStruct> Node;

HashTable<Node> * g_table = NULL;

TEST(MemHash, Init)
{
	g_table = CreateMemHash<Node>(20, 10000);
	ASSERT_FALSE(g_table == NULL);
}

TEST(MemHash, Put)
{
	Node n;
	n->ID = 12;
	n->Value = 199;
	ASSERT_TRUE(g_table->Put(n));

	n->ID = 13;
	n->Value = 900;
	ASSERT_TRUE(g_table->Put(n));
}

TEST(MemHash, Get)
{
	Node n;
	n->ID = 12;
	ASSERT_TRUE(g_table->Get(n));
	ASSERT_TRUE(n->Value == 199);

	n->ID = 13;
	ASSERT_TRUE(g_table->Get(n));
	ASSERT_TRUE(n->Value == 900);

	n->ID = 14;
	ASSERT_FALSE(g_table->Get(n));
}

