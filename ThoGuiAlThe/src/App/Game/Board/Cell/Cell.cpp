#include "tgatLib.h"
#include "Cell.h"

#include "Constants.h"
#include "App/Network/NetworkManager.h"

Cell::Cell(const sf::Vector2u position) : m_Player(nullptr), m_Position(position)
{
}

Cell::~Cell()
= default;

void Cell::Init()
{
	setSize(sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));
	setPosition(
		static_cast<float>(m_Position.x) * CELL_SIZE_X + WINDOW_SCREEN_WIDTH / 2 - getGlobalBounds().width / 2 * BOARD_SIZE_X,
		static_cast<float>(m_Position.y) * CELL_SIZE_Y + WINDOW_SCREEN_HEIGHT / 1.5f - getGlobalBounds().height / 1.5f * BOARD_SIZE_Y
	);
	setFillColor(sf::Color::Transparent);
	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Black);
}

void Cell::HandleEvents(sf::Event* event)
{
	std::cout << "Cell clicked: " << m_Position.x << " " << m_Position.y << std::endl;
	nlohmann::json eventData =
	{
		{"eventType", TgatClientMessage::PLAYER_INPUT},
		{"Move", {{"x", m_Position.x}, {"y", m_Position.y}}},
	};

	TgatNetworkHelper::Message msg;
	std::string strData = eventData.dump();
	const int headerId = NetworkManager::GetInstance().HEADER_ID;
	const int playerId = NetworkManager::GetInstance().GetPlayerId();
	NetworkManager::GetInstance().CreateMessage(headerId, playerId, strData, msg);
	NetworkManager::GetInstance().Send(msg);
}

void Cell::Update(const float& dt)
{
}

void Cell::Render(sf::RenderTarget* target)
{
	target->draw(*this);

	if (m_Player != nullptr)
	{
		sf::RectangleShape player(sf::Vector2f(PLAYER_SIZE_X, PLAYER_SIZE_Y));
		player.setFillColor(m_Player->GetColor());
		player.setPosition(
			getPosition().x + PLAYER_MARGIN_X,
			getPosition().y + PLAYER_MARGIN_Y
		);

		target->draw(player);
	}
}
