#pragma once
class Server
{
	public:
	Server();
	~Server();

	void StartServer();
	void CloseServer();

	void AcceptNewClient();
	void CloseClient(SOCKET clientSocket);

	bool SendToClient(SOCKET clientSocket, const char* message, int messageSize);
	bool ReceiveFromClient(SOCKET clientSocket, char* buffer, int bufferSize);

	bool SendToAllClients(const char* message, int messageSize);
	bool ReceiveFromAllClients(char* buffer, int bufferSize);

private:
	char m_Port[5];
	SOCKET m_ServerSocket;
	std::vector<SOCKET> m_ClientSockets;
};
