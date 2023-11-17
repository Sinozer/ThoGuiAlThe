#pragma once
class NetworkManager
{
public:
	static NetworkManager* GetInstance();

	static const void DestroyInstance();

	~NetworkManager();

	bool Connect();
	void Disconnect();

	void SendData(nlohmann::json& data);

	void HandleData(nlohmann::json& data);

	const bool IsConnected() const { return m_Connected; }

private:
	bool m_Connected = false;

	SOCKET m_Socket;
	addrinfo m_AddressInfo;
	HWND m_hWnd;
	uint32_t m_PlayerId;

	static NetworkManager* s_Instance;

private:
	NetworkManager()
		: m_Socket{INVALID_SOCKET}, m_AddressInfo{}
	{
		Init();
	}
	void CreateSocket();
	void Init();
	void InitWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};