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



