#pragma once

#include "packet.hpp"

class NewTurnPacket : public Packet
{
private:
    uint16_t current_player_id;

public:
    static constexpr uint16_t PACKET_ID = 0x9;

    NewTurnPacket();

    NewTurnPacket(uint16_t current_player_id);

    int get_current_player_id() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
