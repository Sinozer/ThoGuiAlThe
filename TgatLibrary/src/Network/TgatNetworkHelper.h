#pragma once

static class TgatNetworkHelper
{
public:

	struct MessageHeader
	{
		int Protocol;
		TGATPLAYERID PlayerId;
		TGATBODYSIZE BodySize;
	};

	struct Message
	{
		MessageHeader Header;
		const char* Body;
	};

public:
	static constexpr int HEADER_ID = 0x54474154; // "TGAT"
	static constexpr int HEADER_PROTOCOL_SIZE = sizeof(HEADER_ID);

	static constexpr TGATBODYSIZE HEADER_SIZE = HEADER_PROTOCOL_SIZE + sizeof(TGATBODYSIZE) + sizeof(TGATPLAYERID);

	void Send(SOCKET socket, Message msg);

	void Receive(SOCKET socket, Message msg);

	Message CreateMessage(int protocol, TGATPLAYERID playerId, nlohmann::json& body);

private:

	void static WriteHeader(MessageHeader& msgH, char* sendBuf);
	TGATBODYSIZE static ReadHeader(char* buffer);
};

static enum class TgatNetworkError
{
	SEND_ERROR = 0,
	RECEIVE_ERROR = 1,
	HEADER_ERROR = 2
};

static enum class TgatServerMessage
{
	PLAYER_INIT = 0, // {"eventType": "PLAYER_INIT", "Player" : "playerId": uuid(0)}
	PLAYER_DISCONNECT = 1, // {"eventType": "PLAYER_DISCONNECT", "Player" : "playerId": uuid(0)}
	PLAYER_INPUT = 2, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0, "p": 0}
	PLAYER_WIN = 3, // {"eventType": "PLAYER_WIN", "Player" : "playerId": uuid(0)}
};

static enum class TgatClientMessage
{
	PLAYER_INPUT = 0, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0}
};

