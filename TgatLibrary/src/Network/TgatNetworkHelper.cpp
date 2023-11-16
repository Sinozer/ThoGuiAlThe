#include <cstdint>

#include "TgatNetworkHelper.h"

void TgatNetworkHelper::Send(SOCKET socket, char* dataBuf, TGATDATASIZE dataSize)
{
	Message message =
	{
		{ HEADER_ID, 0, dataSize },
		dataBuf
	};
	char* header = new char[HEADER_SIZE];
	WriteHeader(header, dataSize);

}

void TgatNetworkHelper::Receive(SOCKET socket, char* dataBuf, TGATDATASIZE dataSize)
{
	char* header = new char[HEADER_SIZE];
	ReadHeader(header);
}

TgatNetworkHelper::Message TgatNetworkHelper::CreateMessage(int protocol, TGATPLAYERID playerId, nlohmann::json& body)
{
	TgatNetworkHelper::Message message =
	{
		{ protocol, playerId, reinterpret_cast<TGATBODYSIZE>(body.dump().size()) },
		body.dump().c_str()
	};

	return message;
}

void TgatNetworkHelper::WriteHeader(MessageHeader& msgH, char* sendBuf)
{
	// Write header id
	memcpy(sendBuf, &msgH.Protocol, HEADER_PROTOCOL_SIZE);

	// Specify body size to header 
	memcpy(sendBuf + HEADER_PROTOCOL_SIZE, &msgH.PlayerId, sizeof(TGATPLAYERID));

	// Specify body size to header
	memcpy(sendBuf + HEADER_PROTOCOL_SIZE + sizeof(TGATPLAYERID), &msgH.BodySize, sizeof(TGATBODYSIZE));
}

TgatNetworkHelper::TGATBODYSIZE TgatNetworkHelper::ReadHeader(char* buffer)
{
	return 0;
}
