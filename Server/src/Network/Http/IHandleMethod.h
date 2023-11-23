#pragma once

class IHandleMethod
{
public:
	virtual ~IHandleMethod() = default;
	virtual std::string BuildResponse(std::unordered_map<std::string, std::string>& params) const = 0;
};
