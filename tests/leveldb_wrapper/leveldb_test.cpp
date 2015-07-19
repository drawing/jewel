
#include "storage.h"

#include <gtest/gtest.h>

TEST(LevelDB, NormalKV)
{
	jewel::Storage store;
	store.Init("./db");

	store.Register(1, std::string());

	std::string value1 = "this is a test value";
	store.Put(1, "key1", value1);

	std::string value2;
	store.Get(1, "key1", &value2);

	EXPECT_STREQ(value1.c_str(), value2.c_str());
}

