#include "packets/client_left_packet.h"

#include "serializer.h"
#include "deserializer.h"

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
    sz += m_nickname.length() + 1;

    return sz;
}

char* ClientLeftPacket::serialize() const
{
    char* data = new char[getSize()];

    Serializer serializer(data);
    serializer.serializeField(m_nickname);

    return data;
}

void ClientLeftPacket::deserialize(const char* data)
{
    Deserializer deserializer(data);
    deserializer.deserializeField(m_nickname);
}