int main()
{
    static constexpr char PORT[5] = "6969";

    SOCKET serverSocket = INVALID_SOCKET, listenSocket = INVALID_SOCKET;

    addrinfo* result = NULL;
    addrinfo hints{};

    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        LOG("WSAStartup failed with error: " << iResult);
        return 1;
    }
    else
        LOG("WSAStartup success");

    // Resolve address and port
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE; // For wildcard IP address


    iResult = getaddrinfo("localhost", PORT, &hints, &result);

    if (iResult != 0)
    {
        LOG("getaddrinfo failed with error: " << iResult);
        WSACleanup();
        return 1;
    }
    else
        LOG("getaddrinfo success");

    // Socket creation
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        LOG("socket failed with error: " << WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    else
        LOG("socket success");

    // Bind socket to listen to TCP requests on the given address and port
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        LOG("bind failed with error: " << WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    else
        LOG("bind success");
    
    freeaddrinfo(result);

    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        LOG("listen failed with error: " << WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    else
        LOG("listen success");

    // Accept a client socket
    serverSocket = accept(listenSocket, NULL, NULL);
    if (serverSocket == INVALID_SOCKET)
    {
        LOG("accept failed with error: " << WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    else
        LOG("accept success");

    // No longer need server socket
    closesocket(listenSocket);

    LOG("Server is running");

    WSACleanup();
    return 0;
}
