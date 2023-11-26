#pragma once
#include "RequestHandler.h"

class SessionRequestHandler : public RequestHandler
{
public:
	SessionRequestHandler();
	~SessionRequestHandler() {}
};

class SessionMethodHandler : public IHandleMethod
{
public:
	SessionMethodHandler() {}
	~SessionMethodHandler() {}

	std::string BuildResponse(std::unordered_map<std::string, std::string>& params) const override final;
};
