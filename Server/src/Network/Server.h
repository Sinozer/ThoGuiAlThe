#pragma once
class Server
{
public:
	Server();
	~Server();

	static Server& GetInstance()
	{
		static Server instance;
		return instance;
	}

	void StartServer();
	void ProcessMessages();
	void CloseServer();

	void AcceptNewClient(SOCKET& clientSocket);
	void CloseClient(SOCKET clientSocket);

	bool SendToClient(SOCKET clientSocket, const char* message, int messageSize);
	bool ReceiveFromClient(SOCKET clientSocket, char* buffer, int bufferSize);

	void HandleJson(const nlohmann::json& json);

	bool SendToAllClients(const char* message, int messageSize);
	bool ReceiveFromAllClients(char* buffer, int bufferSize);

private:
	HWND m_hWnd; // Handle to the window
	char m_Port[5];
	SOCKET m_ServerSocket;
	std::vector<SOCKET> m_ClientSockets;

	void InitWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
