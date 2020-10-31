#include "room.hpp"

Room::Room(const std::string& name, const std::vector<Item>& items,
    const std::vector<std::pair<int, std::string>>& visitors)
    : name(name), items(items), visitors(visitors)
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

void Room::set_visitors(const std::vector<std::pair<int, std::string>>& visitors)
{
    this->visitors = visitors;
}

void Room::set_clues(const std::vector<Clue>& clues)
{
    this->clues = clues;
}

std::vector<std::pair<int, std::string>>& Room::get_visitors()
{
    return visitors;
}

const std::vector<std::pair<int, std::string>>& Room::get_visitors() const
{
    return visitors;
}

std::vector<Clue>& Room::get_clues()
{
    return clues;
}

const std::vector<Clue>& Room::get_clues() const
{
    return clues;
}

bool Room::operator==(const Room& other) const
{
    return this == &other;
}
