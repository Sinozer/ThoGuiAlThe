#include "SessionRequestHandler.h"
#include "Network/Server.h"
#include "Game/GameManager.h"

SessionRequestHandler::SessionRequestHandler()
{
	m_HttpMethodHandlers.insert({ "GET", std::make_unique<SessionMethodHandler>() });
}

std::string SessionRequestHandler::HandleHttpRequest(std::unordered_map<std::string, std::string>& params, const std::string& method)
{
	if (m_HttpMethodHandlers.contains(method) == false)
	{
		return RequestHandler::MethodNotAllowed();
	}
	return m_HttpMethodHandlers[method]->BuildResponse(params);
}

std::string SessionMethodHandler::BuildResponse(std::unordered_map<std::string, std::string>& params) const
{
	std::ifstream file("assets/session.html");
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	if (params.contains("session") == false)
		return RequestHandler::BadRequest();

	uint32_t sessionID = (uint32_t)std::stoi(params["session"]);

	I(Server).GetGameManager()->EnterCS();
	GameSession* session = I(Server).GetGameManager()->GetActiveSessionById(sessionID);
	if (session == nullptr)
		return RequestHandler::BadRequest();

	// Replace placeholders with actual values using regex
	for (int i = 0; i < GameSession::BOARD_SIZE; ++i)
	{
		for (int j = 0; j < GameSession::BOARD_SIZE; ++j)
		{
			// Generate the placeholder string that can be found in the html file
			std::string placeholder = "%VALUE_" + std::to_string(i) + std::to_string(j) + "%";
			std::regex pattern(placeholder);
			fileContent = std::regex_replace(fileContent, pattern, std::to_string(session->GetBoard()[i * GameSession::BOARD_SIZE + j]));
		}
	}
	I(Server).GetGameManager()->ExitCS();

	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n" + fileContent;
	return response;
}
