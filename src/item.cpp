#include "../item.hpp"

Item::Item(std::string new_name, std::string new_description, Type type)
	: name(new_name), description(new_description), item_type(type)
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
	return this->item_type;
}
