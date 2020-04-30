#include "item.hpp"

Item::Item(std::string name, std::string destription, Type type)
    : name(name), description(destription), type(type)
{}

std::string Item::get_name() const
{
    return this->name;
}

std::string Item::get_description() const
{
    return this->description;
}

Item::Type Item::get_type() const
{
    return this->type;
}
