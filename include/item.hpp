#pragma once

#include <string>

class Item
{
public:
    enum class Type
    {
        Prove,
        Clue
    };

    Item() = default;

    Item(std::string name, std::string description, Type type = Type::Clue);

    std::string get_name() const;

    std::string get_description() const;

    Type get_type() const;

private:
    std::string name;
    std::string description;

    Type type;
};
