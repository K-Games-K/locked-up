#pragma once

#include "packet.hpp"

class PlayerMovePacket : public Packet
{
private:
    std::string nickname;
    int x, y;

public:
    static constexpr uint16_t PACKET_ID = 0x4;

    PlayerMovePacket();

    PlayerMovePacket(const std::string nickname, int x, int y);

    std::string get_nickname() const;

    int get_x() const;

    int get_y() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
