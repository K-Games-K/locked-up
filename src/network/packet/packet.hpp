#pragma once

#include <cstdint>
#include <vector>
#include <SFML/Network/Packet.hpp>

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
    virtual void serialize(sf::Packet& data) const = 0;

    /// Deserializes packet from vector of raw bytes.
    virtual void deserialize(sf::Packet& data) = 0;
};
