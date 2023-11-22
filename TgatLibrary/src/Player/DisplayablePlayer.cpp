#include "tgatLib.h"
#include "DisplayablePlayer.h"

DisplayablePlayer::DisplayablePlayer()
{
	PlayerDisplayData data = 
	{
		.name = "Player",
		.profilePicturePath = "DEFAULT_THUMB",
		.color = { 230, 50, 50, 255 },
	};
	m_DisplayData = data;
}
