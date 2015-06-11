
#ifndef HTTP_STATIC_HANDLER_H
#define HTTP_STATIC_HANDLER_H


#include "http_router.h"

class HTTPStaticHandler : public HTTPHandler
{
public:
	HTTPStaticHandler(const std::string & prefix, const std::string & root);
	virtual int Process(const HTTPRequest & request, HTTPResponse & response) override;
	
private:
	std::string sRoot;
	std::string sPrefix;
	std::map<std::string, std::string> mpContentType;
};


#endif // HTTP_STATIC_HANDLER_H

