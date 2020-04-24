#pragma once

#include "packet.hpp"

class PlayerMovePacket : public Packet
{
private:
    bool relative;
    uint16_t player_id;
    int16_t x, y;

public:
    static constexpr uint16_t PACKET_ID = 0x4;

    PlayerMovePacket();

    PlayerMovePacket(int16_t x, int16_t y, bool relative = true, uint16_t player_id = 0);

    void set_player_id(uint16_t player_id);

    uint16_t get_player_id() const;

    int16_t get_x() const;

    int16_t get_y() const;

    bool is_relative() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
