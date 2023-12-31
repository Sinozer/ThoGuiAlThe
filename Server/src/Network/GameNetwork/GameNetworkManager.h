#pragma once

#include "Network/TgatNetworkHelper.h"
#include "Player/Player.h"

class GameSession;
class Player;


class GameNetworkManager : public TgatNetworkHelper
{
	friend class Server;
public:
	GameNetworkManager();
	~GameNetworkManager() override;

public:
	[[nodiscard]] SOCKET& GetSocket() { return m_ServerSocket; }
	[[nodiscard]] char* GetPort() { return m_Port; }
	[[nodiscard]] HWND& GetWindow() { return m_GameWindow; }
	
	void StartGameServer();
	void CloseGameServer();

	void SendDataToPlayer(Player* player, nlohmann::json& data);
	void SendDataToAllPlayers(std::unordered_map<uint32_t, Player*>& players, nlohmann::json& data);
	void SendDataToAllPlayersInSession(GameSession* session, nlohmann::json& data);

private:
	char m_Port[5];

	HWND m_GameWindow;
	TgatThread m_Thread;

	bool PlayerIdCheck(TGATPLAYERID playerId) override;
	bool PlayerIdCheck(TGATPLAYERID playerId, GameSession* session);

	void ProcessMessages();
	void GameNetworkMain();

	static LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
