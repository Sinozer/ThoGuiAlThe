#pragma once
class UIButton
{
public:
	void SetCallback(std::function<void()> callback) { m_Callback = callback; }
	void OnClick() { if (m_Callback) m_Callback(); }
protected:
	std::function<void()> m_Callback;
};

