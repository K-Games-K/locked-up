#pragma once

#include <cstdint>
#include <string>
#include <vector>

/// A buffer for deserializing data.
class Deserializer
{
private:
    std::vector<uint8_t> buffer;

public:
    /// Constructs `Deserializer` with empty internal buffer.
    Deserializer() = default;

    /// Cosntructs `Deserializer` and puts `data` into its internal buffer.
    explicit Deserializer(std::vector<uint8_t>& data);

    /// Returns `true` if there is no data left in the buffer.
    bool empty();

    /// Consumes part of buffer and deserializes it into numerical value.
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    bool deserialize(T& value)
    {
        auto size = sizeof(T);
        if(buffer.size() < size)
            return false;

        value = 0;
        for(int i = 0; i < size; ++i)
        {
            value |= buffer[i] << 8 * i;
        }

        buffer.erase(buffer.begin(), buffer.begin() + size);
        return true;
    }

    /// Consumes part of buffer and deserializes it into `std::string`.
    bool deserialize(std::string& str);

    /// Consumes part of buffer and deserializes it into `std::vector`.
    template<typename T>
    bool deserialize(std::vector<T>& vec)
    {
        uint16_t size;
        if(!deserialize(size))
            return false;

        vec.resize(size);
        for(int i = 0; i < size; ++i)
        {
            if(!deserialize(vec[i]))
                return false;
        }

        return true;
    }
};
