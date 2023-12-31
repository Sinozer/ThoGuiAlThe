#include "HomeRequestHandler.h"
#include "Network/Server.h"
#include "Game/GameManager.h"

HomeRequestHandler::HomeRequestHandler()
{
	m_HttpMethodHandlers.insert({ "GET", std::make_unique<HomeMethodHandler>() });
}

std::string HomeMethodHandler::BuildResponse(std::unordered_map<std::string, std::string>& params) const
{
	std::ifstream file("assets/index.html");
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::regex pattern("%SESSIONS%");
	std::stringstream sessionLink;
	{
		CriticalSectionScope cs = I(Server).GetGameManager()->GetActiveSessionsCSScope();
		try {
			const auto& sessions = I(Server).GetGameManager()->GetActiveSessions();
			for (auto&[sessionId, session] : sessions)
			{
				std::string idStr = std::to_string(sessionId);
				std::array<std::string, 2> playerNames;
				uint8_t index = 0;
				for (auto&[_, player] : session->GetPlayers())
				{
					playerNames.at(index++) = player->GetName();
				}
				sessionLink << "<a href=\"/session?session=" << idStr << "\">" << idStr
					<< ": " << (playerNames[0]) << " VS " << (playerNames[1]) << "</a><br>";
			}
		}
		catch (...)
		{
			return RequestHandler::ServerError();
		}
	}

	fileContent = std::regex_replace(fileContent, pattern, sessionLink.str());

	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n" + fileContent;
	return response;
}
