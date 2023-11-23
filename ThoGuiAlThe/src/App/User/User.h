#pragma once

#include "Player/DisplayablePlayer.h"

class User : public DisplayablePlayer
{
public:
	User(TGATPLAYERID id) : m_Id(id) { }
	~User() = default;

	[[nodiscard]] TGATPLAYERID GetId() const { return m_Id; }

private:
	PlayerDisplayData m_displayData;
	TGATPLAYERID m_Id;
};

