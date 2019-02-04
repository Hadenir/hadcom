#include "packets/handshake_packet.h"

#include <QDebug>

#include "serializer.h"
#include "deserializer.h"

HandshakePacket::HandshakePacket()
        : Packet(1)
{}

HandshakePacket::HandshakePacket(int magic, const QString& nickname)
        : Packet(1), m_magic(magic), m_nickname(nickname)
{}

int HandshakePacket::getMagic() const
{
    return m_magic;
}

QString HandshakePacket::getNickname() const
{
    return m_nickname;
}

size_t HandshakePacket::getSize() const
{
    size_t sz = 0;
    sz += sizeof(m_magic);
    sz += m_nickname.length() + 1;

    return sz;
}

char* HandshakePacket::serialize() const
{
    char* data = new char[getSize()];

    Serializer serializer(data);
    serializer.serializeField(m_magic);
    serializer.serializeField(m_nickname);

    return data;
}

void HandshakePacket::deserialize(const char* data)
{
    Deserializer deserializer(data);
    deserializer.deserializeField(m_magic);
    deserializer.deserializeField(m_nickname);
}