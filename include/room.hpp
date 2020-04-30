#pragma once

#include <string>
#include <vector>

#include "item.hpp"

class Room
{
private:
    std::string name;

    std::vector<Item> items;

public:
    Room(const std::string& name, const std::vector<Item>& items = {});

    std::string get_name() const;

    std::vector<Item>& get_items();

    const std::vector<Item>& get_items() const;

    bool operator==(const Room& other) const;
};
