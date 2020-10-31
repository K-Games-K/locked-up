#pragma once

#include <string>
#include <vector>

#include "item.hpp"
#include "clue.hpp"

class Room
{
private:
    std::string name;

    std::vector<Item> items;

    std::vector<std::pair<int, std::string>> visitors;
    std::vector<Clue> clues;

public:
    Room(const std::string& name, const std::vector<Item>& items = {},
        const std::vector<std::pair<int, std::string>>& visitors = {});

    std::string get_name() const;

    std::vector<Item>& get_items();

    const std::vector<Item>& get_items() const;

    void set_visitors(const std::vector<std::pair<int, std::string>>& visitors);

    void set_clues(const std::vector<Clue>& clues);

    std::vector<std::pair<int, std::string>>& get_visitors();

    const std::vector<std::pair<int, std::string>>& get_visitors() const;

    std::vector<Clue>& get_clues();

    const std::vector<Clue>& get_clues() const;

    bool operator==(const Room& other) const;
};
