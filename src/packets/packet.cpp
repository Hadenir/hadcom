#include "packets/packet.h"

Packet::Packet(uint8_t packetId)
        : m_packetId(packetId)
{}

uint8_t Packet::getId() const
{
    return m_packetId;
}