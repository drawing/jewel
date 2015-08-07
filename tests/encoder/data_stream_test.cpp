#include "data_stream.h"

#include <gtest/gtest.h>

using namespace jewel;

TEST(DataStream, Out)
{
	OutDataStream out;
	uint8_t i8 = 10;
	uint16_t i16 = 0x1234;
	std::string s = "abc";
	out << i8 << s << i16;
	std::string res = out.data();

	ASSERT_TRUE(res.at(0) == 10);
	ASSERT_TRUE(res.at(1) == 'a');
	ASSERT_TRUE(res.at(2) == 'b');
	ASSERT_TRUE(res.at(3) == 'c');
	ASSERT_TRUE(res.at(4) == 0x12);
	ASSERT_TRUE(res.at(5) == 0x34);
	ASSERT_TRUE(res.size() == 6);
}

TEST(DataStream, In)
{
	OutDataStream out;
	uint8_t i8 = 10;
	uint16_t i16 = 0x1234;
	std::string s = "abc";
	out << i8 << s << i16;
	std::string res = out.data();

	ASSERT_TRUE(res.at(0) == 10);
	ASSERT_TRUE(res.at(1) == 'a');
	ASSERT_TRUE(res.at(2) == 'b');
	ASSERT_TRUE(res.at(3) == 'c');
	ASSERT_TRUE(res.at(4) == 0x12);
	ASSERT_TRUE(res.at(5) == 0x34);
	ASSERT_TRUE(res.size() == 6);

	InDataStream in(res);

	uint8_t oi8;
	uint16_t oi16;
	uint8_t c;

	std::string ostr;
	in >> oi8 >> manip::in_string(ostr, 3) >> oi16;
	ASSERT_TRUE(bool(in));
	ASSERT_TRUE(ostr == s);
	ASSERT_TRUE(i8 == oi8);
	ASSERT_TRUE(i16 == oi16);

	in >> c;
	ASSERT_TRUE(!in);
}

