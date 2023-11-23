#include "RequestHandler.h"

std::string RequestHandler::MethodNotAllowed()
{
	return "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/plain\r\n\r\n405 Method Not Allowed";
}

std::string RequestHandler::NotFound()
{
	return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
}

std::string RequestHandler::BadRequest()
{
	return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
}

std::string RequestHandler::ServerError()
{
	return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\n500 Internal Server Error";
}

std::unordered_map<std::string, std::string> RequestHandler::ParseParams(const std::string& paramString)
{
	std::unordered_map<std::string, std::string> params;
	std::istringstream paramStream(paramString);
	std::string param;
	while (std::getline(paramStream, param, '&'))
	{
		size_t equalPos = param.find('=');
		if (equalPos != std::string::npos)
		{
			std::string key = param.substr(0, equalPos);
			std::string value = param.substr(equalPos + 1);
			params[key] = value;
		}
	}
	return params;
}
