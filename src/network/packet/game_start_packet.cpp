#include "game_start_packet.hpp"

GameStartPacket::GameStartPacket()
    : Packet(PACKET_ID)
{}

GameStartPacket::GameStartPacket(int16_t start_x, int16_t start_y,
    std::vector<std::vector<int32_t>>& alibis, uint32_t crime_room, Item crime_item, uint32_t turns_per_game)
    : Packet(PACKET_ID), start_x(start_x), start_y(start_y), alibis(std::move(alibis)),
    crime_room(crime_room), crime_item(crime_item), turns_per_game(turns_per_game)
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
    data << (sf::Uint64)alibis.size();
    for(auto& alibi : alibis)
    {
        data << (sf::Uint64)alibi.size();
        for(auto room : alibi)
            data << room;
    }

    data << crime_room;
    data << static_cast<uint8_t>(crime_item.get_type());
    data << crime_item.get_name() << crime_item.get_description();
    data << turns_per_game;
}

uint32_t GameStartPacket::get_turns_per_game() const
{
    return turns_per_game;
}

void GameStartPacket::deserialize(sf::Packet& data)
{
    data >> start_x >> start_y;
    sf::Uint64 alibis_size;
    data >> alibis_size;
    alibis.reserve(alibis_size);
    for(int i = 0; i < alibis_size; ++i)
    {
        std::vector<int32_t> alibi;
        sf::Uint64 alibi_length;
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
        crime_item_name, crime_item_description, static_cast<Item::Type>(type)
    );
    data >> turns_per_game;

    
}




