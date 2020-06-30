#pragma once

#include "packet.hpp"
#include "../../action.hpp"

class FakeCluePacket : public Packet
{
private:
    ActionType action_type;
    uint32_t time;
    uint32_t player_id;

public:
    static constexpr uint16_t PACKET_ID = 0xa;

    FakeCluePacket();

    FakeCluePacket(uint32_t time, uint32_t player_id);

    uint32_t get_time() const;

    uint32_t get_player_id() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
