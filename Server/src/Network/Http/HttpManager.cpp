#include "Network/Http/RequestHandlers/RequestHandler.h"

#include "HttpManager.h"

#include "Network/Http/RequestHandlers/HomeRequestHandler.h"
#include "Network/Server.h"

HttpManager::HttpManager()
	: m_WebPort("9669"), m_WebServerSocket(INVALID_SOCKET),
	m_WebWindow(nullptr),
	m_ThreadID(0), m_ThreadHandle(nullptr), m_Running(true),
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
	m_ThreadHandle = CreateThread(nullptr, 0, WebServerThread, this, 0, &m_ThreadID);
	if (m_ThreadHandle == nullptr)
	{
        LOG("CreateWebThread failed with error: " << GetLastError());
        throw std::exception("CreateThread failed");
    }
    else
        LOG("CreateWebThread success");
}

void HttpManager::CloseWebServer()
{
	SendMessage(m_WebWindow, WM_DESTROY, NULL, NULL);
	WaitForSingleObject(m_ThreadHandle, INFINITE);

	CloseHandle(m_ThreadHandle);
}

void HttpManager::InitHttpRequestHandlers()
{
	m_HttpRequestHandlers.insert({ "/", std::make_unique<HomeRequestHandler>() });
}

void HttpManager::InitWebWindow()
{
	WNDCLASSEX wcex =
	{
		.cbSize = sizeof(WNDCLASSEX),
		.style = 0,
		.lpfnWndProc = WebWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = GetModuleHandle(nullptr),
		.hIcon = nullptr,
		.hCursor = nullptr,
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = L"WebServerWindow",
		.hIconSm = nullptr
	};
	RegisterClassEx(&wcex);

	m_WebWindow = CreateWindowEx(0, L"WebServerWindow", L"WebServerWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, GetModuleHandle(nullptr), nullptr);

	if (m_WebWindow == nullptr)
	{
		LOG("CreateWindowEx failed with error: " << GetLastError());
		throw std::exception("CreateWindowEx failed");
	}
	else
		LOG("CreateWindowEx success");
}

LRESULT HttpManager::WebWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			I(Server).GetHttpManager()->Shutdown();
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
	while (PeekMessage(&msg, m_WebWindow, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// Add a sleep to avoid busy-waiting
	Sleep(1);
}

DWORD WINAPI HttpManager::WebServerThread(LPVOID lpParam)
{
	HttpManager* httpManager = static_cast<HttpManager*>(lpParam);
	httpManager->WebMain();
	return 0;
}

void HttpManager::WebMain()
{
	// Initialize the web server window
	InitWebWindow();

	// Initialize the web server socket
	I(Server).InitSocket(m_WebServerSocket, m_WebWindow, m_WebPort, MSG_WEB, FD_ACCEPT | FD_READ | FD_CLOSE);

	// Main loop
	while (m_Running)
	{
		// Process messages
		ProcessMessages();
	}

	// Close the web server socket
	closesocket(m_WebServerSocket);
	m_WebServerSocket = INVALID_SOCKET;

	// Unregister the web server window class
	UnregisterClass(L"WebServerWindow", GetModuleHandle(nullptr));
}
