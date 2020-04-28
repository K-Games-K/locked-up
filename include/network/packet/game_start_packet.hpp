#pragma once

#include "packet.hpp"

class GameStartPacket : public Packet
{
private:
    int16_t start_x;
    int16_t start_y;

    std::vector<std::vector<int32_t>> alibis;

public:
    static constexpr uint16_t PACKET_ID = 0x8;

    GameStartPacket();

    GameStartPacket(int16_t start_x, int16_t start_y, std::vector<std::vector<int32_t>>& alibis);

    int16_t get_start_x() const;

    int16_t get_start_y() const;

    const std::vector<std::vector<int32_t>>& get_alibis() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
