#include "Network/Http/RequestHandlers/RequestHandler.h"

#include "HttpManager.h"

#include "Network/Http/RequestHandlers/HomeRequestHandler.h"

HttpManager::HttpManager() : m_WebServerSocket(INVALID_SOCKET), m_WebPort("9669")
{
	InitHttpRequestHandlers();
}

HttpManager::~HttpManager()
{
}

void HttpManager::HandleHttpRequest(std::string request, SOCKET socket)
{
	// Parse the HTTP request
	std::stringstream ss(request);
	std::string method;
	std::string url;
	std::string httpVersion;

	ss >> method >> url >> httpVersion;

	// Extracting parameters from the URL
	size_t paramsStart = url.find('?');
	std::string route = (paramsStart != std::string::npos) ? url.substr(0, paramsStart) : url;

	// Extracting parameters into a map
	std::unordered_map<std::string, std::string> params;
	if (paramsStart != std::string::npos)
	{
		params = RequestHandler::ParseParams(url.substr(paramsStart + 1));
	}

	// Process the HTTP request (this is where you would typically handle the request)
	std::string response;
	if (m_HttpRequestHandlers.contains(route) == true)
		response = m_HttpRequestHandlers[route]->HandleHttpRequest(params, method);
	else
		response = RequestHandler::NotFound();
	
	if (int r = send(socket, response.c_str(), (int)response.size(), 0); r == SOCKET_ERROR)
	{
		LOG("send failed with error: " << WSAGetLastError());
	}
	else
		LOG("send success");
}

void HttpManager::InitHttpRequestHandlers()
{
	m_HttpRequestHandlers.insert({ "/", std::make_unique<HomeRequestHandler>() });
}
