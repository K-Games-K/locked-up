#pragma once

#include <cstdint>
#include <vector>

#include "network/deserializer.hpp"
#include "network/serializer.hpp"

/// Base class for all packets.
class Packet
{
private:
    /// Unique id for each type of packet.
    const uint16_t PACKET_ID;

public:
    /// Constructs packet and assigns id.
    explicit Packet(uint16_t packet_id);

    /// Returns id of packet.
    constexpr uint16_t get_id() const { return PACKET_ID; }

    /// Serializes packet into vector of raw bytes.
    virtual void serialize(Serializer& serializer) const = 0;

    /// Deserializes packet from vector of raw bytes.
    virtual bool deserialize(Deserializer& deserializer) = 0;
};
