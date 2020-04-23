#pragma once

#include "packet.hpp"
#include "player.hpp"

class PlayersListPacket : public Packet
{
private:
    std::vector<Player> players_list;

public:
    static constexpr uint16_t PACKET_ID = 0x2;

    PlayersListPacket();

    explicit PlayersListPacket(const std::vector<Player>& players_list);

    std::vector<Player> get_players_list() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
