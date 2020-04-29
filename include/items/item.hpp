#pragma once
#include <string>

class Item
{
	enum class Type { Prove, Clue };

private:
	std::string name;
	std::string description;

	Type item_type;
	
public:
	Item(std::string new_name, std::string new_destription, Type type);
	std::string Item::get_name() const;
	std::string Item::get_description() const;
	Type get_type() const;

};