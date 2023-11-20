#pragma once

class TgatNetworkHelper
{
public:

	struct MessageHeader
	{
		int Protocol;
		TGATPLAYERID Id;
		TGATBODYSIZE BodySize;
	};

	struct Message
	{
		MessageHeader Header;
		const char* Body;
	};

public:
	TgatNetworkHelper();
	virtual ~TgatNetworkHelper();

public:
	const int HEADER_ID = 0x54474154; // "TGAT"
	const int HEADER_PROTOCOL_SIZE = sizeof(HEADER_ID);

	const TGATBODYSIZE HEADER_SIZE = HEADER_PROTOCOL_SIZE + sizeof(TGATBODYSIZE) + sizeof(TGATPLAYERID);

	void Send(Message& msg);
	void Send(SOCKET socket, Message& msg);

	[[nodiscard]] int Receive(SOCKET socket, nlohmann::json& data);

	void CreateMessage(int protocol, TGATPLAYERID playerId, std::string& strJson, Message& message);
	nlohmann::json ReadMessage(char* msg);

protected:
	SOCKET m_ServerSocket;

protected:
	virtual bool PlayerIdCheck(TGATPLAYERID playerId) = 0;
};

enum class TgatNetworkError
{
	SEND_ERROR = 0,
	RECEIVE_ERROR = 1,
	HEADER_ERROR = 2
};

enum class TgatServerMessage
{
	PLAYER_INIT = 0, // {"eventType": "PLAYER_INIT", "Player" : "playerId": uuid(0)}
	PLAYER_DISCONNECT = 1, // {"eventType": "PLAYER_DISCONNECT", "Player" : "playerId": uuid(0)}
	PLAYER_INPUT = 2, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0, "p": 0}
	PLAYER_WIN = 3, // {"eventType": "PLAYER_WIN", "Player" : "playerId": uuid(0)}
};

enum class TgatClientMessage
{
	PLAYER_INPUT = 0, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0}
};

