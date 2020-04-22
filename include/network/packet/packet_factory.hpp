#pragma once

#include <cstdint>
#include <memory>

#include "packet.hpp"

namespace PacketFactory
{
    std::unique_ptr<Packet> create(uint16_t packet_id);
}
