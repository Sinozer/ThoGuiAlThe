#pragma once

class SelectState final : public State
{
public:
	SelectState();
	~SelectState() = default;

	void Init();
	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target = nullptr);
	void Resume() override;
	void End();

private:
	void InitBackground();
	void RenderBackground(sf::RenderTarget* target);

	void InitUi() override;
	void HandleUiEvents(sf::Event& event) override;
	void UpdateUi(const float& dt) override;
	void RenderUi(sf::RenderTarget* target) override;
};