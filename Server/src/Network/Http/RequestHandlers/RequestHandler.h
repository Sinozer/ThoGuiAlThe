#pragma once
#include "Network/Http/IHandleMethod.h"

class RequestHandler
{
public:
	virtual ~RequestHandler() = default;

	virtual std::string HandleHttpRequest(std::unordered_map<std::string, std::string>& params, const std::string& method);

	static std::string MethodNotAllowed();
	static std::string NotFound();
	static std::string BadRequest();
	static std::string ServerError();
	static std::unordered_map<std::string, std::string> ParseParams(const std::string& paramString);
protected:
	std::unordered_map<std::string, std::unique_ptr<IHandleMethod>> m_HttpMethodHandlers;
};
