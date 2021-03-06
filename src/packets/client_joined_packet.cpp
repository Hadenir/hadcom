#include "packets/client_joined_packet.h"

#include "serializer.h"
#include "deserializer.h"

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
    size_t sz = 0;
    sz += sizeof(int);
    sz += m_nickname.toUtf8().length();

    return sz;
}

char* ClientJoinedPacket::serialize() const
{
    auto data = new char[getSize()];

    Serializer serializer(data);
    serializer.serializeField(m_nickname);

    return data;
}

void ClientJoinedPacket::deserialize(const char* data)
{
    Deserializer deserializer(data);
    deserializer.deserializeField(m_nickname);
}