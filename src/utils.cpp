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

bool Utils::is_number(const std::string& string)
{
    for(auto& ch : string)
    {
        if(ch < '0' || ch > '9')
            return false;
    }

    return true;
}

bool Utils::is_printable(const std::string& string)
{
    for(auto& ch : string)
    {
        if(!isprint(ch))
            return false;
    }

    return true;
}

std::string Utils::to_lower_case(const std::string& string)
{
    std::string result(string.size(), ' ');
    std::transform(
        string.begin(), string.end(), result.begin(), [](char c) { return std::tolower(c); }
    );

    return result;
}
