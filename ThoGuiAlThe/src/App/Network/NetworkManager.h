class NetworkManager
{
public:
	static NetworkManager& GetInstance();

	static void DestroyInstance()
	{
		if (s_Instance != nullptr)
			s_Instance->Destroy();
	}

	void Destroy();

	void Connect();
	void Disconnect();

	void SendData(std::string data);
	void SendData(nlohmann::json& data);

	void HandleData(nlohmann::json& data);

private:
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
	void Init();
	void InitWindow();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};