
#include "http_router.h"

#include <cstring>

bool HTTPRouter::Register(const std::string & pattern, HTTPHandler * handler)
{
	if (pattern.empty()) {
		return false;
	}

	if (pattern[pattern.size() - 1] == '/') {
		if (mpPaths.find(pattern) != mpPaths.end()) {
			return false;
		}
		mpPaths[pattern] = handler;
	}
	else {
		if (mpLocations.find(pattern) != mpLocations.end()) {
			return false;
		}
		mpLocations[pattern] = handler;
	}
	return true;
}

bool HTTPRouter::Handle(const HTTPRequest & request, HTTPResponse & response)
{
	int code = 404;

	auto uri = request.get_uri();
	int pos = uri.find_first_of('?');
	if (pos != -1) {
		uri = uri.substr(0, pos);
	}

	if (mpLocations.find(uri) != mpLocations.end()) {
		code = mpLocations[uri]->Process(request, response);
	}
	else {
		for (auto it = mpPaths.begin(); it != mpPaths.end(); it++) {
			if (strncmp(it->first.c_str(), uri.c_str(), it->first.size()) == 0) {
				code = it->second->Process(request, response);
				break;
			} 
		}
	}

	if (code == 0) {
		response.set_status_code(200);
	}
	else {
		response.set_status_code(code);
	}

	response.set_status_code(code);

	return true;
}
