#pragma once

#include <QString>

#include "packet.h"

class HandshakePacket : public Packet
{
public:
    HandshakePacket();

    explicit HandshakePacket(int magic, const QString& nickname);

    int getMagic() const;

    QString getNickname() const;

    size_t getSize() const override;

    char* serialize() const override;

    bool deserialize(const char* data) override;

private:
    int m_magic = 0;

    QString m_nickname;
};