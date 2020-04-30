#include "network/packet/game_start_packet.hpp"

GameStartPacket::GameStartPacket()
    : Packet(PACKET_ID)
{}

GameStartPacket::GameStartPacket(int16_t start_x, int16_t start_y,
    std::vector<std::vector<int32_t>>& alibis, uint32_t crime_room, Item crime_item)
    : Packet(PACKET_ID), start_x(start_x), start_y(start_y), alibis(std::move(alibis)),
    crime_room(crime_room), crime_item(crime_item)
{}

int16_t GameStartPacket::get_start_x() const
{
    return start_x;
}

int16_t GameStartPacket::get_start_y() const
{
    return start_y;
}

const std::vector<std::vector<int32_t>>& GameStartPacket::get_alibis() const
{
    return alibis;
}

uint32_t GameStartPacket::get_crime_room() const
{
    return crime_room;
}

Item GameStartPacket::get_crime_item() const
{
    return crime_item;
}

void GameStartPacket::serialize(sf::Packet& data) const
{
    data << start_x << start_y;
    data << alibis.size();
    for(auto& alibi : alibis)
    {
        data << alibi.size();
        for(auto room : alibi)
            data << room;
    }

    data << crime_room;
    data << (uint8_t) (crime_item.get_type() == Item::Type::Prove ? 1 : 0);
    data << crime_item.get_name() << crime_item.get_description();
}

void GameStartPacket::deserialize(sf::Packet& data)
{
    data >> start_x >> start_y;
    size_t alibis_size;
    data >> alibis_size;
    alibis.reserve(alibis_size);
    for(int i = 0; i < alibis_size; ++i)
    {
        std::vector<int32_t> alibi;
        size_t alibi_length;
        data >> alibi_length;
        alibi.reserve(alibi_length);
        for(int j = 0; j < alibi_length; ++j)
        {
            int32_t room;
            data >> room;
            alibi.push_back(room);
        }
        alibis.push_back(std::move(alibi));
    }

    data >> crime_room;

    uint8_t type;
    data >> type;
    std::string crime_item_name, crime_item_description;
    data >> crime_item_name >> crime_item_description;
    crime_item = Item(
        crime_item_name, crime_item_description, type == 1 ? Item::Type::Prove : Item::Type::Clue
    );
}






