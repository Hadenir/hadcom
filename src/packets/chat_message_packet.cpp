#include "packets/chat_message_packet.h"

ChatMessagePacket::ChatMessagePacket()
        : Packet(5)
{}

ChatMessagePacket::ChatMessagePacket(const QString& sender, const QString& message)
        : Packet(5), m_sender(sender), m_message(message)
{}

QString ChatMessagePacket::getSender() const
{
    return m_sender;
}

QString ChatMessagePacket::getMessage() const
{
    return m_message;
}

size_t ChatMessagePacket::getSize() const
{
    size_t sz = 0;
    sz += m_sender.length() + 1;
    sz += m_message.length() + 1;

    return sz;
}

char* ChatMessagePacket::serialize() const
{
    char* data = new char[getSize()];
    unsigned int idx = 0;

    int length = m_sender.length() + 1;
    const char* sender = m_sender.toStdString().c_str();
    memcpy(&data[idx], sender, length);
    idx += length;

    length = m_message.length() + 1;
    const char* message = m_message.toStdString().c_str();
    memcpy(&data[idx], message, length);
    idx += length;

    return data;
}

bool ChatMessagePacket::deserialize(const char* data)
{
    unsigned int idx = 0;

    QString sender(&data[idx]);
    m_sender = sender;
    idx += sender.length() + 1;

    QString message(&data[idx]);
    m_message = message;
    idx += message.length() + 1;

    return true;
}