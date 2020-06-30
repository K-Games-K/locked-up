#pragma once

#include "packet.hpp"

class PlayerReadyPacket : public Packet
{
private:
    int player_id;
    bool ready;

public:
    static constexpr uint16_t PACKET_ID = 0x6;

    PlayerReadyPacket();

    PlayerReadyPacket(int player_id, bool ready);

    int get_player_id() const;

    bool is_ready() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
