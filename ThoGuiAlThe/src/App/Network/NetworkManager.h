class NetworkManager
{
public:
	static NetworkManager& GetInstance()
	{
		static NetworkManager instance;
		return instance;
	}

	static void DestroyInstance()
	{
		GetInstance().Destroy();
	}

	void Init();
	void Destroy();

	void Connect();
	void Disconnect();

	void SendData();

	void ReceiveData();

private:
	SOCKET m_Socket;
	addrinfo m_AddressInfo;

	NetworkManager()
		: m_Socket{INVALID_SOCKET}, m_AddressInfo{}
	{
		Init();
	}
};