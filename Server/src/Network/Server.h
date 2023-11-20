#pragma once
#include "Player.h"
#include "Network/TgatNetworkHelper.h"

class Server : public TgatNetworkHelper
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

	void SendDataToPlayer(const Player& player, nlohmann::json& data);

	void HandleJson(const nlohmann::json& json);

	bool SendToAllClients(const char* message, int messageSize);

	std::unordered_set<Player>& GetPlayers() { return m_Players; }

private:
	HWND m_hWnd;
	char m_Port[5];
	std::unordered_set<Player> m_Players;

	void InitWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool PlayerIdCheck(TGATPLAYERID playerId) override;
};
