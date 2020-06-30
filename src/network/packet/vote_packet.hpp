#pragma once

#include "packet.hpp"

class VotePacket : public Packet
{
private:
    uint16_t player_id;

public:
    static constexpr uint16_t PACKET_ID = 0xd;

    VotePacket();

    explicit VotePacket(uint16_t player_id);

    uint16_t get_player_id() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
