
#include "http_static_handler.h"

#include <fstream>

HTTPStaticHandler::HTTPStaticHandler(const std::string & prefix, const std::string & root) : sPrefix(prefix), sRoot(root)
{
	mpContentType[".html"] = "text/html";
	mpContentType[".css"] = "text/css";
	mpContentType[".js"] = "application/x-javascript";

	if (sPrefix.size() > 0 && sPrefix[sPrefix.size() - 1] == '/') {
		sPrefix = sPrefix.substr(0, sPrefix.size() - 1);
	}
}

int HTTPStaticHandler::Process(const HTTPRequest & request, HTTPResponse & response)
{
	auto uri = request.get_uri();
	if (strncmp(uri.c_str(), sPrefix.c_str(), sPrefix.size()) != 0) {
		return 400;
	}

	auto path = sRoot + uri.substr(sPrefix.size());

	std::string content;
	std::ifstream file (path.c_str(), std::ios::binary | std::ios::in);
	if (!file) {
		return 404;
	}

	char c = 0;
	while (file.get(c)) {
		content.push_back(c);
	}

	response.set_body(content);

	int index = uri.find_last_of('.');
	if (index != -1) {
		auto suffix = uri.substr(index);
		if (mpContentType[suffix] != "") {
			response.add_header("Content-Type", mpContentType[suffix]);
		}
	}
	return 200;
}
