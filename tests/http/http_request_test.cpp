
#include "http.h"

#include <gtest/gtest.h>


TEST(HTTPRequest, encode)
{
	HTTPRequest request;
	char buffer[1024];
	int len = sizeof(buffer);
	int ret = request.encode(buffer, len);
	ASSERT_EQ(ret, 0);
	buffer[len] = 0;
	EXPECT_STREQ(buffer, "GET / HTTP/1.1\r\n\r\n");

	len = sizeof(buffer);
	request.set_method("POST");
	ret = request.encode(buffer, len);
	ASSERT_EQ(ret, 0);
	buffer[len] = 0;
	EXPECT_STREQ(buffer, "POST / HTTP/1.1\r\nContent-Length: 0\r\n\r\n");

	len = sizeof(buffer);
	request.add_header("User-Agent", "google");
	ret = request.encode(buffer, len);
	ASSERT_EQ(ret, 0);
	buffer[len] = 0;
	EXPECT_STREQ(buffer, "POST / HTTP/1.1\r\nContent-Length: 0\r\nUser-Agent: google\r\n\r\n");
}

