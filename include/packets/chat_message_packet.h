#pragma once

#include <QString>
#include "packet.h"

class ChatMessagePacket : public Packet
{
public:
    ChatMessagePacket();

    explicit ChatMessagePacket(const QString& sender, const QString& message);

    QString getSender() const;

    QString getMessage() const;

    size_t getSize() const override;

    char* serialize() const override;

    bool deserialize(const char* data) override;

private:
    QString m_sender;

    QString m_message;
};