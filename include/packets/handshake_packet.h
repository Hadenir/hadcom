#pragma once

#include <QString>

#include "packet.h"

class HandshakePacket : public Packet
{
public:
    HandshakePacket();

    explicit HandshakePacket(uint32_t magic, const QString& nickname);

    uint32_t getMagic() const;

    QString getNickname() const;

    size_t getSize() const override;

    char* serialize() const override;

    void deserialize(const char* data) override;

private:
    uint32_t m_magic = 0;

    QString m_nickname;
};