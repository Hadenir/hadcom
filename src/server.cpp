#include "server.h"

#include "connection.h"
#include "remote_client.h"
#include "packets/handshake_packet.h"
#include "packets/client_joined_packet.h"
#include "packets/client_left_packet.h"
#include "packets/clients_list_packet.h"
#include "packets/chat_message_packet.h"

Server::Server(QObject* parent)
        : QTcpServer(parent)
{}

bool Server::bind(quint16 port)
{
    return listen(QHostAddress::Any, port);
}

bool Server::broadcast(const Packet& packet) const
{
    for(auto connection : m_clients.keys())
    {
        connection->send(packet);
    }
}

QList<QString> Server::getClientsList() const
{
    QList<QString> clientsList;
    clientsList.reserve(m_clients.size());

    for(auto client : m_clients)
        clientsList.push_back(client->getNickname());

    return clientsList;
}

void Server::incomingConnection(qintptr descriptor)
{
    auto connection = new Connection(descriptor, this);
    connect(connection, SIGNAL(packetReceived(Packet * )), this, SLOT(on_packetReceived(Packet * )));
    connect(connection, SIGNAL(disconnected()), this, SLOT(on_connectionLost()));
    auto client = new RemoteClient(this);
    m_clients.insert(connection, client);
}

void Server::on_packetReceived(Packet* packet)
{
    auto connection = (Connection*) sender();
    RemoteClient* client = m_clients[connection];

    int packetId = packet->getId();
    switch(packetId)
    {
        case 1:
        {
            auto handshakePacket = (HandshakePacket*) packet;

            int magic = handshakePacket->getMagic();
            if(magic != MAGIC)
                connection->disconnectFromHost();

            QString nickname = handshakePacket->getNickname();
            client->setNickname(nickname);

            ClientsListPacket clientsListPacket(getClientsList());
            connection->send(clientsListPacket);

            broadcast(ClientJoinedPacket(nickname));

            delete packet;
            break;
        }
        case 5:
        {
            auto chatMessagePacket = (ChatMessagePacket*) packet;

            broadcast(*chatMessagePacket);

            delete packet;
            break;
        }
        default:
            break;
    }
}

void Server::on_connectionLost()
{
    auto* connection = (Connection*) sender();
    RemoteClient* client = m_clients[connection];

    QString nickname = client->getNickname();

    m_clients.remove(connection);
    connection->deleteLater();
    client->deleteLater();

    broadcast(ClientLeftPacket(nickname));
}