#pragma once

#include <cstddef>
#include <cstdint>

class Packet
{
public:
    explicit Packet(uint8_t packetId = 0);

    virtual ~Packet() = default;

    virtual size_t getSize() const = 0;

    virtual char* serialize() const = 0;

    virtual void deserialize(const char* data) = 0;

    uint8_t getId() const;

private:
    uint8_t m_packetId;
};