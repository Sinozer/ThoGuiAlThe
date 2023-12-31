#include <cstdint>

#include <functional>

#include "TgatNetworkHelper.h"

#include "Exceptions/TgatException.h"

TgatNetworkHelper::TgatNetworkHelper() : m_ServerSocket(INVALID_SOCKET)
{
}

TgatNetworkHelper::~TgatNetworkHelper()
{
}

void TgatNetworkHelper::Send(Message& msg)
{
	Send(m_ServerSocket, msg);
}

void TgatNetworkHelper::Send(SOCKET socket, Message& msg)
{
	const int bufSize = (int)(HEADER_SIZE + msg.Header.BodySize);
	const int headerSize = (int)sizeof(msg.Header);
	char* sendBuf = new char[bufSize];
	if (HEADER_SIZE != headerSize)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Header size error : Header was too long\n"));
	}
	memcpy(sendBuf, &msg.Header, headerSize);

	const int bodySize = (int)msg.Header.BodySize;
	if (bodySize != strlen(msg.Body))
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Body size error : Body was too long\n"));
	}
	memcpy(sendBuf + headerSize, msg.Body, bodySize);

	if (int r = send(socket, sendBuf, headerSize + bodySize, 0); r == SOCKET_ERROR)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Send Error : "));
	}
	delete[] sendBuf;
}

int TgatNetworkHelper::Receive(SOCKET socket, nlohmann::json& data)
{
	char* headerBuf = new char[HEADER_SIZE];
	int headerBytes = recv(socket, headerBuf, (int)HEADER_SIZE, 0);

	if (headerBytes == SOCKET_ERROR)
	{
		delete[] headerBuf;
		if (int err = WSAGetLastError(); err == WSAEWOULDBLOCK)
			return err;
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

	char* bodyBuf = new char[header.BodySize + 1];
	int byteReceived = recv(socket, bodyBuf, (int)header.BodySize, 0);
	bodyBuf[header.BodySize] = '\0'; 

	if (byteReceived != header.BodySize)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Receive Error, message was too long.\n"));
	}

	data = ReadMessage(bodyBuf);
	std::string str = data.dump();
	data.push_back({JSON_PLAYER_ID, header.Id});
	str = data.dump();

	delete[] bodyBuf;
	return true;
}

void TgatNetworkHelper::CreateMessage(int protocol, TGATPLAYERID playerId, std::string& strJson, Message& message)
{
	message.Header = { protocol, playerId, strlen(strJson.c_str()) };
	message.Body = strJson.c_str() + '\0';
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
		return {};
	}
}