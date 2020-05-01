#pragma once

#include "packet.hpp"

class JoinGamePacket : public Packet
{
private:
    std::string nickname;
    std::string avatar;

public:
    static constexpr uint16_t PACKET_ID = 0x1;

    JoinGamePacket();

    explicit JoinGamePacket(const std::string& nickname, const std::string& avatar);

    std::string get_nickname() const;

    std::string get_avatar() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
