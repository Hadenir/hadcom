#include "packets/clients_list_packet.h"

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
    unsigned int idx = 0;

    // int size = m_clientsList.size();
    // memcpy(&data[idx], &size, sizeof(size));
    // idx += sizeof(size);

    memcpy(&data[idx], &m_nicknamesLength, sizeof(m_nicknamesLength));
    idx += sizeof(m_nicknamesLength);

    for(const auto& client : m_clientsList)
    {
        int length = client.length() + 1;
        const char* nickname = client.toStdString().c_str();
        memcpy(&data[idx], nickname, length);
        idx += length;
    }

    return data;
}

bool ClientsListPacket::deserialize(const char* data)
{
    unsigned int idx = 0;

    int size;
    memcpy(&size, &data[idx], sizeof(size));
    idx += sizeof(size);

    char buff[size];
    memcpy(buff, &data[idx], size);

    int i = 0;
    while(i < size)
    {
        QString nickname(&buff[i]);
        m_clientsList.push_back(nickname);
        i += nickname.length() + 1;
    }

    return true;
}