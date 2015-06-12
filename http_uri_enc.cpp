#include "http_uri_enc.h"

std::string HTTPHelper::URLEncode(const std::string & input)
{
	std::string output;

	unsigned char hexchars[] = "0123456789ABCDEF";

	for (size_t i = 0; i < input.size(); i++) {
		uint8_t c = input[i];

		if (c == ' ') {
			output.push_back('+');
		} else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			output.push_back(c);
		} else {
			output.push_back('%');
			output.push_back(hexchars[c >> 4]);
			output.push_back(hexchars[c & 15]);
		}
	}
	return std::move(output);
}

bool HTTPHelper::URLDecode(const std::string & input, std::string & output)
{
	output = "";
	for (size_t i = 0; i < input.size(); i++) {
		uint8_t c = input[i];

		if (c == '+') {
			output.push_back(' ');
		} else if (c == '%') {
			i ++;
			if (i >= input.size()) {
				return false;
			}

			c = input[i];

			if (isupper(c))
				c = tolower(c);

			uint8_t value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

			i ++;
			if (i >= input.size()) {
				return false;
			}

			c = input[i];
			if (isupper(c))
				c = tolower(c);

			value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;
			output.push_back(value);

		} else {
			output.push_back(c);
		}
	}

	return true;
}

std::vector<std::string> HTTPHelper::SplitString(const std::string & input, char sp)
{
	std::vector<std::string> result;

	std::string current;

	for (size_t i = 0; i < input.size(); i++) {
		if (input[i] != sp) {
			current.push_back(input[i]);
		}
		else {
			result.push_back(std::move(current));
			current.clear();
		}
	}

	result.push_back(std::move(current));
	return std::move(result);
}

std::map<std::string, std::string> HTTPHelper::GetParams(const std::string & uri)
{
	std::map<std::string, std::string> mpParams;

	int pos = uri.find_first_of('?');
	if (pos != -1) {
		auto sparam = uri.substr(pos + 1);
		auto vparam = HTTPHelper::SplitString(sparam, '&');
		for (auto v : vparam) {
			auto kv = HTTPHelper::SplitString(v, '=');
			if (kv.size() != 2) {
				continue;
			}
			std::string value;
			bool bRet = HTTPHelper::URLDecode(kv[1], value);
			if (bRet) {
				mpParams[kv[0]] = value;
			}
		}
	}

	return std::move(mpParams);
}

