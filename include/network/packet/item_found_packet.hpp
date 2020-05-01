#pragma once

#include "item.hpp"
#include "packet.hpp"

class ItemFoundPacket : public Packet
{
private:
    Item item;

public:
    static constexpr uint16_t PACKET_ID = 0xb;

    ItemFoundPacket();

    explicit ItemFoundPacket(Item item);

    Item get_item() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
