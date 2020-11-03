#pragma once

#include <string>

#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "logging.hpp"


class Clue
{
public:
    Clue();
    Clue(std::string name);
    Clue(std::string name, std::string description);

    std::string get_name() const;

    std::string get_description() const;

    int get_time_mod() const;

    int get_place_mod() const;

    void set_info(std::string info);

    void set_name(std::string name);

    void generateClue(std::string player, std::string time, std::string room);

    void generateOptions();

    bool Clue::operator==(const Clue& other) const;

    void debug();


private:
    std::string name;
    std::string description;
    std::string nr;

    //value of these gives maximum interation modification
    int time_mod; 
    int place_mod;

    std::vector<std::string> conditions;
    std::mt19937 gen;
};
