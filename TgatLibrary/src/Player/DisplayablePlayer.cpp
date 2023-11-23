#include "tgatLib.h"
#include "DisplayablePlayer.h"

DisplayablePlayer::DisplayablePlayer()
{
	PlayerDisplayData data = 
	{
		.name = "Player",
		.profilePicturePath = "DEFAULT",
		.profilePictureThumbPath = "DEFAULT_THUMB",
		.color = { 231, 111, 81, 255 },
	};
	m_DisplayData = data;
}
