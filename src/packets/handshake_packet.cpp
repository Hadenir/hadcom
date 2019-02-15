#include "packets/handshake_packet.h"

#include "serializer.h"
#include "deserializer.h"

HandshakePacket::HandshakePacket()
        : Packet(1)
{}

HandshakePacket::HandshakePacket(uint32_t magic, const QString& nickname)
        : Packet(1), m_magic(magic), m_nickname(nickname)
{}

uint32_t HandshakePacket::getMagic() const
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
    sz += sizeof(int);
    sz += m_nickname.toUtf8().length();

    return sz;
}

char* HandshakePacket::serialize() const
{
    auto data = new char[getSize()];

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