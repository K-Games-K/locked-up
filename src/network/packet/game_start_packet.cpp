#include "network/packet/game_start_packet.hpp"

GameStartPacket::GameStartPacket()
    : Packet(PACKET_ID)
{}

GameStartPacket::GameStartPacket(int16_t start_x, int16_t start_y,
    std::vector<std::vector<int32_t>>& alibis)
    : Packet(PACKET_ID), start_x(start_x), start_y(start_y), alibis(std::move(alibis))
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
}






