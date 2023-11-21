#pragma once
#include "Player/Player.h"
#include "Http/HttpInclude.h"
#include "Network/TgatNetworkHelper.h"

class HttpManager;
class GameNetworkManager;
class PlayerManager;

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

	HttpManager* GetHttpManager() const { return m_HttpManager; }
	GameNetworkManager* GetGameNetworkManager() const { return m_GameNetworkManager; }
	PlayerManager* GetPlayerManager() const { return m_PlayerManager; }

	void StartServer();
	void RunServer();
	void CloseServer();

	void AcceptNewPlayer(Player newPlayer);
	//void SendDataToPlayer(const Player& player, nlohmann::json& data);

	void HandleJson(const nlohmann::json& json);

	bool SendToAllClients(const char* message, int messageSize);

private:
	HttpManager* m_HttpManager;
	GameNetworkManager* m_GameNetworkManager;
	PlayerManager* m_PlayerManager;

	HWND m_hWnd;

	void InitSocket(SOCKET& socket, const char* port, uint32_t msgType, long events);

	void InitWindow();
	void ProcessMessages();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
