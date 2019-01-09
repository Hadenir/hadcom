#include "packets/packet.h"

Packet::Packet(unsigned char packetId)
        : m_packetId(packetId)
{}

unsigned char Packet::getId() const
{
    return m_packetId;
}