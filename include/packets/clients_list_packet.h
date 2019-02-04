#pragma once

#include <QList>

#include "packet.h"

class ClientsListPacket : public Packet
{
public:
    ClientsListPacket();

    explicit ClientsListPacket(const QList<QString> clientsList);

    QList<QString> getClients() const;

    size_t getSize() const override;

    char* serialize() const override;

    void deserialize(const char* data) override;

private:
    QList<QString> m_clientsList;

    unsigned int m_nicknamesLength = 0;
};