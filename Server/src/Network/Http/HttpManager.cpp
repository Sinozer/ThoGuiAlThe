#include "Network/Http/RequestHandlers/RequestHandler.h"

#include "HttpManager.h"

#include "HttpInclude.h"
#include "Network/Server.h"

HttpManager::HttpManager()
	: m_WebPort("9669"), m_WebServerSocket(INVALID_SOCKET),
	m_WebWindow(nullptr), m_Thread(),
	m_HttpRequestHandlers()
{
	InitHttpRequestHandlers();
}

HttpManager::~HttpManager()
{
	closesocket(m_WebServerSocket);
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

void HttpManager::StartWebServer()
{
	m_Thread = TgatThread([this]() { WebMain(); });
}

void HttpManager::CloseWebServer()
{
	SendMessage(m_WebWindow, MSG_NUKE, NULL, NULL);
	m_Thread.Join();
}

void HttpManager::InitHttpRequestHandlers()
{
	m_HttpRequestHandlers.insert({ "/", std::make_unique<HomeRequestHandler>() });
	m_HttpRequestHandlers.insert({ "/session", std::make_unique<SessionRequestHandler>() });
}

LRESULT HttpManager::WebWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
		case MSG_NUKE:
		{
			DestroyWindow(I(Server).GetHttpManager()->m_WebWindow);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case MSG_WEB:
		{
			if (WSAGETSELECTERROR(lParam))
			{
				LOG("FD_READ failed with error: " << WSAGetLastError());
				break;
			}

			switch (WSAGETSELECTEVENT(lParam))
			{
				case FD_ACCEPT:
				{
					SOCKET webClientSocket = accept(wParam, nullptr, nullptr);
					if (webClientSocket == INVALID_SOCKET)
					{
						LOG("accept failed with error: " << WSAGetLastError());
						break;
					}
					break;
				}
				case FD_READ:
				{
					char buffer[4096];
					int bytesReceived = recv((SOCKET)wParam, buffer, sizeof(buffer), 0);

					if (bytesReceived == SOCKET_ERROR)
					{
						LOG("recv failed with error: " << WSAGetLastError());
						closesocket((SOCKET)wParam);
						break;
					}
					else if (bytesReceived == 0)
					{
						// Connection closed by the client
						closesocket((SOCKET)wParam);
						LOG("Web client disconnected.");
						break;
					}

					buffer[bytesReceived] = '\0';

					// Parse the received HTTP request
					std::string httpRequest(buffer);
					// Process the HTTP request
					I(Server).GetHttpManager()->HandleHttpRequest(httpRequest, (SOCKET)wParam);

					closesocket(wParam);

					break;
				}
				case FD_CLOSE:
				{
					if (WSAGETSELECTERROR(lParam))
					{
						LOG("FD_READ failed with error: " << WSAGetLastError());
						break;
					}

					closesocket(wParam);

					break;
				}
			}
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // Call default message handler
}

void HttpManager::ProcessMessages()
{
	MSG msg{};
	while (GetMessage(&msg, m_WebWindow, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void HttpManager::WebMain()
{
	// Initialize the web server window
	I(Server).InitWindow(m_WebWindow, L"WebServerWindow", WebWndProc);

	// Initialize the web server socket
	I(Server).InitSocket(m_WebServerSocket, m_WebWindow, m_WebPort, MSG_WEB, FD_ACCEPT | FD_READ | FD_CLOSE);

	// Main loop
	ProcessMessages();

	// Close the web server socket
	closesocket(m_WebServerSocket);
	m_WebServerSocket = INVALID_SOCKET;

	// Unregister the web server window class
	UnregisterClass(L"WebServerWindow", GetModuleHandle(nullptr));
}
