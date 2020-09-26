#pragma once

#include "packet.hpp"

class DisconnectPacket : public Packet
{
private:
    std::string reason;

public:
    static constexpr uint16_t PACKET_ID = 0x3;

    explicit DisconnectPacket(const std::string& reason = "Disconnected");

    std::string get_reason() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
