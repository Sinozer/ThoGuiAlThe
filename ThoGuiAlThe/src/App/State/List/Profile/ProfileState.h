#pragma once
class ProfileState final : public State
{
public:
	ProfileState();
	~ProfileState() = default;

	void Init() override;
	void HandleEvents(sf::Event& event) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
	void Resume() override;
	void End() override;

private:
	void InitBackground();
	void RenderBackground(sf::RenderTarget* target);

	void InitUi() override;
	void HandleUiEvents(sf::Event& event) override;
	void UpdateUi(const float& dt) override;
	void RenderUi(sf::RenderTarget* target) override;

private:
	int m_ActualIndex = 0;
};

