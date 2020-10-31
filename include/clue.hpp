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

    void set_name(std::string name);

    void generateClue(std::string time, std::string player, std::string preroom,std::string nextroom);

    void generateClue(std::string time, std::string player);

    bool Clue::operator==(const Clue& other) const;


private:
    std::string name;
    std::string description;

    std::mt19937 gen;
};
