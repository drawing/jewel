
#include "http_uri_enc.h"

#include <gtest/gtest.h>

TEST(HTTPHelper, Decode)
{
	std::string uri = "www.qq.com/test?p1=3&p2=2%25M";

	auto params = HTTPHelper::GetParams(uri);
	EXPECT_STREQ(params["p1"].c_str(), "3");
	EXPECT_STREQ(params["p2"].c_str(), "2%M");
}

