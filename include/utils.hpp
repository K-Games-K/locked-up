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

    bool is_inside(sf::Vector2f start, sf::Vector2f size, sf::Vector2f point);
}
