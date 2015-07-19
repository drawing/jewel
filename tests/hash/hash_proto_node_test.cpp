#include "hash_table.h"
#include "protobuf_hash_node.h"
#include "hash_builder.h"

#include <gtest/gtest.h>

#include "test_node.pb.h"

typedef ProtoBufNode<test_node::ProtoNode, 10> Node;

static HashTable<Node> * g_table = NULL;

TEST(ProtoNodeTest, Init)
{
	g_table = CreateMemHash<Node>(20, 10000);
	ASSERT_FALSE(g_table == NULL);
}

TEST(ProtoNodeTest, Put)
{
	Node n;
	n->set_id(12);
	n->set_value(99);
	ASSERT_TRUE(g_table->Put(n));

	n->set_id(13);
	n->set_value(900);
	ASSERT_TRUE(g_table->Put(n));
}

TEST(ProtoNodeTest, Get)
{
	Node n;
	n->set_id(12);
	ASSERT_TRUE(g_table->Get(n));
	ASSERT_TRUE(n->value() == 99);

	n->set_id(13);
	ASSERT_TRUE(g_table->Get(n));
	ASSERT_TRUE(n->value() == 900);

	n->set_id(14);
	ASSERT_FALSE(g_table->Get(n));
}

