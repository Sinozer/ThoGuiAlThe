#pragma once
#include "RequestHandler.h"

class HomeRequestHandler : public RequestHandler
{
public:
	HomeRequestHandler();
	~HomeRequestHandler() {}
};

class HomeMethodHandler : public IHandleMethod
{
public:
	~HomeMethodHandler() final = default;

	std::string BuildResponse(std::unordered_map<std::string, std::string>& route) const override final;
};