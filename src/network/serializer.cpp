#include "network/serializer.hpp"

Serializer::Serializer(size_t capacity)
{
    buffer.reserve(capacity);
}

std::vector<uint8_t> Serializer::finish()
{
    return std::move(buffer);
}

void Serializer::serialize(const std::string& str)
{
    auto length = (uint16_t) str.length();  // First 2 bytes contain length of string.
    serialize(length);
    for(auto ch : str)
    {
        serialize(ch);
    }
}
