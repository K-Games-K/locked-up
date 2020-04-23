#include "room.hpp"

Room::Room(const std::string& name)
        : name(name)
{}

std::string Room::get_name() const
{
    return name;
}

bool Room::operator==(const Room& other)
{
    return this == &other;
}
