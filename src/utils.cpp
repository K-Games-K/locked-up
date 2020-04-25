#include <cmath>

#include "utils.hpp"

float Utils::min(float a, float b)
{
    return a < b ? a : b;
}

float Utils::max(float a, float b)
{
    return a > b ? a : b;
}

float Utils::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float Utils::clamp(float v, float min, float max)
{
    if(v < min)
        return min;
    else if(v > max)
        return max;

    return v;
}

float Utils::magnitude(sf::Vector2f vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f Utils::normalize(sf::Vector2f vec)
{
    return vec / magnitude(vec);
}

bool Utils::is_inside(sf::Vector2f start, sf::Vector2f size, sf::Vector2f point)
{
    return start.x <= point.x && start.x + size.x >= point.x &&
            start.y <= point.y && start.y + size.y >= point.y;
}
