#include "clue.hpp"

Clue::Clue()
    : name(""), description(""), gen(time(nullptr))
{}

Clue::Clue(std::string name)
    : name(name), description(""), gen(time(nullptr))
{}

Clue::Clue(std::string name, std::string destription)
    : name(name), description(destription), gen(time(nullptr))
{}

std::string Clue::get_name() const
{
    return this->name;
}

std::string Clue::get_description() const
{
    return this->description;
}

void Clue::set_name(std::string name)
{
    this->name = name;
}

void Clue::generateClue(std::string time, std::string player, std::string preroom, std::string nextroom)
{

    std::ifstream file("assets/clues/cluespack.CLS");

    int options, winner;
    file >> options;
    std::uniform_int_distribution<int> random_clue(1, options);

    winner = random_clue(this->gen);

    std::string line_str;
    for (std::getline(file >> std::ws, line_str); !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        std::string room_name;
        std::getline(line >> std::ws, room_name, ':');

        if (room_name != std::to_string(winner)) continue;

        std::stringstream ss;
        std::string word;
        while (!line.eof())
        {
            std::getline(line >> std::ws, word, ' ');
            if (word == "$player") word = player;
            if (word == "$time") word = time;
            if (word == "$preroom") word = preroom;
            if (word == "$nextroom") word = nextroom;
            if (word == "$name") word = name;

            ss << word << " ";
        }

        this->description = ss.str();
        return;

    }

    file.close();
}

void Clue::generateClue(std::string time, std::string player)
{

    std::ifstream file("assets/clues/cluespack.CLS");

    int options, winner;
    file >> options;
    std::uniform_int_distribution<int> random_clue(3, options);

    winner = random_clue(this->gen);

    std::string line_str;
    for (std::getline(file >> std::ws, line_str); !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        std::string room_name;
        std::getline(line >> std::ws, room_name, ':');

        if (room_name != std::to_string(winner)) continue;

        std::stringstream ss;
        std::string word;
        while (!line.eof())
        {
            std::getline(line >> std::ws, word, ' ');
            if (word == "$player") word = player;
            if (word == "$time") word = time;
            if (word == "$name") word = name;

            ss << word << " ";
        }

        this->description = ss.str();
        return;

    }

    file.close();
}

bool Clue::operator==(const Clue& other) const
{
    return this == &other;
}
