#pragma once

#include <cstdint>
#include <string>
#include <vector>

/// A buffer for serializing data.
class Serializer
{
private:
    std::vector<uint8_t> buffer;

public:
    /// Constructs empty `Serializer`.
    Serializer() = default;

    /// Constructs empty `Serializer` with buffer allocated for `capacity` bytes.
    explicit Serializer(size_t capacity);

    /// Returns internal buffer with serialized data.
    std::vector<uint8_t> finish();

    /// Serializes numerical `value` and appends it to internal buffer.
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    void serialize(T value)
    {
        for(int i = 0; i < sizeof(T); ++i)
        {
            buffer.push_back((value >> 8 * i) & 0xff);
        }
    }

    /// Serializes `std::string` and appends it to internal buffer.
    void serialize(const std::string& str);

    /// Serializes `std::vector` and appends it to internal buffer.
    template<typename T>
    void serialize(const std::vector<T>& vec)
    {
        auto size = (uint16_t) vec.size();  // First 2 bytes contain size of vector.
        serialize(size);
        for(auto elem : vec)
        {
            serialize(elem);
        }
    }
};


