#pragma once 
#include <stdexcept>

class TgatException : public std::runtime_error
{
public:
	TgatException(const char* message) : std::runtime_error(message) {}
};