#include "network/deserializer.hpp"

Deserializer::Deserializer(std::vector<uint8_t>& data)
        : buffer(std::move(data))
{}

bool Deserializer::empty()
{
    return buffer.empty();
}

bool Deserializer::deserialize(std::string& str)
{
    uint16_t length;
    if(!deserialize(length))
        return false;

    str.resize(length);
    for(int i = 0; i < length; ++i)
    {
        if(!deserialize(str[i]))
            return false;
    }

    return true;
}
