#pragma once
class CreateState : public State
{
public:
	CreateState();
	~CreateState() override = default;

	void Init() override;
	void HandleEvents(sf::Event& event) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target) override;

	void Resume() override;
	void End() override;

private:
	void InitBackground() override;

	void InitUi() override;
	void HandleUiEvents(sf::Event& event) override;
	void UpdateUi(const float& dt) override;
	void RenderUi(sf::RenderTarget* target) override;
};