#include "tgatLib.h"
#include "Player/Player.h"

#include "GameNetworkManager.h"

#include "Player/PlayerManager.h"
#include "Network/Http/HttpManager.h"
#include "Network/Server.h"
#include "Exceptions/TgatException.h"

#include "Game/Session/GameSession.h"

GameNetworkManager::GameNetworkManager()
	: TgatNetworkHelper(), m_Port("6969"), m_ServerSocket(INVALID_SOCKET),
	m_ThreadID(0)
{
	m_ThreadHandle = nullptr;
	m_GameWindow = nullptr;
}

GameNetworkManager::~GameNetworkManager()
{
    closesocket(m_ServerSocket);
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

void GameNetworkManager::StartGameServer()
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

void GameNetworkManager::CloseGameServer()
{
	SendMessage(m_GameWindow, MSG_NUKE, 0, 0);
	WaitForSingleObject(m_ThreadHandle, INFINITE);

	CloseHandle(m_ThreadHandle);
}

void GameNetworkManager::ProcessMessages()
{
	MSG msg{};
	while (GetMessage(&msg, m_GameWindow, 0, 0))
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
	I(Server).InitWindow(m_GameWindow, L"GameNetworkWindow", GameWindowProc);

	//Initialize the network server socket
	I(Server).InitSocket(m_ServerSocket, m_GameWindow, m_Port, MSG_SERVER, FD_ACCEPT | FD_READ | FD_CLOSE);

	//Main loop
	ProcessMessages();

	//Close the network server socket
	closesocket(m_ServerSocket);
	m_ServerSocket = INVALID_SOCKET;

	//Unregister the network server window class
	UnregisterClass(L"GameNetworkWindow", nullptr);
}

LRESULT GameNetworkManager::GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
#pragma region ServerMessage
	case MSG_NUKE:
	{
		DestroyWindow(I(Server).GetGameNetworkManager()->GetWindow()); // Destroy the window
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case MSG_SERVER:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			LOG("FD_ACCEPT failed with error: " << WSAGetLastError());
			break;
		}

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
		{
			SOCKET clientSocket = accept(wParam, nullptr, nullptr);
			try
			{
				I(Server).AcceptNewPlayer(clientSocket); // Implicit construction of Player
			}
			catch (std::exception& e)
			{
				LOG(e.what());
				break;
			}
			catch (...)
			{
				LOG("Unknown exception");
				break;
			}
			break;
		}
		case FD_CLOSE:
		{
			LOG("FD_CLOSE");

			break;
		}
		}

		return 0;
	}
#pragma endregion
#pragma region ClientMessage
	case MSG_CLIENT:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			LOG("FD_READ failed with error: " << WSAGetLastError());
			break;
		}

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
			try
			{
				nlohmann::json jsonData;
				if (I(Server).GetGameNetworkManager()->Receive((SOCKET)wParam, jsonData) == WSAEWOULDBLOCK)
					LOG("WSAEWOULDBLOCK");
				else
					I(Server).HandleJson(jsonData);
			}
			catch (TgatException& e)
			{
				LOG(e.what());
				I(Server).GetPlayerManager()->RemovePlayer(wParam);
				break;
			}
			catch (nlohmann::json::exception& e)
			{
				LOG(e.what());
				I(Server).GetPlayerManager()->RemovePlayer(wParam);
				break;
			}
			catch (...)
			{
				LOG("Unknown exception");
				I(Server).GetPlayerManager()->RemovePlayer(wParam);
				break;
			}

			break;
		}
		case FD_WRITE:
			break;
		case FD_CLOSE:
			I(Server).GetPlayerManager()->RemovePlayer(wParam);
			break;
		}
		return 0;
	}
#pragma endregion
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // Call default message handler
}
