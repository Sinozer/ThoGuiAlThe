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
	int customValues[3][3] = {
	{00, 01, 02},
	{10, 11, 12},
	{20, 21, 22}
	};

	std::ifstream file("assets/index.html");
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	// Replace placeholders with actual values using regex
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			// Generate the placeholder string that can be found in the html file
			std::string placeholder = "%VALUE_" + std::to_string(i) + std::to_string(j) + "%";
			std::regex pattern(placeholder);
			fileContent = std::regex_replace(fileContent, pattern, std::to_string(customValues[i][j]));
		}
	}

	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n" + fileContent;
	return response;
}
