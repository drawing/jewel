#ifndef HTTP_URI_ENC
#define HTTP_URI_ENC

#include <string>
#include <vector>
#include <map>

class HTTPHelper
{
public:
	static std::string URLEncode(const std::string & input);
	static bool URLDecode(const std::string & input, std::string & output);

	static std::vector<std::string> SplitString(const std::string & input, char sp);

	static std::map<std::string, std::string> GetParams(const std::string & uri);
};

#endif // HTTP_URI_ENC

