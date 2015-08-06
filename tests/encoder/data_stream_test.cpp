#include "data_stream.h"

#include <gtest/gtest.h>

using namespace jewel;

TEST(DataStream, In)
{
	InDataStream in;
	uint8_t i8 = 10;
	uint16_t i16 = 0x1234;
	std::string s = "abc";
	in << i8 << s << i16;
	std::string res = in.data();

	ASSERT_TRUE(res.at(0) == 10);
	ASSERT_TRUE(res.at(1) == 'a');
	ASSERT_TRUE(res.at(2) == 'b');
	ASSERT_TRUE(res.at(3) == 'c');
	ASSERT_TRUE(res.at(4) == 0x12);
	ASSERT_TRUE(res.at(5) == 0x34);
	ASSERT_TRUE(res.size() == 6);
}

TEST(MemHash, Put)
{
	Node n;
	n->id = 12;
	n->value = 199;
	ASSERT_TRUE(g_table->Put(n));

	n->id = 13;
	n->value = 900;
	ASSERT_TRUE(g_table->Put(n));
}

TEST(MemHash, Get)
{
	Node n;
	n->id = 12;
	ASSERT_TRUE(g_table->Get(n));
	ASSERT_TRUE(n->value == 199);

	n->id = 13;
	ASSERT_TRUE(g_table->Get(n));
	ASSERT_TRUE(n->value == 900);

	n->id = 14;
	ASSERT_FALSE(g_table->Get(n));
}

