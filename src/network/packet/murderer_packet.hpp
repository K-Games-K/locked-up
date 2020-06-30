#pragma once

#include "packet.hpp"

class MurdererPacket : public Packet
{
public:
    static constexpr uint16_t PACKET_ID = 0xc;

    MurdererPacket();

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
