#include <cstdint>

#include <functional>

#include "TgatNetworkHelper.h"

#include "Exceptions/TgatException.h"

void TgatNetworkHelper::Send(SOCKET socket, Message& msg)
{
	const int bufSize = HEADER_SIZE + msg.Header.BodySize;
	const int headerSize = (int)sizeof(msg.Header);
	char* sendBuf = new char[bufSize];
	if (bufSize != headerSize)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Header size error : Header was too long\n"));
	}
	memcpy(sendBuf, &msg.Header, bufSize);

	const int bodySize = msg.Header.BodySize;
	if (bodySize != strlen(msg.Body))
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Body size error : Body was too long\n"));
	}
	memcpy(sendBuf + headerSize, msg.Body, bodySize);

	if (int r = send(socket, sendBuf, headerSize + bodySize, 0); r == SOCKET_ERROR)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Send Error : "));
	}
}

nlohmann::json TgatNetworkHelper::Receive(SOCKET socket)
{
	char* headerBuf = new char[HEADER_SIZE];
	int headerBytes = recv(socket, headerBuf, HEADER_SIZE, 0);

	if (headerBytes == SOCKET_ERROR)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Message size error : Message was empty or null\n"));
	}

	MessageHeader header;
	memcpy(&header, headerBuf, HEADER_SIZE);
	delete[] headerBuf;

	if (header.Protocol != HEADER_ID)
	{
		// /!\ CATCH THOSE EXCEPTIONS ON SERVER SIDE
		throw TgatException(TgatException::ErrorMessageBuilder("Protocol Error : "));
	}

	if (!PlayerIdCheck(header.Id))
	{
		throw TgatException(TgatException::ErrorMessageBuilder("ID Error : "));
	}

	char* bodyBuf = new char[header.BodySize];
	int byteReceived = recv(socket, bodyBuf, header.BodySize, 0);

	if (byteReceived != header.BodySize)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Receive Error, message was too long.\n"));
	}

	nlohmann::json jsonData = ReadMessage(bodyBuf);
	delete[] bodyBuf;

	return jsonData;
}

TgatNetworkHelper::Message TgatNetworkHelper::CreateMessage(int protocol, TGATPLAYERID playerId, nlohmann::json& body)
{
	TgatNetworkHelper::Message message =
	{
		{ protocol, playerId, body.dump().size() },
		body.dump().c_str()
	};

	return message;
}

nlohmann::json TgatNetworkHelper::ReadMessage(char* msg)
{
	try
	{
		nlohmann::json jsonData = nlohmann::json::parse(msg);
		return jsonData;
	}
	catch (const nlohmann::json::exception& e)
	{
		LOG("Error parsing JSON: " << e.what());
	}
}