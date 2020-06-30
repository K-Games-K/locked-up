#pragma once

#include "packet.hpp"
#include "../../item.hpp"

class ClueFoundPacket : public Packet
{
private:
    std::string clue;
    std::string time;

public:
    static constexpr uint16_t PACKET_ID = 0xb;

    ClueFoundPacket();

    explicit ClueFoundPacket(std::string clue, std::string time);

    std::string get_clue() const;

    std::string get_time() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
