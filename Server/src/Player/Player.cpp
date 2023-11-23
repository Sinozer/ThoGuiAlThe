#include "Player.h"

uint32_t Player::s_IdCounter = 1;

Player::Player(SOCKET socket)
{
	m_Socket = socket;
	m_Id = s_IdCounter++;
}