
#include "db_http.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;


// helper function
string TrimString(const string & sInput)
{
	size_t key_start = sInput.find_first_not_of(" \r\n\t");
	size_t key_end = sInput.find_last_not_of(" \r\n\t");

	if (key_start == string::npos || key_end == string::npos) {
		return sInput;
	}

	return sInput.substr(key_start, key_end + 1);
}

int SplitKeyValue(const string & sData, string & sKey, string & sValue)
{
	size_t split = sData.find_first_of(":");
	if (split == string::npos) {
		return 0;
	}
	sKey = sData.substr(0, split);
	sValue = sData.substr(split + 1);
	return 0;
}


// Request
HTTPRequest::HTTPRequest()
{
	sMethod = "GET";
	sURL = "/";
}

int HTTPRequest::add_header(const std::string & key, const std::string & value)
{
	mpHeaders[key] = value;
	return 0;
}

int HTTPRequest::set_uri(const std::string & uri)
{
	sURL = uri;
	return 0;
}

int HTTPRequest::set_post_body(const std::string & payload)
{
	sPayload = payload;
	return 0;
}

int HTTPRequest::set_method(const std::string & method)
{
	if (method != "POST" && method != "GET") {
		return -1;
	}

	sMethod = method;
	return 0;
}

int HTTPRequest::encode(char buffer[], int & len)
{
	int offset = snprintf(buffer, len, "%s %s HTTP/1.1\r\n", sMethod.c_str(), sURL.c_str());
	if (offset <= 0 || offset >= len) {
		return -10;
	}

	if (sMethod == "POST") {
		char sContentLength[20] = {};
		snprintf(sContentLength, sizeof(sContentLength), "%lu", sPayload.size());
		mpHeaders["Content-Length"] = sContentLength;
	}

	int out_len = 0;

	for (std::map<std::string, std::string>::iterator i = mpHeaders.begin(); i!= mpHeaders.end(); i++) {
		out_len = snprintf(buffer + offset, len - offset, "%s: %s\r\n", i->first.c_str(), i->second.c_str());
		if (out_len <= 0 || out_len >= len - offset) {
			return -11;
		}
		offset += out_len;
	}

	if (sMethod == "POST") {
		out_len = snprintf(buffer + offset, len - offset, "\r\n%s", sPayload.c_str());
	}
	else {
		out_len = snprintf(buffer + offset, len - offset, "\r\n");
	}

	if (out_len <= 0 || out_len >= len - offset) {
		return -12;
	}

	offset += out_len;
	len = offset;
	return 0;
}

std::string & HTTPRequest::get_header(const std::string & key)
{
	return mpHeaders[key];
}
std::string & HTTPRequest::get_post_body()
{
	return sPayload;
}

std::string & HTTPRequest::get_uri()
{
	return sURL;
}

std::string & HTTPRequest::get_method()
{
	return sMethod;
}

int HTTPRequest::decode(char buffer[], int & len)
{
	std::string sData(buffer, len);

	int iRet = 0;
	size_t header_len = sData.find("\r\n\r\n");
	if (header_len == string::npos) {
		if (sData.size() > 10240) {
			return -30;
		}
		return 0;
	}

	int content_length = -1;
	for (size_t i = 0; i < header_len; i++) {
		size_t j = sData.find("\r\n", i);
		if (j == string::npos) {
			break;
		}
		if (i == 0) {
			// HTTP/1.1 200 OK
			char szMethod[128];
			char szURL[128];
			sscanf(sData.substr(i, j - i).c_str(), "%[^ ]s %[^ ]s HTTP/1.1", szMethod, szURL);
			sMethod = szMethod;
			sURL = szURL;
			continue;
		}

		string key, value;
		int iRet = SplitKeyValue(sData.substr(i, j - i), key, value);
		if (iRet != 0) {
			return -10;
		}

		mpHeaders[key] = value;

		if (key == "Content-Length") {
			content_length = atoi(value.c_str());
		}
	}

	if (content_length == -1) {
		return -20;
	}

	if (content_length + header_len + 4 <= sData.size()) {
		sPayload = sData.substr(header_len + 4, content_length);
		// printf("Resp: %s\n", sPayload.c_str());
		return content_length + header_len;
	}
	else {
		return 0;
	}
}

// HTTP 响应
HTTPResponse::HTTPResponse()
{
	iStatusCode = 0;
}

std::string & HTTPResponse::get_header(const std::string & key)
{
	return mpHeaders[key];
}

std::string &  HTTPResponse::get_body()
{
	return sPayload;
}

int HTTPResponse::get_status_code()
{
	return iStatusCode;
}


int HTTPResponse::decode(char buffer[], int len)
{
	std::string sData(buffer, len);

	int iRet = 0;
	size_t header_len = sData.find("\r\n\r\n");
	if (header_len == string::npos) {
		if (sData.size() > 10240) {
			return -30;
		}
		return 0;
	}

	int content_length = -1;
	for (size_t i = 0; i < header_len; i++) {
		size_t j = sData.find("\r\n", i);
		if (j == string::npos) {
			break;
		}
		if (i == 0) {
			// HTTP/1.1 200 OK
			sscanf(sData.substr(i, j - i).c_str(), "HTTP/1.1 %d", &iStatusCode);
			continue;
		}

		string key, value;
		int iRet = SplitKeyValue(sData.substr(i, j - i), key, value);
		if (iRet != 0) {
			return -10;
		}

		mpHeaders[key] = value;

		if (key == "Content-Length") {
			content_length = atoi(value.c_str());
		}
	}

	if (content_length == -1) {
		return -20;
	}

	if (content_length + header_len + 4 <= sData.size()) {
		sPayload = sData.substr(header_len + 4, content_length);
		// printf("Resp: %s\n", sPayload.c_str());
		return content_length + header_len;
	}
	else {
		return 0;
	}
}


int HTTPResponse::add_header(const std::string & key, const std::string & value)
{
	mpHeaders[key] = value;
	return 0;
}

int HTTPResponse::set_body(const std::string & payload)
{
	sPayload = payload;
	return 0;
}

int HTTPResponse::set_status_code(int iCode)
{
	iStatusCode = iCode;
	return 0;
}

int HTTPResponse::encode(char buffer[], int len)
{
	if (iStatusCode == 0) {
		iStatusCode = 200;
	}

	// TODO 返回字符串修改
	int offset = snprintf(buffer, len, "HTTP/1.1 %d OK", iStatusCode);
	if (offset <= 0 || offset >= len) {
		return -10;
	}

	char sContentLength[20] = {};
	snprintf(sContentLength, sizeof(sContentLength), "%lu", sPayload.size());
	mpHeaders["Content-Length"] = sContentLength;

	int out_len = 0;

	for (std::map<std::string, std::string>::iterator i = mpHeaders.begin(); i!= mpHeaders.end(); i++) {
		out_len = snprintf(buffer + offset, len - offset, "%s: %s\r\n", i->first.c_str(), i->second.c_str());
		if (out_len <= 0 || out_len >= len - offset) {
			return -11;
		}
		offset += out_len;
	}

	out_len = snprintf(buffer + offset, len - offset, "\r\n%s", sPayload.c_str());
	if (out_len <= 0 || out_len >= len - offset) {
		return -12;
	}

	offset += out_len;
	len = offset;
	return 0;
}
