#include "packets/client_joined_packet.h"

ClientJoinedPacket::ClientJoinedPacket()
        : Packet(2)
{}

ClientJoinedPacket::ClientJoinedPacket(const QString& nickname)
        : Packet(2), m_nickname(nickname)
{}

QString ClientJoinedPacket::getNickname() const
{
    return m_nickname;
}

size_t ClientJoinedPacket::getSize() const
{
    size_t sz = sizeof(getId());
    sz += sizeof(m_nickname.length());
    sz += m_nickname.length() + 1;

    return sz;
}

char* ClientJoinedPacket::serialize() const
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

bool ClientJoinedPacket::deserialize(const char* data)
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