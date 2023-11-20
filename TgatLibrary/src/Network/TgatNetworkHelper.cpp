#include <cstdint>

#include <functional>

#include "TgatNetworkHelper.h"

#include "Exceptions/TgatException.h"

TgatNetworkHelper::TgatNetworkHelper() : m_Socket(INVALID_SOCKET)
{
}

TgatNetworkHelper::~TgatNetworkHelper()
{
}

void TgatNetworkHelper::Send(Message& msg)
{
	Send(m_Socket, msg);
}

void TgatNetworkHelper::Send(SOCKET socket, Message& msg)
{
	const int bufSize = HEADER_SIZE + msg.Header.BodySize;
	const int headerSize = (int)sizeof(msg.Header);
	char* sendBuf = new char[bufSize];
	if (HEADER_SIZE != headerSize)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Header size error : Header was too long\n"));
	}
	memcpy(sendBuf, &msg.Header, headerSize);

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

	char* bodyBuf = new char[header.BodySize + 1];
	int byteReceived = recv(socket, bodyBuf, header.BodySize, 0);
	bodyBuf[header.BodySize] = '\0'; 

	if (byteReceived != header.BodySize)
	{
		throw TgatException(TgatException::ErrorMessageBuilder("Receive Error, message was too long.\n"));
	}

	nlohmann::json jsonData = ReadMessage(bodyBuf);
	delete[] bodyBuf;

	return jsonData;
}

void TgatNetworkHelper::CreateMessage(int protocol, TGATPLAYERID playerId, std::string& strJson, Message& message)
{
	// So, we need to convert the json to a string, then to a char* because ?????????????

	message.Header = { protocol, playerId, strlen(strJson.c_str()) };
	message.Body = strJson.c_str() + '\0'; // ?????	
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