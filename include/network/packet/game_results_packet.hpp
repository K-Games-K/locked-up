#pragma once

#include "packet.hpp"

class GameResultsPacket : public Packet
{
private:
    std::string murderer;
    std::string voting_result;

public:
    static constexpr uint16_t PACKET_ID = 0xe;

    GameResultsPacket();

    GameResultsPacket(std::string murderer, std::string voting_result);

    std::string get_murderer() const;

    std::string get_voting_result() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
