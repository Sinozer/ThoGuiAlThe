#pragma once
class DisplayablePlayer
{
public:
	DisplayablePlayer();
	virtual ~DisplayablePlayer() = default;

	[[nodiscard]] const PlayerDisplayData& GetDisplayData() const { return m_DisplayData; }

	[[nodiscard]] const std::string& GetName() const { return m_DisplayData.name; }
	[[nodiscard]] const std::string& GetProfilePicturePath() const { return m_DisplayData.profilePicturePath; }
	[[nodiscard]] const std::array<uint32_t, 4>& GetBorderColor() const { return m_DisplayData.color; }

	void SetDisplayData(const PlayerDisplayData& data) { m_DisplayData = data; }

	void SetName(const std::string& name) { m_DisplayData.name = name; }
	void SetProfilePicturePath(const std::string& path) { m_DisplayData.profilePicturePath = path; }
	void SetBorderColor(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a) { m_DisplayData.color = { r, g, b, a }; }

private:
	PlayerDisplayData m_DisplayData;
};

