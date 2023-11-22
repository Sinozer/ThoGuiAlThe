#include "tgatLib.h"
#include "Player/Player.h"

#include "GameNetworkManager.h"

#include "Player/PlayerManager.h"
#include "Network/Http/HttpManager.h"
#include "Network/Server.h"
#include "Exceptions/TgatException.h"

#include "Game/Session/GameSession.h"

GameNetworkManager::GameNetworkManager() 
	: TgatNetworkHelper(), m_ServerSocket(INVALID_SOCKET), 
	m_Port("6969"), m_ThreadHandle(nullptr)
{
}

GameNetworkManager::~GameNetworkManager()
{
    closesocket(m_ServerSocket);
}

void GameNetworkManager::Init()
{
}

void GameNetworkManager::SendDataToPlayer(Player* player, nlohmann::json& data)
{
	try
	{
		TgatNetworkHelper::Message msg;
		std::string strData = data.dump();
		CreateMessage(HEADER_ID, player->GetId(), strData, msg);
		Send(player->GetSocket(), msg);
	}
	catch (const TgatException& e)
	{
		LOG(e.what());
	}
}

void GameNetworkManager::SendDataToAllPlayers(std::unordered_map<uint32_t, Player*>& players, nlohmann::json& data)
{
	for (const auto& p : players | std::views::values)
	{
		SendDataToPlayer(p, data);
	}
}

void GameNetworkManager::SendDataToAllPlayersInSession(GameSession* session, nlohmann::json& data)
{
	SendDataToAllPlayers(session->GetPlayers(), data);
}

bool GameNetworkManager::PlayerIdCheck(TGATPLAYERID playerId)
{
	// Check if the player id is in the list of connected players
	const auto& players = I(Server).GetPlayerManager()->GetPlayers();
	auto it = std::find_if(players.begin(), players.end(), [playerId](std::pair<uint32_t, Player*> player)
		{
			return player.second->GetId() == playerId;
		});

	return it != players.end();
}

bool GameNetworkManager::PlayerIdCheck(TGATPLAYERID playerId, GameSession* session)
{
	const auto& players = session->GetPlayers();
	auto it = std::find_if(players.begin(), players.end(), [playerId](std::pair<uint32_t, Player*> player)
		{
			return player.second->GetId() == playerId;
		});
	return it != players.end();
}

void GameNetworkManager::StartNetworkServer() 
{
	m_ThreadHandle = CreateThread(nullptr, 0, GameNetworkThread, this, 0, nullptr);
	if (m_ThreadHandle == nullptr)
	{
		LOG("CreateWebThread failed with error: " << GetLastError());
		throw std::exception("CreateThread failed");
	}
	else
		LOG("CreateWebThread success");
}

void GameNetworkManager::ProcessMessages()
{
	MSG msg{};
	while (GetMessage(&msg, I(Server).GetWindow(), 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

DWORD WINAPI GameNetworkManager::GameNetworkThread(LPVOID lpParam)
{
	GameNetworkManager* gameNetwork = static_cast<GameNetworkManager*>(lpParam);
	gameNetwork->GameNetworkMain();
	return 0;
}

void GameNetworkManager::GameNetworkMain()
{
	//Initialize the network server window
	Init();

	//Initialize the network server socket
	I(Server).InitSocket(m_ServerSocket, I(Server).GetWindow(), m_Port, MSG_WEB, FD_ACCEPT | FD_READ | FD_CLOSE);

	//Main loop
	ProcessMessages();

	//Close the network server socket
	closesocket(m_ServerSocket);
	m_ServerSocket = INVALID_SOCKET;

	//Unregister the network server window class
	UnregisterClass(L"WebNetworkServer", nullptr);
}




