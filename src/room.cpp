#include "room.hpp"

Room::Room(const std::string& name, const std::vector<Item>& items)
        : name(name), items(items)
{}

std::string Room::get_name() const
{
    return name;
}

std::vector<Item>& Room::get_items()
{
    return items;
}

const std::vector<Item>& Room::get_items() const
{
    return items;
}

bool Room::operator==(const Room& other) const
{
    return this == &other;
}
