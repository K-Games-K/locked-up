#pragma once

#include "action.hpp"
#include "packet.hpp"

class ActionPacket : public Packet
{
private:
    ActionType action_type;

public:
    static constexpr uint16_t PACKET_ID = 0xa;

    ActionPacket();

    explicit ActionPacket(ActionType action_type);

    ActionType get_action_type() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
