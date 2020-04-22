#pragma once

#include <string>

#include "packet.hpp"

/// Packet for debugging purposes.
class DebugPacket : public Packet
{
private:
        std::string debug_msg;

public:
    static constexpr uint16_t PACKET_ID = 0x0;

    DebugPacket();

    explicit DebugPacket(const std::string& debug_msg);

    std::string get_message() const;

    std::vector<uint8_t> serialize() const override;

    bool deserialize(std::vector<uint8_t> data) override;
};
