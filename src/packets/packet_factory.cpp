#include "packets/packet_factory.h"
#include "packets/handshake_packet.h"
#include "packets/client_joined_packet.h"
#include "packets/client_left_packet.h"
#include "packets/clients_list_packet.h"

Packet* PacketFactory::Create(int packetId)
{
    switch(packetId)
    {
        case 1:
            return new HandshakePacket;
        case 2:
            return new ClientJoinedPacket;
        case 3:
            return new ClientLeftPacket;
        case 4:
            return new ClientsListPacket;
        default:
            return nullptr;
    }
}