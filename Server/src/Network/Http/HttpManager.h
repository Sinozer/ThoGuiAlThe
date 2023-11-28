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
	void StartWebServer();
	void CloseWebServer();

private:
	char m_WebPort[5];
	SOCKET m_WebServerSocket;
	HWND m_WebWindow;
	TgatThread m_Thread;
	std::unordered_map<std::string, std::unique_ptr<RequestHandler>> m_HttpRequestHandlers;

private:
	void InitHttpRequestHandlers();

	static LRESULT CALLBACK WebWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessMessages();

	void WebMain();
};
