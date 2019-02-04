#include "packets/chat_message_packet.h"

#include "serializer.h"
#include "deserializer.h"

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
    auto data = new char[getSize()];

    Serializer serializer(data);
    serializer.serializeField(m_sender);
    serializer.serializeField(m_message);

    return data;
}

void ChatMessagePacket::deserialize(const char* data)
{
    Deserializer deserializer(data);
    deserializer.deserializeField(m_sender);
}