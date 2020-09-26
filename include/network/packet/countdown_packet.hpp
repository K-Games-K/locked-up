#pragma once

#include "packet.hpp"

class CountdownPacket : public Packet
{
private:
    float interval;

public:
    static constexpr uint16_t PACKET_ID = 0x7;

    CountdownPacket();

    CountdownPacket(float interval);

    float get_interval() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
