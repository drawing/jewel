#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "db_http.h"

#include <map>
#include <string>

class HTTPHandler
{
public:
	virtual int Process(const HTTPRequest & request, HTTPResponse & response) = 0;
};

class HTTPRouter
{
public:
	bool Register(const std::string & pattern, HTTPHandler * handler);
	bool Handle(const HTTPRequest & request, HTTPResponse & response);
private:
	std::map<std::string, HTTPHandler*, std::greater<std::string>> mpPaths;
	std::map<std::string, HTTPHandler*> mpLocations;
};


#endif // HTTP_ROUTER_H

