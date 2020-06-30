#pragma once

#include "packet.hpp"
#include "../../player.hpp"

class PlayersListPacket : public Packet
{
private:
    uint16_t player_id;                 // Receivers index in players_list array.
    std::vector<Player> players_list;

public:
    static constexpr uint16_t PACKET_ID = 0x2;

    PlayersListPacket();

    explicit PlayersListPacket(uint16_t player_id, const std::vector<Player>& players_list);

    uint16_t get_player_id() const;

    std::vector<Player> get_players_list() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
