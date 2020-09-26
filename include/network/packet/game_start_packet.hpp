#pragma once

#include "packet.hpp"
#include "item.hpp"

class GameStartPacket : public Packet
{
private:
    int16_t start_x;
    int16_t start_y;

    std::vector<std::vector<int32_t>> alibis;

    uint32_t crime_room;
    Item crime_item;

    uint32_t turns_per_game;

public:
    static constexpr uint16_t PACKET_ID = 0x8;

    GameStartPacket();

    GameStartPacket(int16_t start_x, int16_t start_y, std::vector<std::vector<int32_t>>& alibis,
        uint32_t crime_room, Item crime_item, uint32_t game_rounds);

    int16_t get_start_x() const;

    int16_t get_start_y() const;

    const std::vector<std::vector<int32_t>>& get_alibis() const;

    uint32_t get_crime_room() const;

    Item get_crime_item() const;

    uint32_t get_turns_per_game() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
