#pragma once

#include <QString>
#include "packet.h"

class ClientJoinedPacket : public Packet
{
public:
    ClientJoinedPacket();

    explicit ClientJoinedPacket(const QString& nickname);

    QString getNickname() const;

    size_t getSize() const override;

    char* serialize() const override;

    bool deserialize(const char* data) override;

private:
    QString m_nickname;
};