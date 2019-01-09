#include "server.h"

#include "connection.h"
#include "remote_client.h"
#include "packets/handshake_packet.h"
#include "packets/client_joined_packet.h"
#include "packets/client_left_packet.h"

Server::Server(QObject* parent)
        : QTcpServer(parent)
{}

bool Server::bind(quint16 port)
{
    return listen(QHostAddress::LocalHost, port);
}

bool Server::broadcast(const Packet& packet) const
{
    for(auto connection : m_clients.keys())
    {
        connection->send(packet);
    }
}

void Server::incomingConnection(qintptr descriptor)
{
    Connection* connection = new Connection(descriptor, this);
    connect(connection, SIGNAL(packetReceived(Packet * )), this, SLOT(on_packetReceived(Packet * )));
    connect(connection, SIGNAL(disconnected()), this, SLOT(on_connectionLost()));
    RemoteClient* client = new RemoteClient(this);
    m_clients.insert(connection, client);
}

void Server::on_packetReceived(Packet* packet)
{
    Connection* connection = (Connection*) sender();
    RemoteClient* client = m_clients[connection];

    int packetId = packet->getId();
    switch(packetId)
    {
        case 1:
        {
            HandshakePacket* handshakePacket = (HandshakePacket*) packet;

            int magic = handshakePacket->getMagic();
            if(magic != MAGIC)
                connection->disconnectFromHost();

            QString nickname = handshakePacket->getNickname();
            client->setNickname(nickname);

            connection->send(*packet);
            broadcast(ClientJoinedPacket(nickname));

            delete packet;
            break;
        }
        default:
            break;
    }
}

void Server::on_connectionLost()
{
    Connection* connection = (Connection*) sender();
    RemoteClient* client = m_clients[connection];

    QString nickname = client->getNickname();

    m_clients.remove(connection);
    connection->deleteLater();
    client->deleteLater();

    broadcast(ClientLeftPacket(nickname));
}