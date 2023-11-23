#pragma once 
#include <stdexcept>

class TgatException : public std::runtime_error
{
public:
	TgatException(const char* message) : std::runtime_error(message) {}

	static const char* ErrorMessageBuilder(const char* message)
	{
		return message + WSAGetLastError();
	}
};