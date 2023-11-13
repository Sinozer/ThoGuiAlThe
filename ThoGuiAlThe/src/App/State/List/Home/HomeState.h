#pragma once

class HomeState final : public State
{
public:
	HomeState();
	~HomeState() override = default;

	void Init() override;
	void End() override;

	void HandleEvents(sf::Event& event) override;

	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
	
private:
	void InitBackground() override;

	void InitUi() override;
	void HandleUiEvents(sf::Event& event) override;
	void UpdateUi(const float& dt) override;
	void RenderUi(sf::RenderTarget* target) override;
};