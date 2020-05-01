#include "network/packet/item_found_packet.hpp"

ItemFoundPacket::ItemFoundPacket()
    : Packet(PACKET_ID)
{}

ItemFoundPacket::ItemFoundPacket(Item item)
    : Packet(PACKET_ID), item(item)
{}

Item ItemFoundPacket::get_item() const
{
    return item;
}

void ItemFoundPacket::serialize(sf::Packet& data) const
{
    data << static_cast<uint16_t>(item.get_type());
    data << item.get_name() << item.get_description();
}

void ItemFoundPacket::deserialize(sf::Packet& data)
{
    uint16_t type;
    data >> type;
    std::string name, description;
    data >> name >> description;

    item = Item(name, description, static_cast<Item::Type>(type));
}


