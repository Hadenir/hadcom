#pragma once

#include <QString>
#include "packet.h"

class ClientLeftPacket : public Packet
{
public:
    ClientLeftPacket();

    explicit ClientLeftPacket(const QString& nickname);

    QString getNickname() const;

    size_t getSize() const override;

    char* serialize() const override;

    void deserialize(const char* data) override;

private:
    QString m_nickname;
};
