#pragma once

#include "packet.hpp"

class NewTurnPacket : public Packet
{
private:
    uint16_t current_player_id;
    uint16_t current_turn;

public:
    static constexpr uint16_t PACKET_ID = 0x9;

    NewTurnPacket();

    explicit NewTurnPacket(uint16_t current_player_id, uint16_t current_turn);

    int get_current_player_id() const;

    int get_current_turn() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
