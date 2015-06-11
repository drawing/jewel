
#include "http_router.h"


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

	if (mpLocations.find(uri) != mpLocations.end()) {
		code = mpLocations[uri]->Process(request, response);
	}
	else {
		for (auto pattern : mpPaths) {
			if (strncmp(pattern.first.c_str(), uri.c_str(), pattern.first.size()) == 0) {
				code = pattern.second->Process(request, response);
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
