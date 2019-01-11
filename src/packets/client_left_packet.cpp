#include "packets/client_left_packet.h"

ClientLeftPacket::ClientLeftPacket()
        : Packet(3)
{}

ClientLeftPacket::ClientLeftPacket(const QString& nickname)
        : Packet(3), m_nickname(nickname)
{}

QString ClientLeftPacket::getNickname() const
{
    return m_nickname;
}

size_t ClientLeftPacket::getSize() const
{
    size_t sz = 0;
    sz += sizeof(m_nickname.length());
    sz += m_nickname.length() + 1;

    return sz;
}

char* ClientLeftPacket::serialize() const
{
    char* data = new char[getSize()];
    unsigned int idx = 0;

    int length = m_nickname.length() + 1;
    memcpy(&data[idx], &length, sizeof(length));
    idx += sizeof(length);

    const char* nickname = m_nickname.toStdString().c_str();
    memcpy(&data[idx], nickname, length);

    return data;
}

bool ClientLeftPacket::deserialize(const char* data)
{
    unsigned int idx = 0;

    int length;
    memcpy(&length, &data[idx], sizeof(length));
    idx += sizeof(length);

    char nickname[length];
    memcpy(nickname, &data[idx], length);
    m_nickname = QString(nickname);

    return true;
}