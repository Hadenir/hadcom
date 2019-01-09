#include <QDebug>
#include "packets/handshake_packet.h"


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
    sz += sizeof(m_nickname.length());
    sz += m_nickname.length() + 1;

    return sz;
}

char* HandshakePacket::serialize() const
{
    char* data = new char[getSize()];
    memset(data, 0, getSize());
    unsigned int idx = 0;

    memcpy(&data[idx], &m_magic, sizeof(m_magic));
    idx += sizeof(m_magic);

    int length = m_nickname.length() + 1;
    memcpy(&data[idx], &length, sizeof(length));
    idx += sizeof(length);

    const char* nickname = m_nickname.toStdString().c_str();
    memcpy(&data[idx], nickname, length);

    return data;
}

bool HandshakePacket::deserialize(const char* data)
{
    unsigned int idx = 0;

    memcpy(&m_magic, &data[idx], sizeof(m_magic));
    idx += sizeof(m_magic);

    int length;
    memcpy(&length, &data[idx], sizeof(length));
    idx += sizeof(length);

    char nickname[length];
    memcpy(nickname, &data[idx], length);
    m_nickname = QString(nickname);

    return true;
}