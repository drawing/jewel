
#include "http_router.h"

#include <gtest/gtest.h>

class TestHandler : public HTTPHandler
{
public:
	TestHandler(int code) : code(code) {}
private:
	int code;
public:
	int Process(const HTTPRequest & request, HTTPResponse & response) override
	{
		return code;
	}
};

TEST(HTTPRouter, Register)
{
	HTTPRouter router;
	bool bRet = router.Register("/test/", new TestHandler(1));

	EXPECT_TRUE(bRet);

	bRet = router.Register("/test/", new TestHandler(1));

	EXPECT_FALSE(bRet);
}

TEST(HTTPRouter, Handle)
{
	HTTPRouter router;
	router.Register("/test/", new TestHandler(0));
	router.Register("/test/t1", new TestHandler(1));
	router.Register("/test/t2", new TestHandler(2));
	router.Register("/test/t3/", new TestHandler(3));

	HTTPRequest request;
	HTTPResponse response;

	request.set_uri("/test/test1");
	router.Handle(request, response);
	EXPECT_EQ(response.get_status_code(), 0);

	request.set_uri("/testgo");
	router.Handle(request, response);
	EXPECT_EQ(response.get_status_code(), 404);

	request.set_uri("/test/t1/go");
	router.Handle(request, response);
	EXPECT_EQ(response.get_status_code(), 0);

	request.set_uri("/test/t2");
	router.Handle(request, response);
	EXPECT_EQ(response.get_status_code(), 2);

	request.set_uri("/test/t3/go");
	router.Handle(request, response);
	EXPECT_EQ(response.get_status_code(), 3);

	request.set_uri("/test/t3");
	router.Handle(request, response);
	EXPECT_EQ(response.get_status_code(), 0);
}

