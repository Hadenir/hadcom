#pragma once

#include <cstdint>

class Packet
{
public:
    Packet(unsigned char packetId = 0);

    virtual ~Packet(){}

    virtual size_t getSize() const = 0;

    virtual char* serialize() const = 0;

    virtual void deserialize(const char* data) = 0;

    unsigned char getId() const;

private:
    unsigned char m_packetId;
};