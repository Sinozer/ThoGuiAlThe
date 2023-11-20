#pragma once

class RequestHandler;

class HttpManager
{
public:
	HttpManager();
	~HttpManager();

public:
	void HandleHttpRequest(std::string request, SOCKET socket);
	[[nodiscard]] char* GetWebPort() { return m_WebPort; }
	[[nodiscard]] SOCKET GetWebServerSocket() { return m_WebServerSocket; }

private:
	char m_WebPort[5];
	SOCKET m_WebServerSocket;
	std::unordered_map<std::string, std::unique_ptr<RequestHandler>> m_HttpRequestHandlers;
	void InitHttpRequestHandlers();
};

