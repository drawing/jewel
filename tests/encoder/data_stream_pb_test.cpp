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

