#include "packets/clients_list_packet.h"

#include "serializer.h"
#include "deserializer.h"

ClientsListPacket::ClientsListPacket()
        : Packet(4)
{}

ClientsListPacket::ClientsListPacket(const QList<QString> clientsList)
        : Packet(4), m_clientsList(clientsList)
{
    for(const auto& nickname : clientsList)
        m_nicknamesLength += nickname.length() + 1;
}

QList<QString> ClientsListPacket::getClients() const
{
    return m_clientsList;
}

size_t ClientsListPacket::getSize() const
{
    size_t sz = 0;
    sz += sizeof(m_clientsList.size());
    sz += m_nicknamesLength;

    return sz;
}

char* ClientsListPacket::serialize() const
{
    char* data = new char[getSize()];

    Serializer serializer(data);
    serializer.serializeField(m_clientsList);

    return data;
}

void ClientsListPacket::deserialize(const char* data)
{

    Deserializer deserializer(data);
    deserializer.deserializeField(m_clientsList);
}