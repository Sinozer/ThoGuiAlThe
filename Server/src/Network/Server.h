#pragma once
#include "Player.h"
#include "Http/HttpInclude.h"
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
	void InitSocket(SOCKET& socket, const char* port, uint32_t msgType, long events);
	void ProcessMessages();
	void CloseServer();

	void AcceptNewPlayer(Player newPlayer);
	void RemovePlayer(Player& player);
	void RemovePlayer(SOCKET socket);

	void SendDataToPlayer(const Player& player, nlohmann::json& data);

	void HandleJson(const nlohmann::json& json);

	void HandleHttpRequest(std::string request, SOCKET socket);

	bool SendToAllClients(const char* message, int messageSize);

	std::unordered_set<Player>& GetPlayers() { return m_Players; }

private:
	HWND m_hWnd;
	char m_Port[5];
	char m_WebPort[5];
	SOCKET m_WebServerSocket;
	std::unordered_set<Player> m_Players;
	std::unordered_map<std::string, std::unique_ptr<RequestHandler>> m_HttpRequestHandlers;

	void InitWindow();
	void InitHttpRequestHandlers();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool PlayerIdCheck(TGATPLAYERID playerId) override;
};
