
#ifndef STATIC_ROUTER_H
#define STATIC_ROUTER_H


#include "http_router.h"

class StaticRouter : public HTTPHandler
{
public:
	StaticRouter(const std::string & prefix, const std::string & root);
	virtual int Process(const HTTPRequest & request, HTTPResponse & response) override;
	
private:
	std::string sRoot;
	std::string sPrefix;
	std::map<std::string, std::string> mpContentType;
};


#endif // STATIC_ROUTER_H

