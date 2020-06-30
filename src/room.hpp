#pragma once

#include <string>
#include <vector>

#include "item.hpp"

class Room
{
private:
    std::string name;

    std::vector<Item> items;

    std::vector<std::pair<int, std::string>> visitors;

public:
    Room(const std::string& name, const std::vector<Item>& items = {},
        const std::vector<std::pair<int, std::string>>& visitors = {});

    std::string get_name() const;

    std::vector<Item>& get_items();

    const std::vector<Item>& get_items() const;

    void set_visitors(const std::vector<std::pair<int, std::string>>& visitors);

    std::vector<std::pair<int, std::string>>& get_visitors();

    const std::vector<std::pair<int, std::string>>& get_visitors() const;

    bool operator==(const Room& other) const;
};
