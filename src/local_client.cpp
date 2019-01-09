#include "local_client.h"

#include "connection.h"
#include "packets/handshake_packet.h"
#include "packets/client_joined_packet.h"
#include "packets/client_left_packet.h"

LocalClient::LocalClient(Connection* connection, QObject* parent)
        : RemoteClient(parent), m_connection(connection)
{
    connect(m_connection, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(m_connection, SIGNAL(packetReceived(Packet * )), this, SLOT(on_packetReceived(Packet * )));
}

void LocalClient::sendMessage(const QString& message)
{
    //m_connection->write(message.toLatin1().data());
}

void LocalClient::on_connected()
{
    m_connection->send(HandshakePacket(MAGIC, getNickname()));
    emit connected();
}

void LocalClient::on_packetReceived(Packet* packet)
{
    int packetId = packet->getId();
    qDebug() << "Received packet of id " << packetId;
    switch(packetId)
    {
        case 1:
        {
            HandshakePacket* handshakePacket = (HandshakePacket*) packet;
            if(handshakePacket->getNickname() == getNickname())
                emit loggedIn();

            delete packet;
            break;
        }
        case 2:
        {
            ClientJoinedPacket* clientJoinedPacket = (ClientJoinedPacket*) packet;
            emit clientJoined(clientJoinedPacket->getNickname());

            delete packet;
            break;
        }
        case 3:
        {
            ClientLeftPacket* clientLeftPacket = (ClientLeftPacket*) packet;
            emit clientLeft(clientLeftPacket->getNickname());

            delete packet;
            break;
        }
        default:
            break;
    }
}
