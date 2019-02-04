#include "local_client.h"

#include "connection.h"
#include "packets/handshake_packet.h"
#include "packets/client_joined_packet.h"
#include "packets/client_left_packet.h"
#include "packets/clients_list_packet.h"
#include "packets/chat_message_packet.h"

LocalClient::LocalClient(Connection* connection, QObject* parent)
        : RemoteClient(parent), m_connection(connection)
{
    connect(m_connection, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(m_connection, SIGNAL(packetReceived(Packet * )), this, SLOT(on_packetReceived(Packet * )));
}

void LocalClient::sendMessage(const QString& message)
{
    m_connection->send(ChatMessagePacket(getNickname(), message));
}

void LocalClient::on_connected()
{
    m_connection->send(HandshakePacket(MAGIC, getNickname()));
    emit connected();
}

void LocalClient::on_packetReceived(Packet* packet)
{
    int packetId = packet->getId();
    qDebug() << "Received packet of id" << packetId;
    switch(packetId)
    {
        case 2:
        {
            auto* clientJoinedPacket = (ClientJoinedPacket*) packet;
            emit clientJoined(clientJoinedPacket->getNickname());

            delete packet;
            break;
        }
        case 3:
        {
            auto clientLeftPacket = (ClientLeftPacket*) packet;
            emit clientLeft(clientLeftPacket->getNickname());

            delete packet;
            break;
        }
        case 4:
        {
            auto clientsListPacket = (ClientsListPacket*) packet;
            emit loggedIn(clientsListPacket->getClients());

            delete packet;
            break;
        }
        case 5:
        {
            auto chatMessagePacket = (ChatMessagePacket*) packet;
            emit messageReceived(chatMessagePacket->getSender(), chatMessagePacket->getMessage());

            delete packet;
            break;
        }
        default:
            break;
    }
}
