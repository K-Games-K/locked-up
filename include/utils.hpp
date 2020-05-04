#pragma once

#include <SFML/Graphics.hpp>

namespace Utils
{
    float min(float a, float b);

    float max(float a, float b);

    float lerp(float a, float b, float f);

    float clamp(float v, float min, float max);

    float magnitude(sf::Vector2f vec);

    sf::Vector2f normalize(sf::Vector2f vec);

    bool is_number(const std::string& string);

    bool is_printable(const std::string& string);

    std::string to_lower_case(const std::string& string);
}
