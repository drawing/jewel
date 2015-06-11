
#include "static_router.h"

#include <fstream>

StaticRouter::StaticRouter(const std::string & prefix, const std::string & root) : sPrefix(root), sRoot(root)
{
    mpContentType[".html"] = "text/html";
    mpContentType[".css"] = "text/css";
    mpContentType[".js"] = "application/x-javascript";
    
    if (sPrefix.size() > 0 && sPrefix[sPrefix.size() - 1] == '/') {
        sPrefix = sPrefix.substr(0, sPrefix.size() - 1);
    }
}

int StaticRouter::Process(const HTTPRequest & request, HTTPResponse & response)
{
	auto uri = request.get_uri();
    if (uri.size() <= sPrefix.size() || sPrefix != uri.substr(0, sPrefix.size())) {
        return 400;
    }
    
    auto path = sRoot + uri.substr(sPrefix.size());
    
    std::string content;
    std::ifstream file (path.c_str(), std::ios::in);
    if (!file) {
	    return 404;
    }

    while (!file.eof()) {
	    content.push_back(file.get());
    }

    response.set_body(content);

    int index = uri.find_last_of('.');
    if (index != -1) {
	    auto suffix = uri.substr(index);
	    if (mpContentType[suffix] != "") {
		    response.add_header("Content-Type:", mpContentType[suffix]);
	    }
    }
    return 200;
}
