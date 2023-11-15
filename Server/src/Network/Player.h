#pragma once
class Player
{
public:
    Player(SOCKET socket);
    ~Player() {};

    SOCKET GetSocket() const { return m_Socket; }
    uint32_t GetId() const { return m_Id; }

    bool operator==(const Player& other) const
    {
        return m_Socket == other.m_Socket && m_Id == other.m_Id;
    }

private:
    SOCKET m_Socket;
    uint32_t m_Id;

    static uint32_t s_IdCounter;
};

namespace std
{
    template <>
    struct hash<Player>
    {
        size_t operator()(const Player& player) const
        {
            // Use the hash value of the SOCKET as the hash value for the Player
            return hash<SOCKET>()(player.GetSocket());
        }
    };
}