#include <gtest/gtest.h>

#include "data_stream.h"
#include "test_node.pb.h"


using namespace jewel;

TEST(DataStream, pb)
{
	test_node::ProtoNode n1;
	n1.set_id(12);
	n1.set_value(99);

	test_node::ProtoNode n2;

	OutDataStream out;
	out << (uint16_t)n1.ByteSize() << manip::protobuf(n1);

	std::string res = out.data();

	InDataStream in(res);
	uint16_t sz = 0;
	in >> sz;
	in >> manip::protobuf(n2, sz);

	ASSERT_TRUE(sz == n1.ByteSize());
	ASSERT_TRUE(n1.id() == n2.id());
	ASSERT_TRUE(n1.value() == n2.value());
}

TEST(DataStream, pb2)
{
	test_node::ProtoNode n1;
	test_node::ProtoNode n2;
	n1.set_id(1);
	n1.set_value(11);
	n2.set_id(2);
	n2.set_value(12);

	OutDataStream out;
	out << (uint16_t)n1.ByteSize() << (uint16_t)n2.ByteSiez()
		<< manip::protobuf(n1) << manip::protobuf(n2);

	std::string res = out.data();

	test_node::ProtoNode r1;
	test_node::ProtoNode r2;

	InDataStream in(res);
	uint16_t sz1 = 0;
	uint16_t sz2 = 0;
	in >> sz1 >> sz2;
	in >> manip::protobuf(r1, sz1) >> manip::protobuf(r2, sz2);

	ASSERT_TRUE(sz1 == n1.ByteSize());
	ASSERT_TRUE(sz2 == n2.ByteSize());
	ASSERT_TRUE(n1.id() == r1.id());
	ASSERT_TRUE(n2.id() == r2.id());
	ASSERT_TRUE(n1.value() == r1.value());
	ASSERT_TRUE(n2.value() == r2.value());
}

