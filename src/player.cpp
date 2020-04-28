#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>

#include "player.hpp"

Player::Player(const std::string& nickname, int x, int y)
    : nickname(nickname), position(x, y)
{}

Player::Player(const std::string& nickname, sf::Vector2i position)
    : nickname(nickname), position(position)
{}

void Player::set_nickname(const std::string& nickname)
{
    this->nickname = nickname;
}

std::string Player::get_nickname() const
{
    return nickname;
}

void Player::set_position(int x, int y)
{
    this->position = {x, y};
}

void Player::set_position(sf::Vector2i position)
{
    this->position = position;
}

sf::Vector2i Player::get_position() const
{
    return position;
}

void Player::move(int x, int y)
{
    this->position += {x, y};
}

const std::vector<int>& Player::get_alibi() const
{
    return alibi;
}

void Player::generate_alibi(const GameBoard& game_board, int crime_room, int length)
{
    size_t rooms_count = game_board.rooms_count();
    std::vector<int> visit_count(rooms_count);

    alibi.clear();
    alibi.push_back(crime_room);

    // Initializing RNG
    static std::mt19937 gen(time(nullptr));
    std::uniform_real_distribution<> rand_percent(0, 100);
    std::uniform_int_distribution<> rand_room(0, rooms_count - 1);

    int current_room = rand_room(gen);
    for(int i = 0; i < length; ++i)
    {
        std::uniform_int_distribution<> rand_neighour(
            0, game_board.get_neighbours(current_room).size() - 1
        );
        int stay = rand_percent(gen);
        if(stay < 25.0 || (alibi.size() >= 2 && alibi[alibi.size() - 2] == alibi[alibi.size() - 1]))
        {
            int neighbour;
            do
            {
                neighbour = game_board.get_neighbours(current_room)[rand_neighour(gen)];
            }
            while(game_board.get_neighbours(current_room).size() > 1 && visit_count[neighbour] > 2);

            current_room = neighbour;
            visit_count[neighbour]++;
        }

        alibi.push_back(current_room);
    }

    std::reverse(alibi.begin(), alibi.end());
    for(auto room : alibi)
        std::cout << room << " ";
    std::cout << std::endl;
}

bool Player::operator==(const Player& other) const
{
    return nickname == other.nickname;
}
