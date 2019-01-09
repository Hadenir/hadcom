#pragma once

class Packet;

class PacketFactory
{
public:
    static Packet* Create(int packetId);
};