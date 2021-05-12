#include "clue.hpp"
#include <game_board.hpp>

Clue::Clue()
    : name(""), description(""), gen(time(NULL)), time_mod(0), place_mod(0), nr("")
{
    conditions.clear();
}

Clue::Clue(std::string name)
    : name(name), description(""), gen(time(NULL)), time_mod(0), place_mod(0), nr("")
{
    conditions.clear();
}

Clue::Clue(std::string name, std::string description)
    : name(name), description(description), gen(time(NULL)), time_mod(0), place_mod(0), nr("")
{
    conditions.clear();
}

std::string Clue::get_name() const
{
    return this->name;
}

std::string Clue::get_description() const
{
    return this->description;
}

int Clue::get_time_mod() const
{
    return this->time_mod;
}

int Clue::get_place_mod() const
{
    return this->place_mod;
}

void Clue::set_info(std::string info)
{
    if (info == "first") conditions.push_back("-notFirstPlace");
    if (info == "last") conditions.push_back("-notLastPlace");
}

void Clue::set_name(std::string name)
{
    this->name = name;
}

void Clue::generateClue(std::string player, std::string time, std::string room)
{
    std::ifstream file("assets/clues/cluespack.CLS");
    int buf;
    int options, winner;
    file >> options >> buf >> buf;

    std::string line_str;
    for (std::getline(file >> std::ws, line_str); !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        std::string clue_id;
        std::getline(line >> std::ws, clue_id, ':');

        if (clue_id != nr) continue;

        std::getline(line >> std::ws, clue_id, ':');  //flag rem

        std::stringstream ss;
        std::string word;
        while (!line.eof())
        {
            std::getline(line >> std::ws, word, ' ');
            if (word == "$player") word = player;
            if (word == "$next_time") word = time;
            if (word == "$pre_time") word = time;
            if (word == "$pre_room") word = room;
            if (word == "$next_room") word = room;
            if (word == "$name") word = name;

            ss << word << " ";
        }

        this->description = ss.str();
        return;

    }

    file.close();
}

void Clue::generateOptions()
{
    std::random_device rd;
    std::mt19937 generator(rd());

    int max_mod = 4;
    std::ifstream file("assets/clues/cluespack.CLS");

    int buf;
    int options, winner;
    bool ok = false;
    file >> options >> buf >> buf;
    std::uniform_int_distribution<int> random_clue(1, options);

    std::uniform_int_distribution<int> mod_val(1, max_mod);

    while (!ok)
    {
        ok = true;
        winner = random_clue(generator);
        winner = random_clue(generator);


        std::string line_str;
        for (std::getline(file >> std::ws, line_str); !file.eof();
            std::getline(file >> std::ws, line_str))
        {
            std::stringstream line(line_str);
            std::string condition;
            std::getline(line >> std::ws, nr, ':');

            if (nr != std::to_string(winner)) continue;

            std::getline(line >> std::ws, condition, ':');
            if (std::find(conditions.begin(), conditions.end(), condition) != conditions.end())
            {
                ok = false;
                break;
            }

            std::string word;
            while (!line.eof())
            {
                std::getline(line >> std::ws, word, ' ');
                if (word == "$pre_time") time_mod = -mod_val(generator);
                if (word == "next_time") time_mod = mod_val(generator);
                if (word == "$pre_room") place_mod = -1;
                if (word == "$after_room") place_mod = 1;

            }

            return;


        }
    }
    file.close();

}

void Clue::debug()
{
    time_mod = 1;
}

bool Clue::operator==(const Clue& other) const
{
    return this->name == other.name && this->description == other.description;
}
