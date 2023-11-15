#pragma once
#include "Player.h"

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

	void AcceptNewPlayer(Player newPlayer);
	void RemovePlayer(Player& player);
	void RemovePlayer(SOCKET socket);

	bool SendToClient(SOCKET clientSocket, const char* message, int messageSize);

	void HandleJson(const nlohmann::json& json);

	bool SendToAllClients(const char* message, int messageSize);

	std::unordered_set<Player>& GetPlayers() { return m_Players; }

private:
	HWND m_hWnd; // Handle to the window
	char m_Port[5];
	SOCKET m_ServerSocket;
	std::unordered_set<Player> m_Players;

	void InitWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
