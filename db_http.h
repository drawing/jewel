#ifndef DB_HTTP_H
#define DB_HTTP_H

#include <string>
#include <map>

class HTTPRequest
{
public:
	HTTPRequest();

	// 请求方设置
	int add_header(const std::string & key, const std::string & value);
	int set_uri(const std::string & uri);
	int set_post_body(const std::string & payload);
	int set_method(const std::string & method);

	int encode(char buffer[], int & len);

public:
	// 服务器端解析
	std::string get_header(const std::string & key) const;
	std::string get_post_body() const;
	std::string get_uri() const;
	std::string get_method() const;

	int decode(char buffer[], int & len);

private:
	std::string sMethod;
	std::string sURI;
	std::string sPayload;
	std::map<std::string, std::string> mpHeaders;
};


class HTTPResponse
{
public:
	HTTPResponse();

	// 客户端请求响应解析
	std::string get_header(const std::string & key) const;
	std::string get_body() const;
	int get_status_code() const;

	int decode(char buffer[], int len);

	// 服务端组包
	int add_header(const std::string & key, const std::string & value);
	int set_body(const std::string & payload);
	int set_status_code(int iCode);

	int encode(char buffer[], int len);
private:
	int iStatusCode;
	std::string sPayload;
	std::map<std::string, std::string> mpHeaders;
};


#endif // DB_HTTP_H

