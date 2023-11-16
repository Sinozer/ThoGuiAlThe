#include "HomeRequestHandler.h"
#pragma once

HomeRequestHandler::HomeRequestHandler()
{
	m_HttpMethodHandlers.insert({ "GET", std::make_unique<HomeMethodHandler>() });
}

std::string HomeRequestHandler::HandleHttpRequest(std::unordered_map<std::string, std::string>& params, const std::string& method)
{
	if (m_HttpMethodHandlers.contains(method) == false)
	{
		return RequestHandler::MethodNotAllowed();
	}
	return m_HttpMethodHandlers[method]->BuildResponse(params);
}

std::string HomeMethodHandler::BuildResponse(std::unordered_map<std::string, std::string>& params) const
{
	std::ifstream file("assets/index.html");
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n" + fileContent;
	return response;
}
