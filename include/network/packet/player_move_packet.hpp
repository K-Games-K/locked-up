#pragma once

#include "packet.hpp"

class PlayerMovePacket : public Packet
{
private:
    uint16_t player_id;
    uint16_t x, y;

public:
    static constexpr uint16_t PACKET_ID = 0x4;

    PlayerMovePacket();

    PlayerMovePacket(uint16_t player_id, uint16_t x, uint16_t y);

    void set_player_id(uint16_t player_id);

    uint16_t get_player_id() const;

    uint16_t get_x() const;

    uint16_t get_y() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
