#pragma once
#include "RequestHandler.h"

class SessionRequestHandler : public RequestHandler
{
public:
	SessionRequestHandler();
	~SessionRequestHandler() {}

	virtual std::string HandleHttpRequest(std::unordered_map<std::string, std::string>& route, const std::string& method) override final;
};

class SessionMethodHandler : public IHandleMethod
{
public:
	SessionMethodHandler() {}
	~SessionMethodHandler() {}

	std::string BuildResponse(std::unordered_map<std::string, std::string>& params) const override final;
};
