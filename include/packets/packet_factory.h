#pragma once

#include <cstdint>

class Packet;

class PacketFactory
{
public:
    static Packet* Create(uint8_t packetId);
};